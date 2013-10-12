/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <BlendInt/PixelIcon.hpp>

namespace BlendInt {

	PixelIcon::PixelIcon ()
	: m_width(0), m_height(0)
	{
	}

	PixelIcon::PixelIcon (const unsigned char* pixels, int width, int height)
	: m_width(0), m_height(0)
	{
	}

	PixelIcon::PixelIcon (const PixelIcon& orig)
	: m_width(0), m_height(0)
	{
	}

	PixelIcon::~PixelIcon ()
	{
	}

	void PixelIcon::set_pixels (const unsigned char* pixels)
	{
	}

	void PixelIcon::scale (float ratio)
	{
	}

	void PixelIcon::scale (int width, int height)
	{
	}

	void PixelIcon::display ()
	{
	}

	void PixelIcon::display (float x, float y)
	{
	}

}
