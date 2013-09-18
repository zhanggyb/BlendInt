/*
 * This file is part of BILO (Blender Interface Library).
 *
 * BILO (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_TEXTBUFFER_H_
#define _BIL_TEXTBUFFER_H_

#include <list>

#include <BILO/Tuple.hpp>
#include <BILO/FontCache.hpp>
#include <BILO/Color.hpp>
#include <BILO/Coord.hpp>
#include <BILO/Size.hpp>
#include <BILO/String.hpp>
#include <BILO/Rect.hpp>

using namespace std;

namespace BILO {

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

		/**
		 * @brief append text to the current line
		 * @param text
		 */
		void append (const String& text);

		/**
		 * @brief append character to the current line
		 * @param charcode
		 */
		void append (wchar_t charcode);

		void appendParagraph (const String& text);

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

		void setFont (const Font& font);

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

		Rect calculateOutline ();

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

		std::list<String> paragraphs_;

	private:	// member functions disabled

		TextBuffer (const TextBuffer& orig);

		TextBuffer& operator = (const TextBuffer& orig);
	};

} /* namespace BILO */
#endif /* TEXTBUFFER_H_ */
