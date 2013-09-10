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
 * warrantexture_coord_offset_y of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */
#include <GL/glew.h>
#include <cwchar>
#include <iostream>

#include <BIL/TextureFont.hpp>

namespace BIL {

	const char* TextureFont::vs_shader =
			"attribute vec4 coord;"
			"varying vec2 texpos;"
			""
			"void main(void) {"
			"  gl_Position = gl_ModelViewProjectionMatrix * vec4(coord.xy, 0, 1);"
			"  texpos = coord.zw;"
			"}";

	const char* TextureFont::fs_shader =
			"varying vec2 texpos;"
			"uniform sampler2D tex;"
			"uniform vec4 color;"
			""
			"void main(void) {"
			"  gl_FragColor = vec4(1, 1, 1, texture2D(tex, texpos).a) * color;"
			"}";

	TextureFont::TextureFont(wchar_t charcode)
	: charcode_(charcode), texture_(0)
	{

	}

	TextureFont::~TextureFont()
	{
		if(glIsTexture(texture_)) {
			glDeleteTextures(1, &texture_);
		}
	}

	void TextureFont::generate(Freetype* freetype)
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

		if (freetype->loadCharacter(charcode_, FT_LOAD_RENDER)) {

			FT_GlyphSlot g = freetype->getFontFace()->glyph;

			/* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

			/* Calculate the vertex and texture coordinates */
			glyph_metrics_.bitmap_left = g->bitmap_left;
			glyph_metrics_.bitmap_top = g->bitmap_top;
			glyph_metrics_.bitmap_width = g->bitmap.width;
			glyph_metrics_.bitmap_height = g->bitmap.rows;
			glyph_metrics_.advance_x = g->advance.x >> 6;
			glyph_metrics_.advance_y = g->advance.y >> 6;

#ifdef DEBUG
			std::cout << "Metrics: " << std::endl
								<< "		left: " << glyph_metrics_.bitmap_left << std::endl
								<< "		top: " << glyph_metrics_.bitmap_top << std::endl
								<< "		width: " << glyph_metrics_.bitmap_width << std::endl
								<< "		height: " << glyph_metrics_.bitmap_height << std::endl
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
