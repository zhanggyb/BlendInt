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

#include <BILO/AbstractLayout.hpp>

namespace BILO {

	AbstractLayout::AbstractLayout ()
			: Drawable(), m_alignment(0), m_space(1)
	{
	}

	AbstractLayout::AbstractLayout (Drawable *parent)
			: Drawable (parent), m_alignment(0), m_space(1)
	{
	}

	AbstractLayout::~AbstractLayout ()
	{
	}

	void AbstractLayout::set_margin (const Margin& margin)
	{
		Margin new_value = margin;

		if(update(WidgetPropertyMargin, &new_value)) m_margin = new_value;
	}

	void AbstractLayout::set_margin (int left, int right, int top, int bottom)
	{
		Margin new_value (left, right, top, bottom);

		if(update(WidgetPropertyMargin, &new_value)) m_margin = new_value;
	}



} /* namespace BILO */
