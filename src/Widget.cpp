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

#include <GL/glew.h>
#include <assert.h>
#include <algorithm>

#include <iostream>

#include <BlendInt/Widget.hpp>
#include <BlendInt/Types.hpp>
#include <BlendInt/Coord.hpp>
#include <BlendInt/Color.hpp>

#include <BlendInt/Utilities-inl.hpp>

#include <BlendInt/Interface.hpp>
#include <BlendInt/Theme.hpp>

namespace BlendInt {

	Widget::Widget ()
	: Form()
	{
		set_minimal_size(padding().left() + padding().right(),
				padding().top() + padding().bottom());
		resize(120, 80);
		set_preferred_size(120, 80);
	}

	Widget::Widget (AbstractForm* parent)
			: Form(parent)
	{
		set_minimal_size(padding().left() + padding().right(),
				padding().top() + padding().bottom());
		resize(120, 80);
		set_preferred_size(120, 80);
	}

	Widget::~Widget ()
	{
		// TODO Auto-generated destructor stub
	}

	void Widget::set_padding (const Padding& padding)
	{
		m_padding = padding;
		update(WidgetPropertyPadding);
	}

	void Widget::set_padding (int l, int r, int t, int b)
	{
		m_padding.set_left(l);
		m_padding.set_right(r);
		m_padding.set_top(t);
		m_padding.set_bottom(b);

		update(WidgetPropertyPadding);
	}


	void Widget::press_key (KeyEvent* event)
	{
	}

	void Widget::press_context_menu (ContextMenuEvent* event)
	{
	}

	void Widget::release_context_menu (ContextMenuEvent* event)
	{
	}

	void Widget::press_mouse (MouseEvent* event)
	{
	}

	void Widget::release_mouse (MouseEvent* event)
	{
	}

	void Widget::move_mouse (MouseEvent* event)
	{
	}

	void Widget::update (int property_type)
	{
		Form::update(property_type);
	}

	void Widget::render ()
	{
#ifdef DEBUG
		std::set<AbstractForm*>::const_iterator it;
		AbstractForm *item = 0;
		for (it = m_children.begin(); it != m_children.end(); it++) {
			item = *it;
			Interface::instance()->dispatch_render_event(item);
		}
#endif
	}

	bool Widget::contain_no_padding (const Coord2d& cursor)
	{
		if (cursor.x() < (position().x() + m_padding.left()) ||
				cursor.y() < (position().y() + m_padding.bottom()) ||
				cursor.x() > (position().x() + m_size.width() - m_padding.right()) ||
				cursor.y() > (position().y() + m_size.height() - m_padding.top())) {
			return false;
		}

		return true;
	}

} /* namespace BlendInt */

