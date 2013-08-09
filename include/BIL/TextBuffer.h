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

		/**
		 * @brief Calculate and return the box contains the text
		 * @return (width,height)
		 */
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
