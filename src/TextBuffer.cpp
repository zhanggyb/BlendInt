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
			: _rowspacing(1.0)
	{
	}

	TextBuffer::TextBuffer (const wstring& text)
			: _rowspacing(1.0)
	{
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

	void TextBuffer::render (void)
	{
	}

	void TextBuffer::renderAt (const Coord3f& pos)
	{
	}

	void TextBuffer::clear (void)
	{

	}

} /* namespace BIL */
