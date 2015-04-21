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

#include <blendint/core/color.hpp>

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

  extern bool operator == (const Color& src, const Color& dst)
  {
    return memcmp(src.data(), dst.data(), sizeof(float) * 4) == 0;
  }

  extern bool operator != (const Color& src, const Color& dst)
  {
    return memcmp(src.data(), dst.data(), sizeof(float) * 4) != 0;
  }

}
