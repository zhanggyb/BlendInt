/*
 * TextBuffer.cpp
 *
 *  Created on: 2013年7月12日
 *      Author: zhanggyb
 */

#include <GL/glew.h>
#include <GL/gl.h>

#include <BIL/FontEngine.h>
#include <BIL/FontConfig.h>
#include <BIL/TextBuffer.h>

#include <BIL/Tuple.h>

namespace BIL {

	TextBuffer::TextBuffer (const Font& font)
			: _rowspacing(1.0), _fg(1.0, 1.0, 1.0, 1.0), _bg(0.0, 0.0, 0.0, 0.0)
	{
		setFont(font);
		glShadeModel(GL_FLAT);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	TextBuffer::TextBuffer (const wstring& text, const Font& font)
			: _rowspacing(1.0), _fg(1.0, 1.0, 1.0, 1.0), _bg(0.0, 0.0, 0.0, 0.0)
	{
		setFont(font);
		append(text);

		glShadeModel(GL_FLAT);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	TextBuffer::~TextBuffer ()
	{
	}

	void TextBuffer::append (const wstring& text)
	{
		if (text.empty())
			return;

		_text.append(text);
	}

	void TextBuffer::append (wchar_t charcode)
	{
		_text.push_back(charcode);
	}

	void TextBuffer::setRowSpacing (float space)
	{
		_rowspacing = space;
	}

	void TextBuffer::setOrigin (const Coord3f& origin)
	{
		_origin = origin;
	}

	void TextBuffer::setFont (const Font& font)
	{
		_fontcache = FontCache::create(font);
		_fontcache->initialize();
	}

	/*
	 * See section 4b, namely the function void compute_string_bbox( FT_BBox *abbox )
	 *
	 * It basically adds up the bounding boxes of each character.
	 *
	 * Effectively, the algorithm looks like:
	 * 0) M <- invalid bounding box
	 * 1) For each glyph G
	 * 1.1)     Get kerning, advance pen
	 * 1.2)     Get the bounding box as B
	 * 1.3)     Add the pen position to B
	 * 1.4)     Check if B is larger than the current bounding box M, if so, update it
	 * 1.5)     Advance pen by G.advance
	 * 2) Check if box is valid, correct if necessary
	 */
	Vec2ui TextBuffer::calculateBox (void)
	{
		wstring::const_iterator it;
		Vec2ui box;
		Glyph* glyph = NULL;

		unsigned int line_width = 0;
		unsigned int line = 1;

		for (it = _text.begin(); it != _text.end(); it++) {
			if (*it == '\n') {
				line++;
				box.vec.x = box.vec.x > line_width ? box.vec.x : line_width;
			}

			glyph = _fontcache->query(*it);
			if (glyph != NULL) {
				line_width = glyph->getBox().vec.x + line_width;
			}
		}

		box.vec.x = box.vec.x > line_width ? box.vec.x : line_width;
		box.vec.y = _fontcache->getHeight() * line + (line - 1) * _rowspacing;

		return box;
	}

	void TextBuffer::render (void)
	{
		Glyph* glyph = NULL;

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor4f(_fg.rgba.r, _fg.rgba.g, _fg.rgba.b, _fg.rgba.a);

		//glDisable(GL_LIGHTING);
		//glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glMatrixMode(GL_MODELVIEW);

		//glLoadIdentity();
		glPushMatrix();

		glTranslatef(_origin.coord.x, _origin.coord.y, _origin.coord.z);

		int line = 0;
		wstring::const_iterator it;
		for (it = _text.begin(); it != _text.end(); it++) {
			if (*it == '\n') {
				line++;
				glLoadIdentity();
				glTranslatef(_origin.coord.x,
				        _origin.coord.y
				                - _rowspacing * _fontcache->getHeight() * line,
				        _origin.coord.z);
				continue;
			}

			glyph = _fontcache->query(*it);
			if (glyph != NULL) {
				glyph->render();
				glTranslatef(glyph->getMetrics().horiAdvance, 0, 0);
			}
		}

		glPopMatrix();
	}

	void TextBuffer::clear (void)
	{
		_text.clear();
	}

} /* namespace BIL */
