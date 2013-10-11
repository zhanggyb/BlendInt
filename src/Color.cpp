/*
 * This file is part of BILO (Blender-like Interface Library in
 * OpenGL).
 *
 * BILO (Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <string.h>

#include <BILO/Color.hpp>

namespace BILO {

	Color operator + (const Color& orig, short shade)
	{
		Color color;

		color.set_red(orig.r() + shade);
		color.set_green(orig.g() + shade);
		color.set_blue(orig.b() + shade);
		color.set_alpha(orig.a());

		return color;
	}

	Color make_shade_color (const Color& color1, const Color& color2, float factor)
	{
		unsigned char faci = float_to_uchar (factor);
		unsigned char facm = 255 - faci;
		Color shaded_color;

		shaded_color.set_red((faci * color1.r() + facm * color2.r()) >> 8);
		shaded_color.set_green((faci * color1.g() + facm * color2.g()) >> 8);
		shaded_color.set_blue((faci * color1.b() + facm * color2.b()) >> 8);
		shaded_color.set_alpha((faci * color1.a() + facm * color2.a()) >> 8);

		return shaded_color;
	}

	Color::Color()
	{
		memset (m_color_v, 0xFF, sizeof(unsigned char) * 4);
	}

	Color::Color(const Color& orig)
	{
		memcpy (m_color_v, orig.data(), sizeof(unsigned char) * 4);
	}

	Color& Color::operator = (const Color& orig)
	{
		memcpy (m_color_v, orig.data(), sizeof(unsigned char) * 4);

		return *this;
	}

	void Color::set_color(unsigned char color[4])
	{
		memcpy (m_color_v, color, sizeof(unsigned char) * 4);
	}

}
