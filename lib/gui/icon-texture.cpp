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

#include <vector>

#include <blendint/core/types.hpp>
#include <blendint/opengl/opengl.hpp>
#include <blendint/gui/icon-texture.hpp>

namespace BlendInt {

	IconTexture::IconTexture ()
	: GLTexture2D(),
	  cell_width_(0),
	  cell_height_(0),
	  offset_x_(0),
	  offset_y_(0),
	  space_x_(0),
	  space_y_(0)
	{
	}

	IconTexture::~IconTexture ()
	{
	}

	void IconTexture::Generate (int width, int height, short cell_x,
					short cell_y, short xoffset, short yoffset, short xspace,
					short yspace)
	{
		GLTexture2D::generate();

		glBindTexture(GL_TEXTURE_2D, id());

#ifdef DEBUG
#ifdef __APPLE__
		// The Texture showed in testTextureAtlas is not clear in Mac OS, try to initialize this
		// TODO: these 2 lines just for test, remove later
		std::vector<unsigned char> blank(width * height, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, &blank[0]);
#else	// __APPLE__
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
#endif
#else	// DEBUG
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
#endif

		// Clamping to edges is important to prevent artifacts when scaling
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Linear filtering usually looks best for text
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		cell_width_ = cell_x;
		cell_height_ = cell_y;
		space_x_ = xspace;
		space_y_ = yspace;
		offset_x_ = xoffset;
		offset_y_ = yoffset;
	}

	bool IconTexture::SetSubImage (int index, int bitmap_width, int bitmap_rows, const unsigned char* bitmap_buf, int* r_x, int* r_y, bool clear)
	{
		if(bitmap_width > cell_width_ || bitmap_rows > cell_height_) {
			return false;
		}

		GLint tex_width = 0;
		GLint tex_height = 0;

		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_WIDTH,
						&tex_width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_HEIGHT,
						&tex_height);

		int x = 0, y = 0;

		int columns = (tex_width - offset_x_) / (cell_width_ + space_x_);
		int rows = (tex_height - offset_y_) / (cell_height_ + space_y_);

		if(index > (rows * columns - 1)) return false;

		x = index % columns;
		y = index / columns;

		x = offset_x_ + (x * (cell_width_ + space_x_));
		y = offset_y_ + (y * (cell_height_ + space_y_));

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		if(clear) {
			std::vector<unsigned char> blank(cell_width_ * cell_height_, 0);
			glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, cell_width_, cell_height_, GL_RED, GL_UNSIGNED_BYTE, &blank[0]);
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, bitmap_width, bitmap_rows, GL_RED, GL_UNSIGNED_BYTE, bitmap_buf);

		if(r_x) *r_x = x;
		if(r_y) *r_y = y;

		return true;
	}

	bool IconTexture::SetSubImage (int column_index, int row_index, int bitmap_width, int bitmap_rows, const unsigned char* bitmap_buf, int* r_x, int* r_y, bool clear)
	{
		if(bitmap_width > cell_width_ || bitmap_rows > cell_height_) {
			return false;
		}

		GLint tex_width = 0;
		GLint tex_height = 0;

		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_WIDTH,
						&tex_width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_HEIGHT,
						&tex_height);

		int columns = (tex_width - offset_x_) / (cell_width_ + space_x_);
		int rows = (tex_height - offset_y_) / (cell_height_ + space_y_);

		if(column_index > (columns - 1)) return false;
		if(row_index > (rows - 1)) return false;

		int x = 0, y = 0;
		x = offset_x_ + (column_index * (cell_width_ + space_x_));
		y = offset_y_ + (row_index * (cell_height_ + space_y_));

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		if(clear) {
			std::vector<unsigned char> blank(cell_width_ * cell_height_, 0);
			glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, cell_width_, cell_height_, GL_RED, GL_UNSIGNED_BYTE, &blank[0]);
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, bitmap_width, bitmap_rows, GL_RED, GL_UNSIGNED_BYTE, bitmap_buf);

		if(r_x) *r_x = x;
		if(r_y) *r_y = y;

		return true;
	}

	int IconTexture::GetColumns () const
	{
		GLint tex_width = 0;

		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_WIDTH,
						&tex_width);

		int columns = (tex_width - offset_x_) / (cell_width_ + space_x_);

		return columns;
	}

	int IconTexture::GetRows () const
	{
		GLint tex_height = 0;

		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_HEIGHT,
						&tex_height);

		int rows = (tex_height - offset_y_) / (cell_height_ + space_y_);

		return rows;
	}

	int IconTexture::GetMaxNumber() const
	{
		int num = 0;

		GLint tex_width = 0;
		GLint tex_height = 0;

		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_WIDTH,
						&tex_width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_HEIGHT,
						&tex_height);

		int h = (tex_width - offset_x_) / (cell_width_ + space_x_);
		int v = (tex_height - offset_y_) / (cell_height_ + space_y_);

		num = h * v;

		return num;
	}


}
