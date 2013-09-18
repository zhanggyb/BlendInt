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

#ifndef _BIL_IMAGE_HPP_
#define _BIL_IMAGE_HPP_

#include <BILO/String.hpp>
#include <vector>

namespace BILO {

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

		int width_;
		int height_;
		int channels_;
	};
}

#endif /* IMAGE_HPP_ */
