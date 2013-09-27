/*
 * This file is part of BILO (Blender-like Interface Library in
 * OpenGL).
 *
 * BILO (Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */


#ifndef _BILO_PIXELICON_HPP_
#define _BILO_PIXELICON_HPP_

#include <vector>

namespace BILO {

	/**
	 * Icon displayed with pixels
	 */
	class PixelIcon
	{
	public:

		PixelIcon ();

		PixelIcon (const unsigned char* pixels, int width, int height);

		PixelIcon (const PixelIcon& orig);

		~PixelIcon ();

		void set_pixels (const unsigned char* pixels);

		const unsigned char* pixels () const {return &m_pixels[0];}

		int width () const {return m_width;}

		int height () const {return m_height;}

		void scale (float ratio);

		void scale (int width, int height);

		void display ();

		void display (float x, float y);

	private:

		std::vector<unsigned char> m_pixels;

		int m_width;
		int m_height;

		static const int default_icon_size = 16;	// 16 x 16

		// disabled
		PixelIcon& operator = (const PixelIcon& orig);
	};
}

#endif /* _BIL_ICON_HPP_ */
