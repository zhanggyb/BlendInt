/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_CORE_IMAGE_HPP_
#define _BLENDINT_CORE_IMAGE_HPP_

#include <BlendInt/Core/Object.hpp>
#include <BlendInt/Core/String.hpp>
#include <vector>

namespace BlendInt {

	/**
	 * @brief Image class focused on I/O and direct pixel access and manipulation
	 */
	class Image: public Object
	{
	public:

		Image ();

		~Image ();

		bool Read (const char* filename);

		bool Read (const String& filename);

		bool Save ();

		void Close ();

		const unsigned char* pixels () const {return &m_pixels[0];}
		
		int height () const
		{
			return m_height;
		}
		
		int width () const
		{
			return m_width;
		}

		int channels () const
		{
			return m_channels;
		}

	private:

		String m_filename;

		int m_width;
		int m_height;
		int m_channels;

		std::vector<unsigned char> m_pixels;
	};
}

#endif /* IMAGE_HPP_ */
