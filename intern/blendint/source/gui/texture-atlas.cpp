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

#include <core/types.hpp>
#include <vector>

#include <gui/texture-atlas.hpp>

namespace BlendInt {

	void TextureAtlas::Generate(GLsizei width, GLsizei height)
	{
		if(id_) clear ();

		width_ = width;
		height_ = height;

		glGenTextures(1, &id_);
		glBindTexture(GL_TEXTURE_2D, id_);

#ifdef __APPLE__
		// The Texture showed in testTextureAtlas is not clear in Mac OS, try to initialize this
		// TODO: these 2 lines just for test, remove later
		std::vector<unsigned char> blank(width * height, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, &blank[0]);
#else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
#endif

		// Clamping to edges is important to prevent artifacts when scaling
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Linear filtering usually looks best for text
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	bool TextureAtlas::Upload (int bitmap_width,
			int bitmap_rows,
			const unsigned char* bitmap, int* ox, int* oy)
	{
		// check the texture size
		if(bitmap_width > width_) {

			if(bitmap_rows > height_) {
				// TODO: expand texture both horizontally and vertically
			} else {
				// TODO: expand texture horizontally
			}

		} else if (bitmap_rows > height_) {
			// TODO: expand texture vertically
		}

		int x = last_x_;
		int y = last_y_;
		last_row_height_ = std::max(last_row_height_, bitmap_rows);

		if((x + bitmap_width) > width_) {
			x = 0;
			y = last_y_ + last_row_height_;
			last_row_height_ = bitmap_rows;
		}

		if((y + bitmap_rows) > height_) {
			// TODO: expand texture size
		}

		glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
		glTexSubImage2D(GL_TEXTURE_2D,
						0,	// level
						x,
						y,
						bitmap_width,
						bitmap_rows,
						GL_RED,	// format
						GL_UNSIGNED_BYTE,
						bitmap);

		if(ox) *ox = x;
		if(oy) *oy = y;

		last_x_ = x + bitmap_width;
		last_y_ = y;
		
		return true;
	}
	
}
