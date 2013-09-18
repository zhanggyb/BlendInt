/*
 * This file is part of BILO (Blender Interface Library).
 *
 * BILO (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

/**
 * @brief Code for character glyph
 */

#ifndef _BIL_GLYPH_H_
#define _BIL_GLYPH_H_

using namespace std;

namespace BILO {

	struct Glyph
	{
		Glyph ();
		float bitmap_width;
		float bitmap_height;

		float bitmap_left;
		float bitmap_top;

		float advance_x;
		float advance_y;

		float texture_offset_x;
		float texture_offset_y;
	};

} /* namespace BILO */

#endif  /* _BIL_GLYPH_H_ */
