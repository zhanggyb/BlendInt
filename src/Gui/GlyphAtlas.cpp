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

#include <BlendInt/Types.hpp>
#include <BlendInt/Gui/GlyphAtlas.hpp>

namespace BlendInt {

	GlyphAtlas::GlyphAtlas()
	: TextureAtlas2D(),
	  m_index(0)
	{

	}

	GlyphAtlas::~GlyphAtlas()
	{

	}

	bool GlyphAtlas::Push(int bitmap_width, int bitmap_rows, const unsigned char* bitmap_buf, int* r_x, int* r_y, bool clear)
	{
		// FIXME: in some truetype font like "Source Code Pro", the bitmap width may large than cell_width
		// need to find a solution
		if((bitmap_width > cell_width()) || (bitmap_rows > cell_height())) {
			DBG_PRINT_MSG("%s", "Bitmap size is large than a cell size");
			DBG_PRINT_MSG("bitmap width: %d, bitmap height: %d, cell width: %d, cell_height: %d", bitmap_width,
							bitmap_rows, cell_width(), cell_height());
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

		int columns = (tex_width - xoffset()) / (cell_width() + xspace());
		int rows = (tex_height - yoffset()) / (cell_height() + yspace());

		if(m_index > (rows * columns - 1)) {
			DBG_PRINT_MSG("%s", "Not enough space to push a new sub texture");
			return false;
		}

		int x = m_index % columns;
		int y = m_index / columns;

		x = xoffset() + (x * (cell_width() + xspace()));
		y = yoffset() + (y * (cell_height() + yspace()));

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		if(clear) {
			std::vector<unsigned char> blank(cell_width() * cell_height(), 0);
			glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, cell_width(), cell_height(), GL_RED, GL_UNSIGNED_BYTE, &blank[0]);
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, bitmap_width, bitmap_rows, GL_RED, GL_UNSIGNED_BYTE, bitmap_buf);

		if(r_x) *r_x = x;
		if(r_y) *r_y = y;

		return true;
	}


	bool GlyphAtlas::IsFull () const
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

		int columns = (tex_width - xoffset()) / (cell_width() + xspace());
		int rows = (tex_height - yoffset()) / (cell_height() + yspace());

		if(m_index > (rows * columns - 1)) {
			return false;
		}

		return true;
	}

	void GlyphAtlas::MoveToFirst ()
	{
		m_index = 0;
	}

	bool GlyphAtlas::MoveNext ()
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

		int columns = (tex_width - xoffset()) / (cell_width() + xspace());
		int rows = (tex_height - yoffset()) / (cell_height() + yspace());

		if(m_index > (rows * columns - 1)) {
			DBG_PRINT_MSG("%s", "Cannot go to next index, texture is full.");
			return false;
		}

		m_index++;
		return true;
	}


}

