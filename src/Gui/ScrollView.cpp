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

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/ScrollView.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	ScrollView::ScrollView()
	: AbstractSingleContainer(),
	  m_vao(0),
	  m_orientation(Horizontal | Vertical),
	  m_move_status(false)
	{
		set_margin(0, 0, 0, 0);
		set_size(400, 300);
		set_scissor_test(true);

		glGenVertexArrays(1, &m_vao);

		VertexTool tool;
		tool.Setup(size(), DefaultBorderWidth(), round_corner_type(), round_corner_radius());

		glBindVertexArray(m_vao);
		m_inner.reset(new GLArrayBuffer);
		m_inner->Generate();
		m_inner->Bind();
		tool.SetInnerBufferData(m_inner.get());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::Reset();
	}

	ScrollView::~ScrollView ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void ScrollView::SetViewport (AbstractWidget* widget)
	{
		if (SetSubWidget(widget)) {
			int x = position().x() + margin().left();
			int y = position().y() + margin().bottom();

			y = y + size().height() - margin().vsum() - widget->size().height();

			SetSubWidgetPosition(widget, x, y);
			/*
			ResizeSubWidget(widget,
							size().width() - horizontal_margins(),
							size().height() - vertical_margins());
			*/
		}
	}

	void ScrollView::CentralizeViewport()
	{
		if(!sub_widget()) return;

		AbstractWidget* p = sub_widget();

		int w = size().width() - margin().hsum();
		int h = size().height() - margin().vsum();

		int x = position().x() + margin().left();
		x += (w - static_cast<int>(p->size().width())) / 2;
		int y = position().y() + margin().bottom();
		y += (h - static_cast<int>(p->size().height())) / 2;

		SetSubWidgetPosition(p, x, y);
	}

	int BlendInt::ScrollView::GetHPercentage () const
	{
		int percentage = 0;

		if(sub_widget()) {
			AbstractWidget* p = sub_widget();

			int w = size().width() - margin().hsum();

			if (p->size().width() <= w) {
				percentage = 100;
			} else {
				percentage = w * 100 / p->size().width();
			}

		}

		return percentage;
	}

	int BlendInt::ScrollView::GetVPercentage () const
	{
		int percentage = 0;

		if(sub_widget()) {
			AbstractWidget* p = sub_widget();

			int h = size().height() - margin().vsum();

			if (p->size().height() <= h) {
				percentage = 100;
			} else {
				percentage = h * 100 / p->size().height();
			}

		}

		return percentage;
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

	bool ScrollView::IsExpandX() const
	{
		if(sub_widget()) {
			return sub_widget()->IsExpandX();
		} else {
			return false;
		}
	}

	bool ScrollView::IsExpandY() const
	{
		if(sub_widget()) {
			return sub_widget()->IsExpandY();
		} else {
			return false;
		}
	}

	Size ScrollView::GetPreferredSize() const
	{
		Size prefer(400, 300);

		if(sub_widget()) {
			prefer = sub_widget()->GetPreferredSize();
			prefer.add_width(margin().hsum());
			prefer.add_height(margin().vsum());
		}

		return prefer;
	}

	void ScrollView::UpdateContainer(const ContainerUpdateRequest& request)
	{
		switch (request.type()) {

			// TODO: update margin

			default: {
				ReportContainerUpdate(request);
				break;
			}
		}

	}

	bool ScrollView::UpdateGeometryTest (const GeometryUpdateRequest& request)
	{
		if(request.source() == this) {
			return true;
		} else if (request.source() == container()) {
			return true;
		} else {	// called by sub widget

			switch (request.type()) {

				case WidgetPosition:
					return true;

				case WidgetSize:
					return true;

				default:
					return false;
			}

		}
	}

	void ScrollView::UpdateGeometry (const GeometryUpdateRequest& request)
	{
		if (request.target() == this) {
			switch (request.type()) {

				case WidgetSize: {

					const Size* size_p = static_cast<const Size*>(request.data());
					VertexTool tool;
					tool.Setup(*size_p, 0, RoundNone, 0);
					m_inner->Bind();
					tool.SetInnerBufferData(m_inner.get());

					set_size(*size_p);
					break;
				}

				case WidgetPosition: {

					if (sub_widget()) {
						const Point* pos_p =
										static_cast<const Point*>(request.data());
						int x = pos_p->x() - position().x();
						int y = pos_p->y() - position().y();

						set_position(*pos_p);
						MoveSubWidget(x, y);
					}

					break;
				}

				default:
					break;
			}
		}

		ReportGeometryUpdate(request);
	}

	ResponseType ScrollView::Draw (const RedrawEvent& event)
	{
		using Stock::Shaders;

		glm::vec3 pos((float)position().x(), (float)position().y(), 0.f);
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		RefPtr<GLSLProgram> program = Shaders::instance->default_triangle_program();
		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetVertexAttrib4f("Color", 0.208f, 0.208f, 0.208f, 1.0f);
		program->SetUniform1i("Gamma", 0);
		program->SetUniform1i("AA", 0);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
						GetOutlineVertices(round_corner_type()) + 2);
		glBindVertexArray(0);

		program->Reset();

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

		return Ignore;
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

		return Ignore;
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
				return Ignore;
			}


		} else {
			return Ignore;
		}

		return Ignore;
	}

}
