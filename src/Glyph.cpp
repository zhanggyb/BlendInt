/**
 * @brief implementations for Glyph
 */

#include <string.h>
#include <BIL/Glyph.h>
#include <BIL/FontConfig.h>
#include <BIL/FontEngine.h>
#include <string>

namespace BIL {

	Glyph::Glyph (wchar_t charcode, FontEngine* fontlib)
			: _lib(fontlib), _charcode(0), _glyphIndex(0), _texture(0), _displist(
			        0), _dpi(96)
	{
		memset(&_metrics, 0, sizeof(Metrics));
	}

	Glyph::Glyph (wchar_t charcode, const Font& font, unsigned int dpi,
	        FontEngine* fontlib)
			: _lib(fontlib), _charcode(charcode), _glyphIndex(0), _texture(0), _displist(
			        0), _font(font), _dpi(dpi)
	{
		memset(&_metrics, 0, sizeof(Metrics));

		makeDisplayList();
	}

	Glyph::Glyph (const Glyph& orig)
			: _lib(NULL), _charcode(0), _glyphIndex(0), _texture(0), _displist(
			        0), _dpi(96)
	{
		// TODO: copy constructor
	}

	Glyph& Glyph::operator = (const Glyph& orig)
	{
		// TODO: assignment
		return *this;
	}

	Glyph::~Glyph ()
	{
		resetGL();
	}

	void Glyph::setCharacter (wchar_t charcode)
	{
		// if we already create texture and display list for charcode
		if (_charcode == charcode) {
			return;
		}

		resetGL();
		_charcode = charcode;

		makeDisplayList();
	}

	void Glyph::setFontType (FontEngine* fontlib)
	{
		if (_lib == fontlib) {
			return;
		}

		resetGL ();
		_lib = fontlib;

		makeDisplayList();
	}

	void Glyph::render (void)
	{
		if (!glIsTexture(_texture))
			return;
		if (!glIsList(_displist))
			return;

		glEnable(GL_TEXTURE_2D);

		glPushMatrix();

		glMatrixMode(GL_MODELVIEW);

		glTranslatef((float) _metrics.horiBearingX,
		        (float) _metrics.horiBearingY - (float) _metrics.height, 0);

		glBindTexture(GL_TEXTURE_2D, _texture);
		glCallLists(1, GL_UNSIGNED_BYTE, &_displist);

		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
	}

	bool Glyph::makeDisplayList (void)
	{
		FontEngine* fontlib = NULL;

		// if _fonttype is not set, use default font
		if (_lib == NULL) {
			FontConfig* fontserv = FontConfig::getService();

			if (fontserv == NULL) {
				return false;
			}

			if (!fontserv->isInitialized()) {
				return false;
			}
			fontlib = new FontEngine(fontserv->getBuffer(),
			        fontserv->getBufferSize());
		} else {
			fontlib = _lib;
		}

		if (!fontlib->isValid()) {
			cerr << "Cannot get Font" << endl;
			return false;
		}

		fontlib->setCharSize(_font.size, _dpi);
		_glyphIndex = fontlib->getCharIndex(_charcode);
		if (_glyphIndex == 0)
			return false;

		//bool result = font.loadCharacter(_charcode, FT_LOAD_RENDER);
		bool result = fontlib->loadGlyph(_glyphIndex);
		if (!result)
			return false;

		FT_Face face = fontlib->getFontFace();

		fillMetrics(face);

		result = fontlib->renderGlyph();
		if (!result)
			return false;

		int rows = face->glyph->bitmap.rows;
		int width = face->glyph->bitmap.width;

		width = next_p2(width);
		rows = next_p2(rows);

		// Allocate memory for the texture data.
		GLubyte* fontimage = new GLubyte[2 * width * rows];

		// Move the face's glyph into a Glyph object.
		FT_Glyph glyph;

		if (FT_Get_Glyph(face->glyph, &glyph)) {
			delete[] fontimage;
			if (_lib == NULL)
				delete fontlib;
			return false;
		}

		FT_BBox acbox;
		FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_UNSCALED, &acbox);

		_cbox.vec.x = (acbox.xMax - acbox.xMin) >> 6;
		_cbox.vec.y = (acbox.yMax - acbox.yMin) >> 6;

		// Convert the glyph to a bitmap;
		FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
		// FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph) glyph;

		FT_Bitmap& bitmap = face->glyph->bitmap;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < width; j++) {
				fontimage[2 * (i * width + j)] = 255;
				fontimage[2 * (i * width + j) + 1] =
				        (i >= bitmap.rows || j >= bitmap.width) ?
				                0 : bitmap.buffer[bitmap.width * i + j];
			}
		}

		glGenTextures(1, &_texture);
		glBindTexture( GL_TEXTURE_2D, _texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// GL_LUMINANCE_ALPHA to use 2 channel data.
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, rows, 0,
		GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, fontimage);

		delete[] fontimage;

		_displist = glGenLists(1);

		glNewList(_displist, GL_COMPILE);

		glBindTexture(GL_TEXTURE_2D, _texture);

		float x = (float) bitmap.width / (float) width;
		float y = (float) bitmap.rows  / (float) rows;

		glBegin(GL_QUADS);
		glTexCoord2d(0, 0);
		glVertex2f(0, bitmap.rows);
		glTexCoord2d(0, y);
		glVertex2f(0, 0);
		glTexCoord2d(x, y);
		glVertex2f(bitmap.width, 0);
		glTexCoord2d(x, 0);
		glVertex2f(bitmap.width, bitmap.rows);
		glEnd();

		glEndList();

		if (_lib == NULL) {
			delete fontlib;
		}

		return true;
	}

	void Glyph::resetGL (void)
	{
		if (glIsList(_displist)) {
			glDeleteLists(_displist, 1);
		}
		if (glIsTexture(_texture)) {
			glDeleteTextures(1, &_texture);
		}
		_displist = 0;
		_texture = 0;
	}

	void Glyph::fillMetrics (const FT_Face& face)
	{
		_metrics.width = face->glyph->metrics.width / 64;
		_metrics.height = face->glyph->metrics.height / 64;
		_metrics.horiBearingX = face->glyph->metrics.horiBearingX / 64;
		_metrics.horiBearingY = face->glyph->metrics.horiBearingY / 64;
		_metrics.horiAdvance = face->glyph->metrics.horiAdvance / 64;
		_metrics.vertBearingX = face->glyph->metrics.vertBearingX / 64;
		_metrics.vertBearingY = face->glyph->metrics.vertBearingY / 64;
		_metrics.vertAdvance = face->glyph->metrics.vertAdvance / 64;
	}

} // namespace BIL
