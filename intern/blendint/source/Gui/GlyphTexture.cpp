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
#include <algorithm>

#include <BlendInt/Gui/GlyphTexture.hpp>

namespace BlendInt {

#ifdef DEBUG
	unsigned char GlyphTexture::image[HEIGHT][WIDTH];
#endif

	GlyphTexture::GlyphTexture()
	: m_texture(0)
	{

	}

	GlyphTexture::~GlyphTexture()
	{
		glDeleteTextures(1, &m_texture);
	}

	void GlyphTexture::Load (const FTFace& font_face, uint32_t charcode)
	{
		if(!m_texture) {
			glGenTextures(1, &m_texture);
		}

		glBindTexture(GL_TEXTURE_2D, m_texture);

		/* We require 1 byte alignment when uploading texture data */
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		/* Clamping to edges is important to prevent artifacts when scaling */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		/* Linear filtering usually looks best for text */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (font_face.LoadChar(charcode, FT_LOAD_RENDER)) {

			FT_GlyphSlot g = font_face.face()->glyph;

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
			m_glyph.bitmap_left = g->bitmap_left;
			m_glyph.bitmap_top = g->bitmap_top;
			m_glyph.bitmap_width = g->bitmap.width;
			m_glyph.bitmap_height = g->bitmap.rows;
			m_glyph.advance_x = g->advance.x >> 6;
			m_glyph.advance_y = g->advance.y >> 6;

			m_glyph.vertexes[0].x = m_glyph.bitmap_left;
			m_glyph.vertexes[0].y = m_glyph.bitmap_top - m_glyph.bitmap_height;
			m_glyph.vertexes[0].s = 0;
			m_glyph.vertexes[0].t = 1;

			m_glyph.vertexes[1].x = m_glyph.bitmap_left + m_glyph.bitmap_width;
			m_glyph.vertexes[1].y = m_glyph.bitmap_top - m_glyph.bitmap_height;
			m_glyph.vertexes[1].s = 1;
			m_glyph.vertexes[1].t = 1;

			m_glyph.vertexes[2].x = m_glyph.bitmap_left;
			m_glyph.vertexes[2].y = m_glyph.bitmap_top;
			m_glyph.vertexes[2].s = 0;
			m_glyph.vertexes[2].t = 0;

			m_glyph.vertexes[3].x = m_glyph.bitmap_left + m_glyph.bitmap_width;
			m_glyph.vertexes[3].y = m_glyph.bitmap_top;
			m_glyph.vertexes[3].s = 1;
			m_glyph.vertexes[3].t = 0;
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}


#ifdef DEBUG
	void GlyphTexture::draw_bitmap (FT_Bitmap* bitmap, FT_Int x, FT_Int y)
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

	void GlyphTexture::show_image (FT_Bitmap* bitmap)
	{
	}
#endif

}
