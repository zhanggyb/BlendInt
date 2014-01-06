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

#ifdef __UNIX__
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <iostream>

#include <BlendInt/ScrollView.hpp>

namespace BlendInt {

	ScrollView::ScrollView()
	: Widget(), m_orientation(Horizontal | Vertical), m_move_status(false), m_viewport(0)
	{
		Resize(200, 160);
		SetPreferredSize(200, 160);
	}

	ScrollView::ScrollView(AbstractWidget* parent)
	: Widget(parent), m_orientation(Horizontal | Vertical), m_move_status(false), m_viewport(0)
	{
		Resize(200, 160);
		SetPreferredSize(200, 160);
	}

	void ScrollView::set_viewport(AbstractWidget* viewport)
	{
		if(!viewport) return;

		if(m_viewport) {
			delete m_viewport;
		}

		m_viewport = viewport;
		Attach (m_viewport);
		reset_viewport_position();
	}

	void ScrollView::reset_viewport_position()
	{
		if(m_viewport) {
			int w = size().width();
			int h = size().height();

			int x = position().x() + (w - static_cast<int>(m_viewport->size().width())) / 2;
			int y = position().y() + (h - static_cast<int>(m_viewport->size().height())) / 2;

			m_viewport->SetPosition(x, y);
		}
	}

	void ScrollView::Update (int type, const void* data)
	{
		switch(type) {

			case FormPosition: {
				reset_viewport_position();
				break;
			}

			default:
				break;
		}

	}

	void ScrollView::Draw ()
	{
		glEnable (GL_SCISSOR_TEST);
		glScissor (position().x(),
				position().y(),
				size().width(),
				size().height());

		// test code
//		glEnable(GL_BLEND);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//		glColor4f(0.2f, 0.05f, 0.2f, 0.5f);
//		glBegin(GL_POLYGON);
//			glVertex2d(50,50);
//			glVertex2d(1200,50);
//			glVertex2d(1200,1200);
//			glVertex2d(50,1200);
//		glEnd();
//
//		glDisable(GL_BLEND);

		if(m_viewport) DispatchRender(m_viewport);

		glDisable(GL_SCISSOR_TEST);

#ifdef DEBUG
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(), position().y(), z());

		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
		glVertex2i(0, 0);
		glVertex2i(size().width(), 0);
		glVertex2i(size().width(), size().height());
		glVertex2i(0, size().height());
		glEnd();

		glDisable(GL_LINE_STIPPLE);

		glPopMatrix();
#endif
	}

	void ScrollView::MousePressEvent(MouseEvent* event)
	{
		if(contain(event->position())) {

			if(!m_viewport) return;

			if (event->button() == MouseButtonMiddle) {

				m_move_status = true;
				m_move_start_pos.set_x(event->position().x());
				m_move_start_pos.set_y(event->position().y());
				m_origin_pos = m_viewport->position();
			} else {
				dispatch_mouse_press_event(m_viewport, event);
			}

		}
	}

	void ScrollView::MouseReleaseEvent(MouseEvent* event)
	{
		if(m_move_status) m_move_status = false;

		if(!m_viewport) return;
		dispatch_mouse_release_event(m_viewport, event);
	}

	void ScrollView::MouseMoveEvent(MouseEvent* event)
	{
		if(!m_viewport) return;

		if(m_move_status) {

			int w = size().width();
			int h = size().height();

			if (m_orientation & Horizontal) {

				if (w < static_cast<int>(m_viewport->size().width())) {
					int x_min = position().x() - (m_viewport->size().width() - w);
					int x_max = position().x();
					if (x_min > x_max)
						x_min = x_max;

					m_viewport->SetPosition(
					        m_origin_pos.x() + event->position().x()
					                - m_move_start_pos.x(),
					        m_viewport->position().y());

					if (m_viewport->position().x() < x_min) {
						m_viewport->SetPosition(x_min,
						        m_viewport->position().y());
					}

					if (m_viewport->position().x() > x_max) {
						m_viewport->SetPosition(x_max,
						        m_viewport->position().y());
					}

				}
			}

			if (m_orientation & Vertical) {

				if (h < static_cast<int>(m_viewport->size().height())) {
					int y_min = position().y() - (m_viewport->size().height() - h);
					int y_max = position().y();

					if (y_min > y_max)
						y_min = y_max;

					m_viewport->SetPosition(m_viewport->position().x(),
					        m_origin_pos.y() + event->position().y()
					                - m_move_start_pos.y());


					if (m_viewport->position().y() < y_min) {
						m_viewport->SetPosition(m_viewport->position().x(),
						        y_min);
					}

					if (m_viewport->position().y() > y_max) {
						m_viewport->SetPosition(m_viewport->position().x(),
						        y_max);
					}
				}
			}

			return;
		}

		if(contain(event->position())) {
			dispatch_mouse_move_event(m_viewport, event);
		}
	}

}
