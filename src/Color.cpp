/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <BIL/Color.hpp>

namespace BIL {

	void Color::ConvertShadeColor (const Color& color,
								   short shadetop,
								   short shadedown,
								   Color* top_color,
								   Color* bottom_color)
	{
		top_color->set_red (color.r() + shadetop);
		top_color->set_green (color.g() + shadetop);
		top_color->set_blue (color.b() + shadetop);
		top_color->set_alpha (color.a());

		bottom_color->set_red (color.r() + shadedown);
		bottom_color->set_green (color.g() + shadedown);
		bottom_color->set_blue (color.b() + shadedown);
		bottom_color->set_alpha (color.a());
	}


	void Color::ConvertRoundBoxShadeColor (const Color& color1,
										   const Color& color2,
										   float factor,
										   unsigned char color_output[4])
	{
		unsigned char faci = convert_color_from_float (factor);
		unsigned char facm = 255 - faci;

		color_output[0] = (faci * color1.r() + facm * color2.r()) >> 8;
		color_output[1] = (faci * color1.g() + facm * color2.g()) >> 8;
		color_output[2] = (faci * color1.b() + facm * color2.b()) >> 8;
		color_output[3] = (faci * color1.a() + facm * color2.a()) >> 8;
	}

}
