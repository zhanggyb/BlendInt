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
		TextBuffer ();

		/**/
		TextBuffer (const wstring& text);

		void append (const wstring& text);

		void append (wchar_t charcode);

		void setRowSpacing (int space);

		void render (void); /* render the text */

		void renderAt (const Coord3f& pos);

		void clear (void); /* clear the text */

		virtual ~TextBuffer ();

	private:	// member functions disabled

		TextBuffer (const TextBuffer& orig);

		TextBuffer& operator = (const TextBuffer& orig);

	private:	// member variables

		FontCache* _fontcache;

		/** Row spacing */
		float _rowspacing;

		/** Base color for text */
		RGBAf _color;

		/** Background Color */
		RGBAf _background;

		/** Pen origin */
		Coord3f _origin;

		/** string */
		wstring _text;
	};

} /* namespace BIL */
#endif /* TEXTBUFFER_H_ */
