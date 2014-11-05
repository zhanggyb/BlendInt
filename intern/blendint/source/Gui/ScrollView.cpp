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

	using Stock::Shaders;

	ScrollView::ScrollView()
	: Layout(),
	  m_vao(0),
	  m_orientation(Horizontal | Vertical),
	  m_move_status(false)
	{
		set_margin(0, 0, 0, 0);
		set_size(400, 300);

		glGenVertexArrays(1, &m_vao);

		VertexTool tool;
		tool.GenerateVertices(size(), 0, RoundNone, 0.f);

		glBindVertexArray(m_vao);
		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COORD), 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

	ScrollView::~ScrollView ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void ScrollView::Setup (AbstractWidget* widget)
	{
		if(widget == 0) return;

		if(widget->parent() == this)
			return;

		if(first_child()) {
			ClearSubWidgets();
		}

		if (PushBackSubWidget(widget)) {
			int x = position().x() + margin().left();
			int y = position().y() + size().height() - margin().top();

			// move widget to align the top-left of the viewport
			SetSubWidgetPosition(widget, x, y - widget->size().height());

			/*
			ResizeSubWidget(widget,
							size().width() - horizontal_margins(),
							size().height() - vertical_margins());
			*/


		}
	}

	void ScrollView::CentralizeViewport()
	{
		if(first_child() == 0) return;

		AbstractWidget* p = first_child();

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

		if(first_child()) {
			AbstractWidget* p = first_child();

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

		if(first_child()) {
			AbstractWidget* p = first_child();

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
		if(first_child()) {

			if(x != 0 || y != 0) {
				AbstractWidget* p = first_child();
				SetSubWidgetPosition(p, p->position().x() + x, p->position().y() + y);

				Refresh();
			}
		}
	}

	void ScrollView::SetReletivePosition (int x, int y)
	{
		if(first_child()) {
			AbstractWidget* p = first_child();

			SetSubWidgetPosition(p, position().x() + x, position().y() + y);

			Refresh();
		}
	}

	bool ScrollView::IsExpandX() const
	{
		if(first_child()) {
			return first_child()->IsExpandX();
		} else {
			return false;
		}
	}

	bool ScrollView::IsExpandY() const
	{
		if(first_child()) {
			return first_child()->IsExpandY();
		} else {
			return false;
		}
	}

	Size ScrollView::GetPreferredSize() const
	{
		Size prefer(400, 300);

		AbstractWidget* widget = first_child();

		if(widget) {
			prefer = widget->GetPreferredSize();
			prefer.add_width(margin().hsum());
			prefer.add_height(margin().vsum());
		}

		return prefer;
	}

	void ScrollView::PerformMarginUpdate(const Margin& request)
	{
		// nothing to do
	}

	void ScrollView::PerformPositionUpdate (
	        const PositionUpdateRequest& request)
	{
		if (request.target() == this) {
			int x = request.position()->x() - position().x();
			int y = request.position()->y() - position().y();

			set_position(*request.position());

			if(first_child()) {
				MoveSubWidgets(x, y);
			}
		}

		ReportPositionUpdate(request);
	}

	void ScrollView::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(*request.size(), 0, RoundNone, 0);
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());

			// align the subwidget
			if (first_child()) {

				int dy = request.size()->height() - size().height();

				first_child()->SetPosition(first_child()->position().x(),
				        first_child()->position().y() + dy);
			}

			set_size(*request.size());
		}

		ReportSizeUpdate(request);
	}

	ResponseType ScrollView::Draw (Profile& profile)
	{
		using Stock::Shaders;

		RefPtr<GLSLProgram> program = Shaders::instance->widget_triangle_program();
		program->use();

		glUniform2f(Shaders::instance->location(Stock::WIDGET_TRIANGLE_POSITION), (float) position().x(), (float) position().y());
		glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_TRIANGLE_ANTI_ALIAS), 0);

		if(first_child()) {
			glVertexAttrib4f(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COLOR), 0.208f, 0.208f, 0.208f, 1.0f);
		} else {
			glVertexAttrib4f(Shaders::instance->location(Stock::WIDGET_TRIANGLE_COLOR), 0.447f, 0.447f, 0.447f, 1.0f);
		}

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);

		program->reset();

		return Ignore;
	}

	ResponseType ScrollView::MousePressEvent (const MouseEvent& event)
	{
		if (!first_child()) {
			return Ignore;
		}

		AbstractWidget* p = first_child();

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

		if(!first_child()) {
			return Ignore;
		}

		//AbstractWidget* p = sub_widgets()->front();

		//if(!m_viewport) return;
		//dispatch_mouse_release_event(m_viewport, event);

		return Ignore;
	}

	ResponseType ScrollView::MouseMoveEvent(const MouseEvent& event)
	{
		if(first_child()) {

			if(m_move_status) {

				AbstractWidget* p = first_child();

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
