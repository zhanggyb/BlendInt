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

#ifndef TEXTUREATLAS2D_HPP_
#define TEXTUREATLAS2D_HPP_

#include <BlendInt/Core/Object.hpp>

namespace BlendInt {

	class TextureAtlas2D: public Object
	{
	public:

		TextureAtlas2D ();

		virtual ~TextureAtlas2D ();

		void Generate (int width, int height, int cell_x, int cell_y, int space = 1);

		void Bind ();

		bool Push (int bitmap_width, int bitmap_rows, unsigned char* bitmap_buffer, bool clear = false);

		bool Update (int index, int bitmap_width, int bitmap_rows, unsigned char* bitmap_buffer, bool clear = false);

		bool Update (int hindex, int yindex, int bitmap_width, int bitmap_rows, unsigned char* bitmap_buffer, bool clear = false);

		bool IsFull () const;

		void MoveToFirst ();

		bool MoveNext ();

		int GetCurrentIndex () const;

		int GetMaxNumber () const;

		GLint GetWidth (int level = 0) const;

		GLint GetHeight (int level = 0) const;

		int GetColumns () const;

		int GetRows () const;

		void Clear ();

		static void Reset ();

		GLuint texture () const
		{
			return m_texture;
		}

		int xoffset () const
		{
			return m_xoffset;
		}

		int yoffset () const
		{
			return m_yoffset;
		}

		int space () const
		{
			return m_space;
		}

		int cell_width () const
		{
			return m_cell_width;
		}

		int cell_height () const
		{
			return m_cell_height;
		}

	private:

		GLuint m_texture;

		int m_cell_width;
		int m_cell_height;

		int m_space;

		int m_xoffset;
		int m_yoffset;
	};

}

#endif /* TEXTUREATLAS2D_HPP_ */
