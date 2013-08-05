/*
 * TextBuffer.h
 *
 *  Created on: 2013年7月12日
 *      Author: zhanggyb
 */

#ifndef _BIL_TEXTBUFFER_H_
#define _BIL_TEXTBUFFER_H_

#include <stddef.h>
#include <string>

#include <BIL/Tuple.h>
#include <BIL/FontCache.h>
//#include <BIL/BasicObject.h>

using namespace boost;
using namespace std;

namespace BIL {

	class TextBuffer
	{
	public:

		/**
		 * @brief Default Constructor
		 *
		 * create a new empty text buffer
		 */
		TextBuffer (const Font& font = Font("Sans"));

		/**/
		TextBuffer (const wstring& text, const Font& font = Font("Sans"));

		virtual ~TextBuffer ();

		void append (const wstring& text);

		void append (wchar_t charcode);

		void setRowSpacing (float space);

		void setForeground (const RGBAf& color)
		{
			_fg = color;
		}

		void setBackground (const RGBAf& color)
		{
			_bg = color;
		}

		void setOrigin (const Coord3f& origin);

		void setFont (const Font& font);

		void render (void); /* render the text */

		void clear (void); /* clear the text */

		const wstring& getText (void) const
		{
			return _text;
		}

		const FontCache* getFontCache (void) const
		{
			return _fontcache;
		}

		Vec2ui calculateBox (void);

	private:	// member functions disabled

		TextBuffer (const TextBuffer& orig);

		TextBuffer& operator = (const TextBuffer& orig);

	private:	// member variables

		// DO not delete this member
		FontCache* _fontcache;

		/** Row spacing */
		float _rowspacing;

		/** Foreground, text color */
		RGBAf _fg;

		/** Background Color */
		RGBAf _bg;

		/** Pen origin */
		Coord3f _origin;

		/** string */
		wstring _text;
	};

} /* namespace BIL */
#endif /* TEXTBUFFER_H_ */
