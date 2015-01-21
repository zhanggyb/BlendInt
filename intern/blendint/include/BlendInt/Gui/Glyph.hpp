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

#pragma once

namespace BlendInt {

	struct Glyph
	{
		Glyph ()
		: bitmap_width(0),
		  bitmap_height(0),
		  bitmap_left(0),
		  bitmap_top(0),
		  advance_x(0),
		  advance_y(0),
		  offset_u(0),
		  offset_v(0)
		{
		}

		Glyph (const Glyph& orig)
		: bitmap_width(orig.bitmap_width),
		  bitmap_height(orig.bitmap_height),
		  bitmap_left(orig.bitmap_left),
		  bitmap_top(orig.bitmap_top),
		  advance_x(orig.advance_x),
		  advance_y(orig.advance_y),
		  offset_u(orig.offset_u),
		  offset_v(orig.offset_v)
		{}

		Glyph& operator = (const Glyph& orig)
		{
			bitmap_width = orig.bitmap_width;
			bitmap_height = orig.bitmap_height;
			bitmap_left = orig.bitmap_left;
			bitmap_top = orig.bitmap_top;

			advance_x = orig.advance_x;
			advance_y = orig.advance_y;

			offset_u = orig.offset_u;
			offset_v = orig.offset_v;

			return *this;
		}

		int bitmap_width;
		int bitmap_height;

		int bitmap_left;
		int bitmap_top;

		int advance_x;
		int advance_y;

		int offset_u;
		int offset_v;
	};

} /* namespace BlendInt */
