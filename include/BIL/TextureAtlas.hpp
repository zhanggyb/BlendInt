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

#ifndef _BIL_TEXTUREATLAS_HPP_
#define _BIL_TEXTUREATLAS_HPP_

#include <vector>
#include <BIL/Vector.hpp>
#include <BIL/Freetype.hpp>

namespace BIL {

	/**
	 * A texture atlas is basically a big texture which contains many
	 * small images that are packed together.
	 */
	class TextureAtlas
	{
	public:

		TextureAtlas (const size_t width, const size_t height,
		        const size_t depth);

		~TextureAtlas ();

		void setRegion (const size_t x, const size_t y, const size_t width,
		        const size_t height, const unsigned char * data,
		        const size_t stride);

		Rect getRegion (const size_t width, const size_t height);

		/**
		 * Upload to video memory
		 */
		void upload ();

	private:

		int fit (const size_t index, const size_t width, const size_t height);

		void merge ();

		/**
		 * Texture ID
		 */
		GLuint texture_;		// texture object

		size_t width_;				// width of texture in pixels
		size_t height_;			// height of texture in pixels

		/**
		 * Depth (in bytes) of the underlying texture
		 */
		size_t depth_;

		/**
		 * Atlas data
		 */
		unsigned char * data_;

		std::vector<Vector3i> nodes_;
	};

}

#endif /* _BIL_TEXTUREATLAS_HPP_ */
