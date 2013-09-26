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

#include <BILO/Frame.hpp>

namespace BILO {

	Frame::Frame ()
	: Widget(), m_layout(0)
	{

	}

	Frame::Frame (Drawable* parent)
	: Widget (parent), m_layout(0)
	{

	}

	Frame::~Frame ()
	{

	}

	void Frame::set_layout(AbstractLayout* layout)
	{
		if (m_layout == layout) return;
	}

	void Frame::render ()
	{

	}

}
