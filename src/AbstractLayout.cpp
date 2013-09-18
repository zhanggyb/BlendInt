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

#include <BIL/AbstractLayout.hpp>

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

	bool AbstractLayout::add_widget (Widget* widget)
	{
		bool ret = add_child (widget);

		update(WidgetPropertySize);

		return ret;
	}

	bool AbstractLayout::add_layout(AbstractLayout* layout)
	{
		bool ret = add_child (layout);

		update(WidgetPropertySize);

		return ret;
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


} /* namespace BIL */
