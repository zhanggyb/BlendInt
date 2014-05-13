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

#include <string.h>
#include <iostream>
#include <algorithm>
#include <stdexcept>

#include <BlendInt/Core/Freetype.hpp>

#include <BlendInt/Gui/TextureAtlas.hpp>
#include <BlendInt/Types.hpp>

// Maximum texture width
#define MAXWIDTH 2048

namespace BlendInt {

	TextureAtlasExt::TextureAtlasExt ()
	: Object(),
	  m_texture(0),
	  m_cell_x(0),
	  m_cell_y(0),
	  m_space(0),
	  m_last_x(0),
	  m_last_y(0)
	{
	}

	TextureAtlasExt::~TextureAtlasExt ()
	{
		if(m_texture) {
			glDeleteTextures(1, &m_texture);
		}
	}

	void TextureAtlasExt::Generate (int width, int height, int cell_x, int cell_y, int space)
	{
		if(m_texture) {
			glDeleteTextures(1, &m_texture);
		}

		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

		// We require 1 byte alignment when uploading texture data
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Clamping to edges is important to prevent artifacts when scaling
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Linear filtering usually looks best for text
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		m_cell_x = cell_x;
		m_cell_y = cell_y;
		m_space = space;

		m_last_x = m_space;
		m_last_y = m_space;
	}
	
	bool TextureAtlasExt::Push(int width, int rows, unsigned char* buf)
	{
		bool ret = false;

		if(!glIsTexture(m_texture)) return ret;

		GLint tex_width = 0;
		GLint tex_height = 0;
		int x = m_last_x + m_cell_x + m_space;
		int y = m_last_y + m_cell_y + m_space;

		glBindTexture(GL_TEXTURE_2D, m_texture);
		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_WIDTH,
						&tex_width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_HEIGHT,
						&tex_height);


		if(x <= tex_width && y <= tex_height) {
			ret = true;
		}

		if(ret) {

			if(width >= m_cell_x || rows >= m_cell_y) {
				ret = false;
			} else {

				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glTexSubImage2D(GL_TEXTURE_2D, 0, m_last_x, m_last_y, width, rows, GL_RED, GL_UNSIGNED_BYTE, buf);

				// TODO: upload image at last (x, y)
				fprintf(stdout, "push at (%d, %d)\n", m_last_x, m_last_y);

				m_last_x = x;
				if((m_last_x + m_cell_x + m_space) > tex_width) {
					m_last_x = m_space;
					m_last_y = y;
				}

			}
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		return ret;
	}

	bool TextureAtlasExt::IsFull () const
	{
		bool ret = true;

		GLint tex_width = 0;
		GLint tex_height = 0;
		int x = m_last_x + m_cell_x + m_space;
		int y = m_last_y + m_cell_y + m_space;

		glBindTexture(GL_TEXTURE_2D, m_texture);
		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_WIDTH,
						&tex_width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_HEIGHT,
						&tex_height);
		glBindTexture(GL_TEXTURE_2D, 0);

		if(x <= tex_width || y <= tex_height) {
			ret = false;
		}

		return ret;
	}

	int TextureAtlasExt::GetMaxNumber() const
	{
		int num = 0;

		GLint tex_width = 0;
		GLint tex_height = 0;

		glBindTexture(GL_TEXTURE_2D, m_texture);
		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_WIDTH,
						&tex_width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_HEIGHT,
						&tex_height);
		glBindTexture(GL_TEXTURE_2D, 0);

		int h = (tex_width - m_space) / (m_cell_x + m_space);
		int v = (tex_height - m_space) / (m_cell_y - m_space);

		num = h * v;

		return num;
	}

	void TextureAtlasExt::Clear ()
	{
		glDeleteTextures(1, &m_texture);
		m_texture = 0;
	}

	// --------------------------------

	TextureAtlas::TextureAtlas ()
			: m_texture(0), m_width(0), m_height(0), m_starting_charcode(0), m_stride(0),
			  m_glyph_array(0)
	{
	}

	TextureAtlas::~TextureAtlas ()
	{
		if(glIsTexture(m_texture)) {
			glDeleteTextures(1, &m_texture);
		}

		if(m_glyph_array) {
			delete [] m_glyph_array;
		}
	}

	void TextureAtlas::Generate (const Freetype& freetype, wchar_t start, int size)
	{
		if(!freetype.valid()) return;

		if(size <= 0) return;

		m_stride = size;
		m_starting_charcode = start;

		if(m_glyph_array) {
			delete [] m_glyph_array;
			m_glyph_array = 0;
		}
		m_glyph_array = new Glyph[m_stride];

		if(glIsTexture(m_texture)) {
			glDeleteTextures(1, &m_texture);
		}

		FT_GlyphSlot g = freetype.face()->glyph;

		int roww = 0;
		int rowh = 0;
		m_width = 0;
		m_height = 0;

		/* Find minimum size for a texture holding all visible ASCII characters */
		for (wchar_t i = m_starting_charcode; i < (m_starting_charcode + m_stride - 1); i++) {
			if (!freetype.LoadCharacter(i, FT_LOAD_RENDER)) {
				fprintf(stderr, "Loading character %c failed!\n", i);
				continue;
			}
			if (roww + g->bitmap.width + 1 >= MAXWIDTH) {
				m_width = std::max(m_width, static_cast<unsigned int>(roww));
				m_height += rowh;
				roww = 0;
				rowh = 0;
			}
			roww += g->bitmap.width + 1;
			rowh = std::max(rowh, g->bitmap.rows);
		}

		m_width = std::max(m_width, static_cast<unsigned int>(roww));
		m_height += rowh;

		/* Create a texture that will be used to hold all ASCII glyphs */
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

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

		int offset = 0;
		float texture_offset_x = 0.f;
		float texture_offset_y = 0.f;
		for (wchar_t i = m_starting_charcode; i < (m_starting_charcode + m_stride - 1); i++) {
			if (!freetype.LoadCharacter(i, FT_LOAD_RENDER)) {
				fprintf(stderr, "Loading character %c failed!\n", i);
				continue;
			}

			if (ox + g->bitmap.width + 1 >= MAXWIDTH) {
				oy += rowh;
				rowh = 0;
				ox = 0;
			}

			glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width,
							g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE,
							g->bitmap.buffer);

			offset = i - m_starting_charcode;
			(m_glyph_array + offset )->advance_x = g->advance.x >> 6;
			(m_glyph_array + offset)->advance_y = g->advance.y >> 6;

			(m_glyph_array + offset)->bitmap_width = g->bitmap.width;
			(m_glyph_array + offset)->bitmap_height = g->bitmap.rows;

			(m_glyph_array + offset)->bitmap_left = g->bitmap_left;
			(m_glyph_array + offset)->bitmap_top = g->bitmap_top;

			texture_offset_x = ox / (float)m_width;
			texture_offset_y = oy / (float)m_height;

			(m_glyph_array + offset)->vertexes[0].x = (m_glyph_array + offset)->bitmap_left;
			(m_glyph_array + offset)->vertexes[0].y = (m_glyph_array + offset)->bitmap_top - (m_glyph_array + offset)->bitmap_height;
			(m_glyph_array + offset)->vertexes[0].s = texture_offset_x;
			(m_glyph_array + offset)->vertexes[0].t = texture_offset_y + (m_glyph_array + offset)->bitmap_height / m_height;

			(m_glyph_array + offset)->vertexes[1].x = (m_glyph_array + offset)->bitmap_left + (m_glyph_array + offset)->bitmap_width;
			(m_glyph_array + offset)->vertexes[1].y = (m_glyph_array + offset)->bitmap_top - (m_glyph_array + offset)->bitmap_height;
			(m_glyph_array + offset)->vertexes[1].s = texture_offset_x + (m_glyph_array + offset)->bitmap_width / m_width;
			(m_glyph_array + offset)->vertexes[1].t = texture_offset_y + (m_glyph_array + offset)->bitmap_height / m_height;

			(m_glyph_array + offset)->vertexes[2].x = (m_glyph_array + offset)->bitmap_left;
			(m_glyph_array + offset)->vertexes[2].y = (m_glyph_array + offset)->bitmap_top;
			(m_glyph_array + offset)->vertexes[2].s = texture_offset_x;
			(m_glyph_array + offset)->vertexes[2].t = texture_offset_y;

			(m_glyph_array + offset)->vertexes[3].x = (m_glyph_array + offset)->bitmap_left + (m_glyph_array + offset)->bitmap_width;
			(m_glyph_array + offset)->vertexes[3].y = (m_glyph_array + offset)->bitmap_top;
			(m_glyph_array + offset)->vertexes[3].s = texture_offset_x + (m_glyph_array + offset)->bitmap_width / m_width;
			(m_glyph_array + offset)->vertexes[3].t = texture_offset_y;

			rowh = std::max(rowh, g->bitmap.rows);
			ox += g->bitmap.width + 1;
		}

		glBindTexture(GL_TEXTURE_2D, 0);

#ifdef DEBUG
		fprintf(stdout, "Generated %u characters with a %u x %u (%u kb) texture atlas: \n",
				m_stride, m_width, m_height, m_width * m_height / 1024);

		//std::cerr << ConvertFromString(freetype->font().family) << " with size: "<< freetype->font().size << " and dpi: " << freetype->dpi() << std::endl;
#endif
	}

	bool TextureAtlas::Contain(wchar_t charcode) const
	{
		int offset = charcode - m_starting_charcode;

		if (offset < 0 || offset > (m_stride - 1))  return false;

		return true;
	}

	const Glyph& TextureAtlas::glyph (wchar_t charcode) const
	{
		int offset = charcode - m_starting_charcode;
		if(offset < 0 || offset >= m_stride) {

			//throw std::out_of_range("Charcode not in atlas\n");

			// for unknown character, just display '?'
			charcode = '?';
			offset = charcode - m_starting_charcode;

		}
		if(!m_glyph_array) {
			throw std::runtime_error("No glyph allocated in this atlas\n");
		}

		return *(m_glyph_array + offset);
	}

}
