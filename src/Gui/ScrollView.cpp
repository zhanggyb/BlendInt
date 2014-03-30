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
	: AbstractContainer(), m_orientation(Horizontal | Vertical), m_move_status(false)
	{
		set_size(200, 160);
		set_preferred_size(200, 160);

		m_checkerboard.reset(new CheckerBoard(20));
		m_checkerboard->Resize(size());
	}

	ScrollView::~ScrollView ()
	{

	}

	void ScrollView::reset_viewport_position()
	{
		if(!sub_widget_size()) return;

		AbstractWidget* p = sub_widgets().front();

		if(p) {
			int w = size().width();
			int h = size().height();

			int x = position().x() + (w - static_cast<int>(p->size().width())) / 2;
			int y = position().y() + (h - static_cast<int>(p->size().height())) / 2;

			p->SetPosition(x, y);
		}
	}

	bool ScrollView::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch (request.type()) {

				case FormSize: {

					const Size* size_p = static_cast<const Size*>(request.data());

					m_checkerboard->Resize(*size_p);

					return true;
				}

				case FormPosition: {
					reset_viewport_position();
					return true;
				}

				default:
					return true;
			}

		} else {
			return false;
		}
	}

	ResponseType ScrollView::Draw (const RedrawEvent& event)
	{
		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		m_checkerboard->Draw(mvp);

		StackScissorTestRect(
		        Rect(position().x() + margin().left(),
		                position().y() + margin().bottom(),
		                size().width() - margin().left() - margin().right(),
		                size().height() - margin().top() - margin().bottom()));

		return AcceptAndContinue;
	}

	ResponseType ScrollView::MousePressEvent (const MouseEvent& event)
	{
		if (!sub_widget_size()) {
			return Ignore;
		}

		AbstractWidget* p = sub_widgets().front();

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
		if(m_move_status) m_move_status = false;

		if(!sub_widget_size()) {
			return Ignore;
		}

		//AbstractWidget* p = sub_widgets().front();

		//if(!m_viewport) return;
		//dispatch_mouse_release_event(m_viewport, event);

		return Accept;
	}

	void ScrollView::SetViewport (AbstractWidget* widget)
	{
		if(sub_widgets().size() >= 1) {
			ClearSubWidgets();
		}

		if (AddSubWidget(widget)) {
			// set preferred size
			SetPosition(widget, position().x() + margin().left(),
			        position().y() + margin().bottom());
		}
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

	ResponseType ScrollView::MouseMoveEvent(const MouseEvent& event)
	{
		if(sub_widget_size()) {

			if(m_move_status) {

				int w = size().width();
				int h = size().height();

				if (m_orientation & Horizontal) {

					if (w < static_cast<int>(sub_widgets().front()->size().width())) {
						int x_min = position().x() - (sub_widgets().front()->size().width() - w);
						int x_max = position().x();
						if (x_min > x_max)
							x_min = x_max;

						SetPosition(sub_widgets().front(), m_origin_pos.x() + event.position().x() - m_move_start_pos.x(),
								sub_widgets().front()->position().y());

						if (sub_widgets().front()->position().x() < x_min) {
							SetPosition(sub_widgets().front(), x_min, sub_widgets().front()->position().y());
						}

						if (sub_widgets().front()->position().x() > x_max) {
							SetPosition(sub_widgets().front(), x_max, sub_widgets().front()->position().y());
						}

					}
				}

				if (m_orientation & Vertical) {

					if (h < static_cast<int>(sub_widgets().front()->size().height())) {
						int y_min = position().y() - (sub_widgets().front()->size().height() - h);
						int y_max = position().y();

						if (y_min > y_max)
							y_min = y_max;

						SetPosition(sub_widgets().front(), sub_widgets().front()->position().x(), m_origin_pos.y() + event.position().y() - m_move_start_pos.y());

						if (sub_widgets().front()->position().y() < y_min) {

							SetPosition(sub_widgets().front(), sub_widgets().front()->position().x(),
							        y_min);
						}

						if (sub_widgets().front()->position().y() > y_max) {
							SetPosition(sub_widgets().front(), sub_widgets().front()->position().x(),
							        y_max);
						}
					}
				}

				Refresh();
				return Accept;
			}


		} else {
			return Ignore;
		}

		/*
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

		*/

		return Accept;
	}

}

