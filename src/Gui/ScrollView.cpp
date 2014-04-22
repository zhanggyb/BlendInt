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
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/ScrollView.hpp>

namespace BlendInt {

	ScrollView::ScrollView()
	: AbstractSingleContainer(), m_orientation(Horizontal | Vertical), m_move_status(false)
	{
		set_margin(0, 0, 0, 0);
		set_size(200, 160);
		set_preferred_size(200, 160);
		set_scissor_test(true);
	}

	ScrollView::~ScrollView ()
	{

	}

	void ScrollView::SetViewport (AbstractWidget* widget)
	{
		if (AddSubWidget(widget)) {
			SetSubWidgetPosition(widget, position().x() + margin().left(),
			        position().y() + margin().bottom());
			ResizeSubWidget(widget, size().width() - margin().left() - margin().right(),
			        size().height() - margin().top() - margin().bottom());
		}
	}

	void ScrollView::ResetViewportPosition()
	{
		if(!sub_widget()) return;

		AbstractWidget* p = sub_widget();

		if(p) {
			int w = size().width() - margin().left() - margin().right();
			int h = size().height() - margin().top() - margin().bottom();

			int x = position().x() + margin().left() + (w - static_cast<int>(p->size().width())) / 2;
			int y = position().y() + margin().bottom() + (h - static_cast<int>(p->size().height())) / 2;

			SetSubWidgetPosition(p, x, y);
		}
	}

	int BlendInt::ScrollView::GetHPercentage ()
	{
		int percentage = 0;

		if(sub_widget()) {
			AbstractWidget* p = sub_widget();

			unsigned int w = size().width() - margin().left() - margin().right();

			if (p->size().width() <= w) {
				percentage = 100;
			} else {
				percentage = w * 100 / p->size().width();
			}

		} else {

		}

		return percentage;
	}

	int BlendInt::ScrollView::GetVPercentage ()
	{
		int percentage = 0;

		if(sub_widget()) {
			AbstractWidget* p = sub_widget();

			unsigned int h = size().height() - margin().top() - margin().bottom();

			if (p->size().height() <= h) {
				percentage = 100;
			} else {
				percentage = h * 100 / p->size().height();
			}

		} else {

		}

		return percentage;
	}

	void ScrollView::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch (request.type()) {

				case FormSize: {

					break;
				}

				case FormPosition: {
					ResetViewportPosition();
					break;
				}

				case ContextRefresh: {

					Refresh();
					break;
				}

				default:
					break;
			}

		}
	}

	ResponseType ScrollView::Draw (const RedrawEvent& event)
	{
		//glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		//glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		return AcceptAndContinue;
	}

	ResponseType ScrollView::MousePressEvent (const MouseEvent& event)
	{
		if (!sub_widget()) {
			return Ignore;
		}

		AbstractWidget* p = sub_widget();

		if (event.button() == MouseButtonMiddle) {
			m_move_status = true;
			m_move_start_pos.set_x(event.position().x());
			m_move_start_pos.set_y(event.position().y());
			m_origin_pos = p->position();
		} else {
			//dispatch_mouse_press_event(m_viewport, event);
		}

		return Accept;
	}

	ResponseType ScrollView::MouseReleaseEvent(const MouseEvent& event)
	{
		if(m_move_status) {
			m_move_status = false;
			Refresh();
		}

		if(!sub_widget()) {
			return Ignore;
		}

		//AbstractWidget* p = sub_widgets()->front();

		//if(!m_viewport) return;
		//dispatch_mouse_release_event(m_viewport, event);

		return Accept;
	}

	ResponseType ScrollView::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType ScrollView::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType ScrollView::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ScrollView::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	void ScrollView::MoveViewport(int x, int y)
	{
		if(sub_widget()) {

			if(x != 0 || y != 0) {
				AbstractWidget* p = sub_widget();
				SetSubWidgetPosition(p, p->position().x() + x, p->position().y() + y);

				Refresh();
			}
		}
	}

	void ScrollView::SetReletivePosition (int x, int y)
	{
		if(sub_widget()) {
			AbstractWidget* p = sub_widget();

			SetSubWidgetPosition(p, position().x() + x, position().y() + y);

			Refresh();
		}
	}

	ResponseType ScrollView::MouseMoveEvent(const MouseEvent& event)
	{
		if(sub_widget()) {

			if(m_move_status) {

				AbstractWidget* p = sub_widget();

				SetSubWidgetPosition(p,
				        m_origin_pos.x() + event.position().x()
				                - m_move_start_pos.x(),
				        m_origin_pos.y() + event.position().y()
				                - m_move_start_pos.y());
				/*
				int w = size().width() - margin().left() - margin().right();
				int h = size().height() - margin().top() - margin().bottom();

				if (m_orientation & Horizontal) {

					if (w < static_cast<int>(p->size().width())) {
						int x_min = position().x() - (p->size().width() - w);
						int x_max = position().x();
						if (x_min > x_max)
							x_min = x_max;

						SetPosition(p, m_origin_pos.x() + event.position().x() - m_move_start_pos.x(),
								p->position().y());

						if (p->position().x() < x_min) {
							SetPosition(p, x_min, p->position().y());
						}

						if (p->position().x() > x_max) {
							SetPosition(p, x_max, p->position().y());
						}

					}
				}

				if (m_orientation & Vertical) {

					if (h < static_cast<int>(p->size().height())) {
						int y_min = position().y() - (p->size().height() - h);
						int y_max = position().y();

						if (y_min > y_max)
							y_min = y_max;

						SetPosition(p, p->position().x(), m_origin_pos.y() + event.position().y() - m_move_start_pos.y());

						if (p->position().y() < y_min) {

							SetPosition(p, p->position().x(),
							        y_min);
						}

						if (p->position().y() > y_max) {
							SetPosition(p, p->position().x(),
							        y_max);
						}
					}
				}
				*/

				Refresh();
				return Accept;
			}


		} else {
			return Ignore;
		}

		return Accept;
	}

}
