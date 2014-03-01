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

#include <string.h>
#include <BlendInt/Gui/Glyph.hpp>

namespace BlendInt {

	Glyph::Glyph ()
	: bitmap_width(0), bitmap_height(0), bitmap_left(0), bitmap_top(0),
	  advance_x (0), advance_y(0), texture_offset_x(0), texture_offset_y(0)
	{
		memset(vertexes, 6 * sizeof(Vertex2D), 0);
	}

} // namespace BlendInt
