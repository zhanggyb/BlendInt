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

#include <BIL/Tuple.hpp>
#include <BIL/FontCache.hpp>
#include <BIL/Color.hpp>
#include <BIL/Coord.hpp>
#include <BIL/Size.hpp>
#include <BIL/String.hpp>

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
		// TextBuffer (const Font& font = Font("Sans"));

		/**/
		TextBuffer (const String& text = String(), const Font& font = Font("Sans"));

		virtual ~TextBuffer ();

		void append (const String& text);

		void append (wchar_t charcode);

		void setLineSpacing (float space)
		{
			line_spacing_ = space;
		}

		void setForeground (const Color& color)
		{
			foreground_ = color;
		}

		void setBackground (const Color& color)
		{
			background_ = color;
		}

		void setOrigin (const Coord3f& origin)
		{
			origin_ = origin;
		}

		void setFont (const Font& font)
		{
			fontcache_ = FontCache::create(font);
			fontcache_->setup();
		}

		void render (); /* render the text */

		void clear (); /* clear the text */

		const String& text () const
		{
			return text_;
		}

		void setText (const String& text)
		{
			text_ = text;
		}

		const FontCache* fontcache () const
		{
			return fontcache_;
		}

		/**
		 * @brief Calculate and return the box contains the text
		 * @return (width,height)
		 */
		Size calculateOutlineBoxSize ();

	private:	// member variables

		// DO not delete this member
		FontCache* fontcache_;

		/** Row spacing */
		float line_spacing_;

		/** Foreground, text color */
		Color foreground_;

		/** Background Color */
		Color background_;

		/** Pen origin */
		Coord3f origin_;

		/** string */
		String text_;

	private:	// member functions disabled

		TextBuffer (const TextBuffer& orig);

		TextBuffer& operator = (const TextBuffer& orig);
	};

} /* namespace BIL */
#endif /* TEXTBUFFER_H_ */
