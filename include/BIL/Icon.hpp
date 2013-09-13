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

#ifndef _BIL_ICON_HPP_
#define _BIL_ICON_HPP_

#include <BIL/String.hpp>
#include <BIL/Rect.hpp>

namespace BIL {

	class Icon
	{
	public:

		Icon ();

		Icon (const unsigned char* icon, unsigned int width, unsigned int height);

		/**
		 * @brief create icon from allocated memory
		 * @param icon
		 * @return
		 */
		bool create (const unsigned char* icon, unsigned int width, unsigned int height);

		/**
		 * @brief create icon from a icon file
		 * @param filename
		 * @return
		 */
		bool create (const String& filename);

		int width() const {return width;}

		int height() const {return height;}

		const unsigned char* data () const {return data_;}

		void scale (float ratio);

		void scale (unsigned int width, unsigned height);

		/**
		 * @brief draw icon at position x,y with the default size
		 * @param x
		 * @param y
		 */
		void draw (int x, int y);

		void draw (int x, int y, unsigned width, unsigned height);

		void draw (const Rect& rect);

	private:

		unsigned char* data_;

		int width_;

		int height_;
	};
}

#endif /* _BIL_ICON_HPP_ */
