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

#ifndef _BIL_ICON_HPP_
#define _BIL_ICON_HPP_

#include <vector>

#include <BILO/Widget.hpp>
#include <BILO/String.hpp>
#include <BILO/Rect.hpp>

#include <BILO/Image.hpp>

namespace BILO {

	class Icon: public Widget
	{
	public:

		Icon (Drawable* parent = 0);

		Icon (const unsigned char* icon, unsigned int width, unsigned int height, Drawable* parent = 0);

		virtual ~Icon ();

		/**
		 * @brief read icon from allocated memory
		 * @param icon
		 * @return
		 */
		bool read (const unsigned char* icon, unsigned int width, unsigned int height);

		/**
		 * @brief create icon from a icon file
		 * @param filename
		 * @return
		 */
		bool read (const String& filename);

		void scale (float ratio);

		void scale (unsigned int width, unsigned height);

	protected:

		virtual void render ();

	private:

		std::vector<unsigned char> m_pixels;

		static const int default_icon_size = 16;	// 16 x 16

	};
}

#endif /* _BIL_ICON_HPP_ */
