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
 * warrantexture_coord_offset_y of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */
#include <GL/glew.h>
#include <cwchar>
#include <iostream>

#include <BILO/TextureFont.hpp>

namespace BILO {

	TextureFont::TextureFont()
	: texture_(0)
	{

	}

	TextureFont::~TextureFont()
	{
		if(glIsTexture(texture_)) {
			glDeleteTextures(1, &texture_);
		}
	}

	void TextureFont::generate(Freetype* freetype, wchar_t charcode)
	{
		if(!freetype || !freetype->valid()) return;

		if(glIsTexture(texture_)) {
			glDeleteTextures(1, &texture_);
		}

		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &texture_);
		glBindTexture(GL_TEXTURE_2D, texture_);

		/* We require 1 byte alignment when uploading texture data */
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		/* Clamping to edges is important to prevent artifacts when scaling */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		/* Linear filtering usually looks best for text */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (freetype->loadCharacter(charcode, FT_LOAD_RENDER)) {

			FT_GlyphSlot g = freetype->getFontFace()->glyph;

			/* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

			/* Calculate the vertex and texture coordinates */
			glyph_.bitmap_left = g->bitmap_left;
			glyph_.bitmap_top = g->bitmap_top;
			glyph_.bitmap_width = g->bitmap.width;
			glyph_.bitmap_height = g->bitmap.rows;
			glyph_.advance_x = g->advance.x >> 6;
			glyph_.advance_y = g->advance.y >> 6;

#ifdef DEBUG
			std::cout << "Metrics: " << std::endl
								<< "		left: " << glyph_.bitmap_left << std::endl
								<< "		top: " << glyph_.bitmap_top << std::endl
								<< "		width: " << glyph_.bitmap_width << std::endl
								<< "		height: " << glyph_.bitmap_height << std::endl
								<< "and Glyph Metrics: " << std::endl
								<< "		width: " << (g->metrics.width >> 6) << std::endl
								<< "		height: " << (g->metrics.height >> 6) << std::endl
								<< "		horiBearingX: " << (g->metrics.horiBearingX >> 6) << std::endl
								<< "		horiBearingY: " << (g->metrics.horiBearingY >> 6) << std::endl
								<< "		horiAdvance: " << (g->metrics.horiAdvance >> 6) << std::endl
								<< std::endl;
#endif
		}
	}

}
