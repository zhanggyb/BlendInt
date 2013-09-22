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

#include <BILO/AbstractLayout.hpp>

namespace BILO {

	AbstractLayout::AbstractLayout (Drawable *parent)
			: Drawable (parent), alignment_(0)
	{
		set_padding(Padding(5, 5, 5, 5));
		set_margin(1, 1, 1, 1);
	}

	AbstractLayout::~AbstractLayout ()
	{
		// TODO Auto-generated destructor stub
	}

	void AbstractLayout::add_widget (Widget* widget)
	{
		m_list.push_back(widget);

		bind (widget);

		update(WidgetPropertySize);
	}

	void AbstractLayout::add_layout(AbstractLayout* layout)
	{
		m_list.push_back(layout);
		bind (layout);

		update(WidgetPropertySize);

	}

	void AbstractLayout::press_key (KeyEvent* event)
	{
	}

	void AbstractLayout::press_context_menu (ContextMenuEvent* event)
	{
	}

	void AbstractLayout::release_context_menu (ContextMenuEvent* event)
	{
	}

	void AbstractLayout::press_mouse (MouseEvent* event)
	{
	}

	void AbstractLayout::release_mouse (MouseEvent* event)
	{
	}

	void AbstractLayout::move_mouse (MouseEvent* event)
	{
	}


} /* namespace BILO */
