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

#ifndef _BLENDINT_GUI_GLYPHATLAS_HPP_
#define _BLENDINT_GUI_GLYPHATLAS_HPP_

#include <BlendInt/OpenGL/TextureAtlas2D.hpp>

namespace BlendInt {

	class GlyphAtlas: public TextureAtlas2D
	{
	public:

		GlyphAtlas ();

		virtual ~GlyphAtlas ();

		bool Push (int bitmap_width,
						int bitmap_rows,
						const unsigned char* bitmap_buffer,
						int* r_x,
						int* r_y,
						bool clear = false);

		bool IsFull () const;

		void MoveToFirst ();

		bool MoveNext ();

		int index () const
		{
			return m_index;
		}

	private:

		int m_index;
	};

}

#endif /* _BLENDINT_GUI_GLYPHATLAS_HPP_ */
