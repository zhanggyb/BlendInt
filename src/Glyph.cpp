/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>
#include <GL/gl.h>

#include <string>
#include <string.h>

#include <BIL/Glyph.hpp>
#include <BIL/FontConfig.hpp>
#include <BIL/FontEngine.hpp>


namespace BIL {

	Glyph::Glyph (wchar_t charcode, FontEngine* fontlib)
			: font_engine_(fontlib), charcode_(0), glyph_index_(0), texture_(0), displist_(
			        0), dpi_(96)
	{
		memset(&metrics_, 0, sizeof(Metrics));

		MakeDisplayList();
	}

	Glyph::Glyph (wchar_t charcode, const Font& font, unsigned int dpi,
	        FontEngine* fontlib)
			: font_engine_(fontlib), charcode_(charcode), glyph_index_(0), texture_(0), displist_(
			        0), font_(font), dpi_(dpi)
	{
		memset(&metrics_, 0, sizeof(Metrics));

		MakeDisplayList();
	}

	Glyph::Glyph (const Glyph& orig)
			: font_engine_(NULL), charcode_(0), glyph_index_(0), texture_(0), displist_(
			        0), dpi_(96)
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
		ResetGL();
	}

	void Glyph::set_charcode (wchar_t charcode)
	{
		// if we already create texture and display list for charcode
		if (charcode_ == charcode) {
			return;
		}

		ResetGL();
		charcode_ = charcode;

		MakeDisplayList();
	}

	void Glyph::set_font_engine (FontEngine* fontlib)
	{
		if (font_engine_ == fontlib) {
			return;
		}

		ResetGL ();
		font_engine_ = fontlib;

		MakeDisplayList();
	}

	void Glyph::Render ()
	{
		if (!glIsTexture(texture_))
			return;
		if (!glIsList(displist_))
			return;

		glEnable(GL_TEXTURE_2D);

		glPushMatrix();

		glMatrixMode(GL_MODELVIEW);

		glTranslatef((float) metrics_.horiBearingX,
		        (float) metrics_.horiBearingY - (float) metrics_.height, 0);

		glBindTexture(GL_TEXTURE_2D, texture_);
		glCallLists(1, GL_UNSIGNED_BYTE, &displist_);

		glPopMatrix();

		glDisable(GL_TEXTURE_2D);
	}

	Rect Glyph::OutlineBox ()
	{
		if (!font_engine_) return Rect();

		FT_Face face = font_engine_->getFontFace();

		FT_BBox outline_box;
		FT_Outline_Get_CBox (&(face->glyph->outline), &outline_box);
		outline_box.xMin = outline_box.xMin / 64;
		outline_box.xMax = outline_box.xMax / 64;
		outline_box.yMin = outline_box.yMin / 64;
		outline_box.yMax = outline_box.yMax / 64;

		return Rect (Point(outline_box.xMin, outline_box.yMin),
				Point(outline_box.xMax, outline_box.yMax));
	}

	bool Glyph::MakeDisplayList (void)
	{
		FontEngine* fontlib = NULL;

		// if _fonttype is not set, use default font
		if (font_engine_ == NULL) {
			FontConfig* fontserv = FontConfig::getService();

			if (fontserv == NULL) {
				return false;
			}

			if (!fontserv->isInitialized()) {
				return false;
			}
			font_engine_ = new FontEngine(fontserv->getBuffer(),
			        fontserv->getBufferSize());
		}

		fontlib = font_engine_;

		if (!fontlib->valid()) {
			cerr << "Cannot get Font" << endl;
			return false;
		}

		fontlib->setCharSize(font_.size, dpi_);
		glyph_index_ = fontlib->getCharIndex(charcode_);
		if (glyph_index_ == 0) {
			// TODO: if the character code is not supported in the font
			// file, print a special symbol
			return false;
		}

		//bool result = font.loadCharacter(_charcode, FT_LOAD_RENDER);
		bool result = fontlib->loadGlyph(glyph_index_);
		if (!result)
			return false;

		FT_Face face = fontlib->getFontFace();

		FillMetrics(face);

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
			if (font_engine_ == NULL)
				delete fontlib;
			return false;
		}

		FT_BBox acbox;
		FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_UNSCALED, &acbox);

		cbox_.vec.x = (acbox.xMax - acbox.xMin) >> 6;
		cbox_.vec.y = (acbox.yMax - acbox.yMin) >> 6;

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

		glGenTextures(1, &texture_);
		glBindTexture( GL_TEXTURE_2D, texture_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// GL_LUMINANCE_ALPHA to use 2 channel data.
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, rows, 0,
		GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, fontimage);

		delete[] fontimage;

		displist_ = glGenLists(1);

		glNewList(displist_, GL_COMPILE);

		glBindTexture(GL_TEXTURE_2D, texture_);

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

		if (font_engine_ == NULL) {
			delete fontlib;
		}

		return true;
	}

	void Glyph::ResetGL (void)
	{
		if (glIsList(displist_)) {
			glDeleteLists(displist_, 1);
		}
		if (glIsTexture(texture_)) {
			glDeleteTextures(1, &texture_);
		}
		displist_ = 0;
		texture_ = 0;
	}

	void Glyph::FillMetrics (const FT_Face& face)
	{
		metrics_.width = face->glyph->metrics.width / 64;
		metrics_.height = face->glyph->metrics.height / 64;
		metrics_.horiBearingX = face->glyph->metrics.horiBearingX / 64;
		metrics_.horiBearingY = face->glyph->metrics.horiBearingY / 64;
		metrics_.horiAdvance = face->glyph->metrics.horiAdvance / 64;
		metrics_.vertBearingX = face->glyph->metrics.vertBearingX / 64;
		metrics_.vertBearingY = face->glyph->metrics.vertBearingY / 64;
		metrics_.vertAdvance = face->glyph->metrics.vertAdvance / 64;
	}

} // namespace BIL
