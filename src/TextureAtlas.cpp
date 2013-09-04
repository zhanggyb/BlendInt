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

#include <GL/glew.h>

#include <string.h>
#include <algorithm>
#include <assert.h>

#include <BIL/TextureAtlas.hpp>
#include <BIL/Freetype.hpp>

// Maximum texture width
#define MAXWIDTH 1024

namespace BIL {

	TextureAtlas::TextureAtlas (const std::string& filename)
			: texture_(0), width_(0), height_(0), filename_(filename)
	{
		 memset(c_, 0, sizeof c_);
	}

	TextureAtlas::~TextureAtlas ()
	{
		if(glIsTexture(texture_)) {
			glDeleteTextures(1, &texture_);
		}
	}

	void TextureAtlas::generate ()
	{
		Freetype freetype;
		freetype.open(filename_, 9, 96);
		if(!freetype.valid()) return;

		freetype.setPixelSize(0, 12);

		FT_GlyphSlot g = freetype.getFontFace()->glyph;

		int roww = 0;
		int rowh = 0;
		 width_ = 0;
		 height_ = 0;


		/* Find minimum size for a texture holding all visible ASCII characters */
		for (int i = 32; i < 128; i++) {
			if (freetype.loadCharacter(i, FT_LOAD_RENDER)) {
				fprintf(stderr, "Loading character %c failed!\n", i);
				continue;
			}
			if (roww + g->bitmap.width + 1 >= MAXWIDTH) {
				width_ = std::max(width_, static_cast<unsigned int>(roww));
				height_ += rowh;
				roww = 0;
				rowh = 0;
			}
			roww += g->bitmap.width + 1;
			rowh = std::max(rowh, g->bitmap.rows);
		}

		width_ = std::max(width_, static_cast<unsigned int>(roww));
		height_ += rowh;

		/* Create a texture that will be used to hold all ASCII glyphs */
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &texture_);
		glBindTexture(GL_TEXTURE_2D, texture_);
		//glUniform1i(uniform_tex, 0);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width_, height_, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

		/* We require 1 byte alignment when uploading texture data */
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		/* Clamping to edges is important to prevent artifacts when scaling */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		/* Linear filtering usually looks best for text */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		/* Paste all glyph bitmaps into the texture, remembering the offset */
		int ox = 0;
		int oy = 0;

		rowh = 0;

		for (int i = 32; i < 128; i++) {
			if (freetype.loadCharacter(i, FT_LOAD_RENDER)) {
				fprintf(stderr, "Loading character %c failed!\n", i);
				continue;
			}

			if (ox + g->bitmap.width + 1 >= MAXWIDTH) {
				oy += rowh;
				rowh = 0;
				ox = 0;
			}

			glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
			c_[i].advance_x = g->advance.x >> 6;
			c_[i].advance_y = g->advance.y >> 6;

			c_[i].bitmap_width = g->bitmap.width;
			c_[i].bitmap_height = g->bitmap.rows;

			c_[i].bitmap_left = g->bitmap_left;
			c_[i].bitmap_right = g->bitmap_top;

			c_[i].texture_coord_offset_x = ox / (float)width_;
			c_[i].texture_coord_offset_y = oy / (float)height_;

			rowh = std::max(rowh, g->bitmap.rows);
			ox += g->bitmap.width + 1;
		}

		fprintf(stderr, "Generated a %u x %u (%u kb) texture atlas\n", width_, height_, width_ * height_ / 1024);
	}
}
