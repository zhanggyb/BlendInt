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

#include <BlendInt/OpenGL/GLTexture2D.hpp>

namespace BlendInt {

	class TextureAtlas2D: public GLTexture2D
	{
	public:

		TextureAtlas2D ();

		virtual ~TextureAtlas2D ();

		void Generate (int width,
						int height,
						short cell_x,
						short cell_y,
						short xoffset = 1,
						short yoffset = 1,
						short xspace = 1,
						short yspace = 1);

		bool SetSubImage (int index,
						int bitmap_width,
						int bitmap_rows,
						const unsigned char* bitmap_buffer,
						int* r_x,
						int* r_y,
						bool clear = false);

		bool SetSubImage (int hindex,
						int yindex,
						int bitmap_width,
						int bitmap_rows,
						const unsigned char* bitmap_buffer,
						int* r_x,
						int* r_y,
						bool clear = false);

		int GetMaxNumber () const;

		int GetColumns () const;

		int GetRows () const;

		short xoffset () const
		{
			return m_xoffset;
		}

		short yoffset () const
		{
			return m_yoffset;
		}

		short xspace () const
		{
			return m_xspace;
		}

		short yspace () const
		{
			return m_yspace;
		}

		short cell_width () const
		{
			return m_cell_width;
		}

		short cell_height () const
		{
			return m_cell_height;
		}

	private:

		short m_cell_width;
		short m_cell_height;

		short m_xoffset;
		short m_yoffset;

		short m_xspace;
		short m_yspace;
	};

}

#endif /* TEXTUREATLAS2D_HPP_ */
