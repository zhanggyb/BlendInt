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

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <cwchar>
#include <iostream>

#include <BlendInt/Gui/TextureGlyph.hpp>

namespace BlendInt {

	unsigned char TextureGlyph::image[HEIGHT][WIDTH];

	TextureGlyph::TextureGlyph()
	: texture_(0)
	{

	}

	TextureGlyph::~TextureGlyph()
	{
		glDeleteTextures(1, &texture_);
	}

	void TextureGlyph::Load (Freetype& freetype, wchar_t charcode)
	{
		if(!freetype.valid()) return;

		if(!texture_) {
			glGenTextures(1, &texture_);
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		/* We require 1 byte alignment when uploading texture data */
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		/* Clamping to edges is important to prevent artifacts when scaling */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		/* Linear filtering usually looks best for text */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (freetype.LoadCharacter(charcode, FT_LOAD_RENDER)) {

			FT_GlyphSlot g = freetype.face()->glyph;

			/* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
			// GL_ALPHA is not valid in GL 3.2 core
			glTexImage2D(
							GL_TEXTURE_2D,
							0,
							GL_R8,
							g->bitmap.width,
							g->bitmap.rows,
							0,
							GL_RED,
							GL_UNSIGNED_BYTE,
							g->bitmap.buffer);

			/* Calculate the vertex and texture coordinates */
			glyph_.bitmap_left = g->bitmap_left;
			glyph_.bitmap_top = g->bitmap_top;
			glyph_.bitmap_width = g->bitmap.width;
			glyph_.bitmap_height = g->bitmap.rows;
			glyph_.advance_x = g->advance.x >> 6;
			glyph_.advance_y = g->advance.y >> 6;

			glyph_.vertexes[0].x = glyph_.bitmap_left;
			glyph_.vertexes[0].y = glyph_.bitmap_top - glyph_.bitmap_height;
			glyph_.vertexes[0].s = 0;
			glyph_.vertexes[0].t = 1;

			glyph_.vertexes[1].x = glyph_.bitmap_left + glyph_.bitmap_width;
			glyph_.vertexes[1].y = glyph_.bitmap_top - glyph_.bitmap_height;
			glyph_.vertexes[1].s = 1;
			glyph_.vertexes[1].t = 1;

			glyph_.vertexes[2].x = glyph_.bitmap_left + glyph_.bitmap_width;
			glyph_.vertexes[2].y = glyph_.bitmap_top;
			glyph_.vertexes[2].s = 1;
			glyph_.vertexes[2].t = 0;

			glyph_.vertexes[3].x = glyph_.bitmap_left;
			glyph_.vertexes[3].y = glyph_.bitmap_top - glyph_.bitmap_height;
			glyph_.vertexes[3].s = 0;
			glyph_.vertexes[3].t = 1;

			glyph_.vertexes[4].x = glyph_.bitmap_left + glyph_.bitmap_width;
			glyph_.vertexes[4].y = glyph_.bitmap_top;
			glyph_.vertexes[4].s = 1;
			glyph_.vertexes[4].t = 0;

			glyph_.vertexes[5].x = glyph_.bitmap_left;
			glyph_.vertexes[5].y = glyph_.bitmap_top;
			glyph_.vertexes[5].s = 0;
			glyph_.vertexes[5].t = 0;

		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

#ifdef DEBUG
	void TextureGlyph::draw_bitmap (FT_Bitmap* bitmap, FT_Int x, FT_Int y)
	{
	    FT_Int i, j, p, q;
	    FT_Int x_max = x + bitmap->width;
	    FT_Int y_max = y + bitmap->rows;

	    for(i = x, p = 0; i < x_max; i++, p++)
	    {
	    	if(i >= WIDTH) break;

	        for(j = y, q = 0; j < y_max; j++, q++)
	        {
	            if (j >= HEIGHT) break;

	            image[j][i] |= bitmap->buffer[q * bitmap->width + p];
	        }
	    }
	}

	void TextureGlyph::show_image (FT_Bitmap* bitmap)
	{
	    int  i, j;

	    int w = std::min (bitmap->width, WIDTH);
	    int h = std::min(bitmap->rows, HEIGHT);

	    for ( i = 0; i < h; i++ )
	    {
	        for ( j = 0; j < w; j++ )
	            putchar( image[i][j] == 0 ? ' '
	                     : image[i][j] < 128 ? '+'
	                     : '*' );
	        putchar( '\n' );
	    }
	}
#endif

	void TextureGlyph::generate(Freetype* freetype, wchar_t charcode)
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

		if (freetype->LoadCharacter(charcode, FT_LOAD_RENDER)) {

			FT_GlyphSlot g = freetype->face()->glyph;

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

		glBindTexture(GL_TEXTURE_2D, 0);
	}

}
