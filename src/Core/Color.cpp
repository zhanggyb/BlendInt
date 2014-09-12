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

#include <BlendInt/Core/Color.hpp>

namespace BlendInt {

	Color operator + (const Color& orig, short shade)
	{
		Color color;
		color.set_red(orig.r() + shade / 255.f);
		color.set_green(orig.g() + shade / 255.f);
		color.set_blue(orig.b() + shade / 255.f);
		color.set_alpha(orig.a());

		return color;
	}

	Color operator + (const Color& color1, const Color& color2)
	{
		Color color;
		color.set_red(color1.r() + color2.r());
		color.set_green(color1.g() + color2.g());
		color.set_blue(color1.b() + color2.b());
		color.set_alpha(color1.a() + color2.a());

		return color;
	}

	extern bool operator ==(const Color& src, const Color& dist)
	{
		return (src.r() == dist.r()) &&
				(src.g() == dist.g()) &&
				(src.b() == dist.b()) &&
				(src.a() == dist.a());
	}

	Color MakeShadedColor (const Color& color1, const Color& color2, float fact)
	{
		Color shaded_color;

		float faci = glm::clamp(fact - 0.5f / 255.f, 0.f, 1.f);
		float facm = 1.f - fact;

		shaded_color.set_red(faci * color1.r() + facm * color2.r());
		shaded_color.set_green(faci * color1.g() + facm * color2.g());
		shaded_color.set_blue(faci * color1.b() + facm * color2.b());
		shaded_color.set_alpha(faci * color1.a() + facm * color2.a());

		return shaded_color;
	}

}
