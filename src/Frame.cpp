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
		set_minimal_size(margin().left() + margin().right(),
		        margin().top() + margin().bottom());
		set_size(120, 80);
		set_preferred_size(120, 80);
	}

	Frame::Frame (AbstractWidget* parent)
			: Widget(parent), m_widget(0)
	{
		set_minimal_size(margin().left() + margin().right(),
		        margin().top() + margin().bottom());
		set_size(120, 80);
		set_preferred_size(120, 80);
	}

	Frame::~Frame ()
	{
		// TODO Auto-generated destructor stub
	}

	void Frame::set_margin (const Margin& margin)
	{
		if (m_margin.equal(margin))
			return;

		Update(FrameMargin, &margin);
		m_margin = margin;
	}

	void Frame::set_margin (int l, int r, int t, int b)
	{
		if (m_margin.equal(l, r, t, b))
			return;

		Margin new_margin(l, r, t, b);

		Update(FrameMargin, &new_margin);
		m_margin = new_margin;
	}

	void Frame::set_widget (AbstractWidget* widget)
	{
		if (widget && (widget != m_widget)) {
			delete m_widget;
			m_widget = widget;

			m_widget->SetPosition(position().x() + margin().left(),
			        position().y() + margin().bottom());
			m_widget->Resize(
			        size().width() - margin().left() - margin().right(),
			        size().height() - margin().top() - margin().bottom());

			bind(m_widget);
		}
	}

	void Frame::KeyPressEvent (KeyEvent* event)
	{
	}

	void Frame::ContextMenuPressEvent (ContextMenuEvent* event)
	{
	}

	void Frame::ContextMenuReleaseEvent (ContextMenuEvent* event)
	{
	}

	void Frame::MousePressEvent (MouseEvent* event)
	{
		if(m_widget)
			dispatch_mouse_press_event(m_widget, event);
	}

	void Frame::MouseReleaseEvent (MouseEvent* event)
	{
		if(m_widget)
			dispatch_mouse_release_event(m_widget, event);
	}

	void Frame::MouseMoveEvent (MouseEvent* event)
	{
		if(m_widget)
			dispatch_mouse_move_event(m_widget,event);
	}

	void Frame::Update (int type, const void* data)
	{
		switch (type) {

			case FormPosition: {
				if (m_widget) {
					const Point* pos_p = static_cast<const Point*>(data);
					int offset_x = pos_p->x() - position().x();
					int offset_y = pos_p->y() - position().y();
					SetPosition(m_widget, m_widget->position().x() + offset_x,
					        m_widget->position().y() + offset_y);
				}
				break;
			}

			case FormSize: {
				if (m_widget) {
					Size size = *(static_cast<const Size*>(data));
					size.add_width(-(margin().left() + margin().right()));
					size.add_height(-(margin().top() + margin().bottom()));
					m_widget->Resize(size);
				}
				break;
			}

			case FrameMargin: {
				if (m_widget) {
					const Margin* margin_p = static_cast<const Margin*>(data);
					Size new_size(
					        size().width() - margin_p->left()
					                - margin_p->right(),
					        size().height() - margin_p->top()
					                - margin_p->bottom());
					m_widget->Resize(new_size);
				}
				break;
			}

			default:
				Widget::Update(type, data);
				break;
		}
	}

	void Frame::Render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(), position().y(), z());

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

		if (m_widget)
			dispatch_render(m_widget);
	}

	bool Frame::contain_no_margin (const Coord2d& cursor)
	{
		if (cursor.x() < (position().x() + m_margin.left())
		        || cursor.y() < (position().y() + m_margin.bottom())
		        || cursor.x()
		                > (position().x() + size().width() - m_margin.right())
		        || cursor.y()
		                > (position().y() + size().height() - m_margin.top())) {
			return false;
		}

		return true;
	}

} /* namespace BlendInt */

