/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_IMAGE_HPP_
#define _BLENDINT_IMAGE_HPP_

#include <BlendInt/String.hpp>
#include <vector>

namespace BlendInt {

	/**
	 * @brief Image class focused on I/O and direct pixel access and manipulation
	 */
	class Image
	{
	public:

		Image ();
		~Image ();

		bool read (const String& filename);

		bool save ();

		void close ();

		const unsigned char* pixels () const {return &m_pixels[0];}

	private:

		std::vector<unsigned char> m_pixels;

		int m_width;
		int m_height;
		int m_channels;
	};
}

#endif /* IMAGE_HPP_ */
