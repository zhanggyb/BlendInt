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

#include <blendint/core/types.hpp>
#include <vector>

#include <blendint/gui/texture-atlas.hpp>

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
		int new_width = 0;
		int new_height = 0;
		bool expand = false;
		int reference_max_height = 0;

		if(bitmap_width > width_) {

			DBG_PRINT_MSG("%s", "big width");

			if(bitmap_rows > height_) {

				DBG_PRINT_MSG("%s", "big height");
				// expand texture both horizontally and vertically
				new_width = width_ + bitmap_width - (width_ - last_x_);
				new_height = height_ + bitmap_rows - (height_ - last_y_);

			} else {

				DBG_PRINT_MSG("%s", "big width only");
				// expand texture horizontally
				new_width = width_ + bitmap_width - (width_ - last_x_);
				new_height = height_;

			}

			expand = true;

		} else if (bitmap_rows > height_) {

			DBG_PRINT_MSG("%s", "big height");
			// expand texture vertically

			new_width = width_;
			new_height = height_ + bitmap_rows - (height_ - last_y_);

			expand = true;

		}

		int x = last_x_;
		int y = last_y_;
		last_row_height_ = std::max(last_row_height_, bitmap_rows);
		reference_max_height = last_row_height_;

		if((x + bitmap_width) > width_) {
			x = 0;
			y = last_y_ + last_row_height_;
			last_row_height_ = bitmap_rows;
		}

		if((y + bitmap_rows) > height_) {
			new_width = width_;
			new_height = height_ + std::max(reference_max_height, bitmap_rows) - (height_ - y);
			expand = true;
		}

		if(expand) {

			// FIXME: the following code is not tested and may fail.

			DBG_PRINT_MSG("expand texture: from (%d, %d) to (%d, %d)", width_, height_, new_width, new_height);

			// Generate pixel buffer object for unpack and copy texture data to it.
			GLuint pbo = 0;
			glGenBuffers(1, &pbo);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
			glBufferData(GL_PIXEL_UNPACK_BUFFER, width_ * height_ * sizeof(GLubyte), 0, GL_STREAM_DRAW);

			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

			GLuint new_tex_id = 0;
			glGenTextures(1, &new_tex_id);
			glBindTexture(GL_TEXTURE_2D, new_tex_id);

			// Clamping to edges is important to prevent artifacts when scaling
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			// Linear filtering usually looks best for text
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// set image size
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, new_width, new_height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

			// Copy pixel buffer data back to new texture
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width_, height_, GL_RED, GL_UNSIGNED_BYTE, 0);

			glDeleteBuffers(1, &pbo);
			glDeleteTextures(1, &id_);

			id_ = new_tex_id;
			width_ = new_width;
			height_ = new_height;

			glBindTexture(GL_TEXTURE_2D, id_);

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
