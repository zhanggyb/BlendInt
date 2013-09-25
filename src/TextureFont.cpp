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

			glyph_.vertexes[0].x = glyph_.bitmap_left;
			glyph_.vertexes[0].y = glyph_.bitmap_top;
			glyph_.vertexes[0].s = 0;
			glyph_.vertexes[0].t = 0;

			glyph_.vertexes[1].x = glyph_.bitmap_left + glyph_.bitmap_width;
			glyph_.vertexes[1].y = glyph_.bitmap_top;
			glyph_.vertexes[1].s = 1;
			glyph_.vertexes[1].t = 0;

			glyph_.vertexes[2].x = glyph_.bitmap_left;
			glyph_.vertexes[2].y = glyph_.bitmap_top - glyph_.bitmap_height;
			glyph_.vertexes[2].s = 0;
			glyph_.vertexes[2].t = 1;

			glyph_.vertexes[3].x = glyph_.bitmap_left + glyph_.bitmap_width;
			glyph_.vertexes[3].y = glyph_.bitmap_top;
			glyph_.vertexes[3].s = 1;
			glyph_.vertexes[3].t = 0;

			glyph_.vertexes[4].x = glyph_.bitmap_left;
			glyph_.vertexes[4].y = glyph_.bitmap_top - glyph_.bitmap_height;
			glyph_.vertexes[4].s = 0;
			glyph_.vertexes[4].t = 1;

			glyph_.vertexes[5].x = glyph_.bitmap_left + glyph_.bitmap_width;
			glyph_.vertexes[5].y = glyph_.bitmap_top - glyph_.bitmap_height;
			glyph_.vertexes[5].s = 1;
			glyph_.vertexes[5].t = 1;

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
								<< "Vertexes: " << std::endl
								<< "		vertex[0].x: " << glyph_.vertexes[0].x << std::endl
								<< "		vertex[0].y: " << glyph_.vertexes[0].y << std::endl
								<< "		vertex[0].s: " << glyph_.vertexes[0].s << std::endl
								<< "		vertex[0].t: " << glyph_.vertexes[0].t << std::endl
								<< "		vertex[1].x: " << glyph_.vertexes[1].x << std::endl
								<< "		vertex[1].y: " << glyph_.vertexes[1].y << std::endl
								<< "		vertex[1].s: " << glyph_.vertexes[1].s << std::endl
								<< "		vertex[1].t: " << glyph_.vertexes[1].t << std::endl
								<< "		vertex[2].x: " << glyph_.vertexes[2].x << std::endl
								<< "		vertex[2].y: " << glyph_.vertexes[2].y << std::endl
								<< "		vertex[2].s: " << glyph_.vertexes[2].s << std::endl
								<< "		vertex[2].t: " << glyph_.vertexes[2].t << std::endl
								<< "		vertex[3].x: " << glyph_.vertexes[3].x << std::endl
								<< "		vertex[3].y: " << glyph_.vertexes[3].y << std::endl
								<< "		vertex[3].s: " << glyph_.vertexes[3].s << std::endl
								<< "		vertex[3].t: " << glyph_.vertexes[3].t << std::endl
								<< "		vertex[4].x: " << glyph_.vertexes[4].x << std::endl
								<< "		vertex[4].y: " << glyph_.vertexes[4].y << std::endl
								<< "		vertex[4].s: " << glyph_.vertexes[4].s << std::endl
								<< "		vertex[4].t: " << glyph_.vertexes[4].t << std::endl
								<< "		vertex[5].x: " << glyph_.vertexes[5].x << std::endl
								<< "		vertex[5].y: " << glyph_.vertexes[5].y << std::endl
								<< "		vertex[5].s: " << glyph_.vertexes[5].s << std::endl
								<< "		vertex[5].t: " << glyph_.vertexes[5].t << std::endl
								<< std::endl;
#endif
		}
	}

}
