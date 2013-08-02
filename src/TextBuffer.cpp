/*
 * TextBuffer.cpp
 *
 *  Created on: 2013年7月12日
 *      Author: zhanggyb
 */

#include <BIL/FontEngine.h>
#include <BIL/FontConfig.h>
#include <BIL/TextBuffer.h>

#include <BIL/Tuple.h>

namespace BIL {

	TextBuffer::TextBuffer ()
			: _rowspacing(1.0), _color(1.0, 1.0, 1.0, 1.0),
			  _background(0.0, 0.0, 0.0, 0.0)
	{
		_fontcache = FontCache::create();
		_fontcache->initialize();

		glShadeModel(GL_FLAT);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	TextBuffer::TextBuffer (const wstring& text)
			: _rowspacing(1.0), _color(1.0, 1.0, 1.0, 1.0),
			  _background(0.0, 0.0, 0.0, 0.0)
	{
		_fontcache = FontCache::create();
		_fontcache->initialize();

		append(text);

		glShadeModel(GL_FLAT);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	TextBuffer::~TextBuffer ()
	{
		FontCache::release();
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

	void TextBuffer::render (void)
	{
		Glyph* glyph = NULL;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor4f (_color.rgba.r,
				_color.rgba.g,
				_color.rgba.b,
				_color.rgba.a);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		for(size_t i = 0; i < _text.length(); i++)
		{
			glyph = _fontcache->query(_text[i]);
			if(glyph != NULL) {
				glyph->render();
				glTranslatef(glyph->getMetrics().horiAdvance, 0, 0);
			}
		}
	}

	void TextBuffer::renderAt (const Coord3f& pos)
	{
		Glyph* glyph = NULL;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor4f (_color.rgba.r,
				_color.rgba.g,
				_color.rgba.b,
				_color.rgba.a);

		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(pos.coord.x, pos.coord.y, pos.coord.z);

		int line = 0;
		wstring::const_iterator it;
		for(it = _text.begin(); it != _text.end(); it++)
		{
			if (*it == '\n') {
				line++;
				glLoadIdentity();
				glTranslatef(pos.coord.x, pos.coord.y -
							 _rowspacing * _fontcache->getHeight() * line,
							 pos.coord.z);
				continue;
			}

			glyph = _fontcache->query(*it);
			if(glyph != NULL) {
				glyph->render();
				glTranslatef(glyph->getMetrics().horiAdvance, 0, 0);
			}
		}
	}

	void TextBuffer::clear (void)
	{
		_text.clear();
	}

} /* namespace BIL */
