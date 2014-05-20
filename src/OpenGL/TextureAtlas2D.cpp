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

#include <BlendInt/Types.hpp>
#include <vector>

#include <BlendInt/OpenGL/TextureAtlas2D.hpp>

namespace BlendInt {

	TextureAtlas2D::TextureAtlas2D ()
	: Object(),
	  m_texture(0),
	  m_cell_width(0),
	  m_cell_height(0),
	  m_space(0),
	  m_xoffset(0),
	  m_yoffset(0)
	{
	}

	TextureAtlas2D::~TextureAtlas2D ()
	{
		if(m_texture) {
			glDeleteTextures(1, &m_texture);
		}
	}

	void TextureAtlas2D::Generate (int width, int height, int cell_x, int cell_y, int space)
	{
		if(m_texture) {
			glDeleteTextures(1, &m_texture);
		}

		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);

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

		// We require 1 byte alignment when uploading texture data
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Clamping to edges is important to prevent artifacts when scaling
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Linear filtering usually looks best for text
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		m_cell_width = cell_x;
		m_cell_height = cell_y;
		m_space = space;

		m_xoffset = m_space;
		m_yoffset = m_space;
	}

	bool TextureAtlas2D::Push(int bitmap_width, int bitmap_rows, unsigned char* bitmap_buf, bool clear)
	{
		GLint tex_width = 0;
		GLint tex_height = 0;
		int x = m_xoffset + m_cell_width + m_space;
		int y = m_yoffset + m_cell_height + m_space;

		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_WIDTH,
						&tex_width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_HEIGHT,
						&tex_height);

		if(x > tex_width || y > tex_height) {
			DBG_PRINT_MSG("%s", "Not enough space to push a new sub texture");
			return false;
		}

		if((bitmap_width > m_cell_width) || (bitmap_rows > m_cell_height)) {
			DBG_PRINT_MSG("%s", "Bitmap size is large than a cell size");
			DBG_PRINT_MSG("bitmap width: %d, bitmap height: %d, cell width: %d, cell_height: %d", bitmap_width,
							bitmap_rows, m_cell_width, m_cell_height);
			return false;
		}

		//DBG_PRINT_MSG("push character at: %d, %d, width: %d, rows: %d", m_xoffset, m_yoffset, width, rows);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		if(clear) {
			std::vector<unsigned char> blank(m_cell_width * m_cell_height, 0);
			glTexSubImage2D(GL_TEXTURE_2D, 0, m_xoffset, m_yoffset, m_cell_width, m_cell_height, GL_RED, GL_UNSIGNED_BYTE, &blank[0]);
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, m_xoffset, m_yoffset, bitmap_width, bitmap_rows, GL_RED, GL_UNSIGNED_BYTE, bitmap_buf);

		return true;
	}

	bool TextureAtlas2D::Update (int index, int bitmap_width, int bitmap_rows, unsigned char* bitmap_buf, bool clear)
	{
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

		int columns = (tex_width - m_space) / (m_cell_width + m_space);
		int rows = (tex_height - m_space) / (m_cell_height + m_space);

		if(index > (rows * columns - 1)) return false;

		x = index % columns;
		y = index / columns;

		//DBG_PRINT_MSG("local col: %d, local row: %d", x, y);

		x = m_space + (x * (m_cell_width + m_space));
		y = m_space + (y * (m_cell_height + m_space));

		//DBG_PRINT_MSG("locate cell position: %d, %d, columns: %d, rows: %d", x, y, columns, rows);

		if(bitmap_width >= m_cell_width || bitmap_rows >= m_cell_height) {
			return false;
		}

		//DBG_PRINT_MSG("Update character at: %d, %d, width: %d, rows: %d", x, y, bitmap_width, bitmap_rows);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		if(clear) {
			std::vector<unsigned char> blank(m_cell_width * m_cell_height, 0);
			glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, m_cell_width, m_cell_height, GL_RED, GL_UNSIGNED_BYTE, &blank[0]);
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, bitmap_width, bitmap_rows, GL_RED, GL_UNSIGNED_BYTE, bitmap_buf);

		return true;
	}

	bool TextureAtlas2D::Update (int column_index, int row_index, int bitmap_width, int bitmap_rows, unsigned char* bitmap_buf, bool clear)
	{
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

		int columns = (tex_width - m_space) / (m_cell_width + m_space);
		int rows = (tex_height - m_space) / (m_cell_height + m_space);

		if(column_index > (columns - 1)) return false;
		if(row_index > (rows - 1)) return false;

		int x = 0, y = 0;
		x = m_space + (column_index * (m_cell_width + m_space));
		y = m_space + (row_index * (m_cell_height + m_space));

		//DBG_PRINT_MSG("locate cell position: %d, %d, columns: %d, rows: %d", x, y, columns, rows);

		if(bitmap_width >= m_cell_width || bitmap_rows >= m_cell_height) {
			return false;
		}

		//DBG_PRINT_MSG("Update character at: %d, %d, width: %d, rows: %d", x, y, bitmap_width, bitmap_rows);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		if(clear) {
			std::vector<unsigned char> blank(m_cell_width * m_cell_height, 0);
			glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, m_cell_width, m_cell_height, GL_RED, GL_UNSIGNED_BYTE, &blank[0]);
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, bitmap_width, bitmap_rows, GL_RED, GL_UNSIGNED_BYTE, bitmap_buf);

		return true;
	}


	bool TextureAtlas2D::IsFull () const
	{
		bool ret = true;

		GLint tex_width = 0;
		GLint tex_height = 0;
		int x = m_xoffset + m_cell_width + m_space;
		int y = m_yoffset + m_cell_height + m_space;

		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_WIDTH,
						&tex_width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_HEIGHT,
						&tex_height);

		if(x <= tex_width || y <= tex_height) {
			ret = false;
		}

		return ret;
	}

	void TextureAtlas2D::MoveToFirst ()
	{
		m_xoffset = m_space;
		m_yoffset = m_space;
	}

	bool TextureAtlas2D::MoveNext ()
	{
		GLint tex_width = 0;
		GLint tex_height = 0;
		int x = m_xoffset;
		int y = m_yoffset;

		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_WIDTH,
						&tex_width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_HEIGHT,
						&tex_height);

		if((x + m_cell_width + m_space) > tex_width || (y + m_cell_height + m_space) > tex_height) {
			return false;
		}

		x += m_cell_width + m_space;
		if((x + m_cell_width + m_space) > tex_width) {
			x = m_space;
			y = m_yoffset + m_cell_height + m_space;
		}

		m_xoffset = x;
		m_yoffset = y;

		if((x + m_cell_height + m_space) <= tex_width && (y + m_cell_height + m_space) <= tex_height) {
			return true;
		} else {
			return false;
		}
	}

	GLint TextureAtlas2D::GetWidth (int level) const
	{
		GLint width = 0;

		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						level,
						GL_TEXTURE_WIDTH,
						&width);

		return width;
	}

	GLint TextureAtlas2D::GetHeight (int level) const
	{
		GLint height = 0;

		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						level,
						GL_TEXTURE_WIDTH,
						&height);

		return height;
	}

	int TextureAtlas2D::GetColumns () const
	{
		GLint tex_width = 0;

		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_WIDTH,
						&tex_width);

		int columns = (tex_width - m_space) / (m_cell_width + m_space);

		return columns;
	}

	int TextureAtlas2D::GetRows () const
	{
		GLint tex_height = 0;

		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_HEIGHT,
						&tex_height);

		int rows = (tex_height - m_space) / (m_cell_height + m_space);

		return rows;
	}

	int TextureAtlas2D::GetCurrentIndex() const
	{
		int index = 0;

		GLint tex_width = 0;

		glGetTexLevelParameteriv(GL_TEXTURE_2D,
						0,
						GL_TEXTURE_WIDTH,
						&tex_width);

		int h = (tex_width - m_space) / (m_cell_width + m_space);

		int x = (m_xoffset - m_space) / (m_cell_height + m_space);
		int y = (m_yoffset - m_space) / (m_cell_height + m_space);

		index = y * h + x;

		return index;
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

		int h = (tex_width - m_space) / (m_cell_width + m_space);
		int v = (tex_height - m_space) / (m_cell_height - m_space);

		num = h * v;

		return num;
	}

	void TextureAtlas2D::Bind ()
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}

	void TextureAtlas2D::Clear ()
	{
		glDeleteTextures(1, &m_texture);
		m_texture = 0;
	}

	void TextureAtlas2D::Reset ()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}
