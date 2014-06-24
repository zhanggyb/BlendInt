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

#include <BlendInt/Core/Types.hpp>
#include <vector>

#include <BlendInt/OpenGL/TextureAtlas2D.hpp>

namespace BlendInt {

	TextureAtlas2D::TextureAtlas2D ()
	: GLTexture2D(),
	  m_cell_width(0),
	  m_cell_height(0),
	  m_xoffset(0),
	  m_yoffset(0),
	  m_xspace(0),
	  m_yspace(0)
	{
	}

	TextureAtlas2D::~TextureAtlas2D ()
	{
	}

	void TextureAtlas2D::Generate (int width, int height, short cell_x,
					short cell_y, short xoffset, short yoffset, short xspace,
					short yspace)
	{
		GLTexture2D::Generate();

		glBindTexture(GL_TEXTURE_2D, texture());

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

		m_cell_width = cell_x;
		m_cell_height = cell_y;
		m_xspace = xspace;
		m_yspace = yspace;
		m_xoffset = xoffset;
		m_yoffset = yoffset;
	}

	bool TextureAtlas2D::SetSubImage (int index, int bitmap_width, int bitmap_rows, const unsigned char* bitmap_buf, int* r_x, int* r_y, bool clear)
	{
		if(bitmap_width > m_cell_width || bitmap_rows > m_cell_height) {
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

		int columns = (tex_width - m_xoffset) / (m_cell_width + m_xspace);
		int rows = (tex_height - m_yoffset) / (m_cell_height + m_yspace);

		if(index > (rows * columns - 1)) return false;

		x = index % columns;
		y = index / columns;

		x = m_xoffset + (x * (m_cell_width + m_xspace));
		y = m_yoffset + (y * (m_cell_height + m_yspace));

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		if(clear) {
			std::vector<unsigned char> blank(m_cell_width * m_cell_height, 0);
			glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, m_cell_width, m_cell_height, GL_RED, GL_UNSIGNED_BYTE, &blank[0]);
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, bitmap_width, bitmap_rows, GL_RED, GL_UNSIGNED_BYTE, bitmap_buf);

		if(r_x) *r_x = x;
		if(r_y) *r_y = y;

		return true;
	}

	bool TextureAtlas2D::SetSubImage (int column_index, int row_index, int bitmap_width, int bitmap_rows, const unsigned char* bitmap_buf, int* r_x, int* r_y, bool clear)
	{
		if(bitmap_width > m_cell_width || bitmap_rows > m_cell_height) {
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

		int columns = (tex_width - m_xoffset) / (m_cell_width + m_xspace);
		int rows = (tex_height - m_yoffset) / (m_cell_height + m_yspace);

		if(column_index > (columns - 1)) return false;
		if(row_index > (rows - 1)) return false;

		int x = 0, y = 0;
		x = m_xoffset + (column_index * (m_cell_width + m_xspace));
		y = m_yoffset + (row_index * (m_cell_height + m_yspace));

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		if(clear) {
			std::vector<unsigned char> blank(m_cell_width * m_cell_height, 0);
			glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, m_cell_width, m_cell_height, GL_RED, GL_UNSIGNED_BYTE, &blank[0]);
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, bitmap_width, bitmap_rows, GL_RED, GL_UNSIGNED_BYTE, bitmap_buf);

		if(r_x) *r_x = x;
		if(r_y) *r_y = y;

		return true;
	}

	int TextureAtlas2D::GetColumns () const
	{
		GLint tex_width = 0;

		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_WIDTH,
						&tex_width);

		int columns = (tex_width - m_xoffset) / (m_cell_width + m_xspace);

		return columns;
	}

	int TextureAtlas2D::GetRows () const
	{
		GLint tex_height = 0;

		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_HEIGHT,
						&tex_height);

		int rows = (tex_height - m_yoffset) / (m_cell_height + m_yspace);

		return rows;
	}

	int TextureAtlas2D::GetMaxNumber() const
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

		int h = (tex_width - m_xoffset) / (m_cell_width + m_xspace);
		int v = (tex_height - m_yoffset) / (m_cell_height + m_yspace);

		num = h * v;

		return num;
	}

}
