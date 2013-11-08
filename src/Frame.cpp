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

#include <BlendInt/Frame.hpp>
#include <BlendInt/Types.hpp>
#include <BlendInt/Coord.hpp>
#include <BlendInt/Color.hpp>

#include <BlendInt/Utilities-inl.hpp>

#include <BlendInt/Interface.hpp>
#include <BlendInt/Theme.hpp>

namespace BlendInt {

	Frame::Frame ()
	: Widget()
	{
		set_minimal_size(padding().left() + padding().right(),
				padding().top() + padding().bottom());
		resize(120, 80);
		set_preferred_size(120, 80);
	}

	Frame::Frame (AbstractWidget* parent)
			: Widget(parent)
	{
		set_minimal_size(padding().left() + padding().right(),
				padding().top() + padding().bottom());
		resize(120, 80);
		set_preferred_size(120, 80);
	}

	Frame::~Frame ()
	{
		// TODO Auto-generated destructor stub
	}

	void Frame::set_padding (const Padding& padding)
	{
		if(m_padding.equal(padding)) return;

		Padding new_padding = padding;
		update(FramePropertyPadding, &new_padding);
		m_padding = new_padding;
	}

	void Frame::set_padding (int l, int r, int t, int b)
	{
		if(m_padding.equal(l, r, t, b)) return;

		Padding new_padding(l, r, t, b);

		update(FramePropertyPadding, &new_padding);
		m_padding = new_padding;
	}


	void Frame::press_key (KeyEvent* event)
	{
	}

	void Frame::press_context_menu (ContextMenuEvent* event)
	{
	}

	void Frame::release_context_menu (ContextMenuEvent* event)
	{
	}

	void Frame::press_mouse (MouseEvent* event)
	{
	}

	void Frame::release_mouse (MouseEvent* event)
	{
	}

	void Frame::move_mouse (MouseEvent* event)
	{
	}

	void Frame::update (int type, const void* data)
	{
		Widget::update(type, data);
	}

	void Frame::render ()
	{
		Widget::render();
	}

	bool Frame::contain_no_padding (const Coord2d& cursor)
	{
		if (cursor.x() < (position().x() + m_padding.left()) ||
				cursor.y() < (position().y() + m_padding.bottom()) ||
				cursor.x() > (position().x() + size().width() - m_padding.right()) ||
				cursor.y() > (position().y() + size().height() - m_padding.top())) {
			return false;
		}

		return true;
	}

} /* namespace BlendInt */

