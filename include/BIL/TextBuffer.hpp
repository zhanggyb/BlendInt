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

		void Append (const wstring& text);

		void Append (wchar_t charcode);

		void set_rowspacing (float space)
		{
			rowspacing_ = space;
		}

		void set_foreground (const Color& color)
		{
			foreground_ = color;
		}

		void set_background (const Color& color)
		{
			background_ = color;
		}

		void set_origin (const Coord3f& origin)
		{
			origin_ = origin;
		}

		void set_font (const Font& font)
		{
			fontcache_ = FontCache::create(font);
			fontcache_->initialize();
		}

		void Render (void); /* render the text */

		void Clear (void); /* clear the text */

		const wstring& text (void) const
		{
			return text_;
		}

		const FontCache* fontcache (void) const
		{
			return fontcache_;
		}

		/**
		 * @brief Calculate and return the box contains the text
		 * @return (width,height)
		 */
		Vec2ui calculateBox (void);

	private:	// member variables

		// DO not delete this member
		FontCache* fontcache_;

		/** Row spacing */
		float rowspacing_;

		/** Foreground, text color */
		Color foreground_;

		/** Background Color */
		Color background_;

		/** Pen origin */
		Coord3f origin_;

		/** string */
		wstring text_;

	private:	// member functions disabled

		TextBuffer (const TextBuffer& orig);

		TextBuffer& operator = (const TextBuffer& orig);
	};

} /* namespace BIL */
#endif /* TEXTBUFFER_H_ */
