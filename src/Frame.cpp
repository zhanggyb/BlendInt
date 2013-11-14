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
	: Widget(), m_widget(0)
	{
		set_minimal_size(padding().left() + padding().right(),
				padding().top() + padding().bottom());
		resize(120, 80);
		set_preferred_size(120, 80);
	}

	Frame::Frame (AbstractWidget* parent)
			: Widget(parent), m_widget(0)
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
		update(FramePadding, &new_padding);
		m_padding = new_padding;
	}

	void Frame::set_padding (int l, int r, int t, int b)
	{
		if(m_padding.equal(l, r, t, b)) return;

		Padding new_padding(l, r, t, b);

		update(FramePadding, &new_padding);
		m_padding = new_padding;
	}

	void Frame::set_widget(AbstractWidget* widget)
	{
		if(widget && (widget != m_widget)) {
			delete m_widget;
			m_widget = widget;

			m_widget->set_position(position().x() + padding().left(), position().y() + padding().bottom());
			m_widget->resize(size().width() - padding().left() - padding().right(),
					size().height() - padding().top() - padding().bottom());

			bind(m_widget);
		}
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
		switch (type) {

			case FormPosition: {
				if(m_widget) {
					const Point* pos_p = static_cast<const Point*>(data);
					int offset_x = pos_p->x() - position().x();
					int offset_y = pos_p->y() - position().y();
					m_widget->set_position(m_widget->position().x() + offset_x, m_widget->position().y() + offset_y);
				}
				break;
			}

			case FormSize: {
				if(m_widget) {
					Size size = *(static_cast<const Size*>(data));
					size.add_width( - (padding().left() + padding().right()) );
					size.add_height( - (padding().top() + padding().bottom()) );
					m_widget->resize(size);
				}
				break;
			}

			case FramePadding: {
				if(m_widget) {
					const Padding* pad_p = static_cast<const Padding*>(data);
					Size new_size(size().width() - pad_p->left() - pad_p->right(), size().height() - pad_p->top() - pad_p->bottom());
					m_widget->resize(new_size);
				}
				break;
			}

			default:
				Widget::update (type, data);
				break;
		}
	}

	void Frame::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(),
					 position().y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef DEBUG
		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glColor4f(1.0f, 0.0f, 0.0f, 0.45f);
		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
			glVertex2i(0, 0);
			glVertex2i(size().width(), 0);
			glVertex2i(size().width(), size().height());
			glVertex2i(0, size().height());
		glEnd();

		glDisable(GL_LINE_STIPPLE);
#endif

		glDisable(GL_BLEND);

		glPopMatrix();

		if(m_widget)
			dispatch_render(m_widget);
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

