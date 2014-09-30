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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/ScrollBar.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

#include <BlendInt/Gui/AbstractFrame.hpp>

namespace BlendInt {

	using Stock::Shaders;

	ScrollBar::ScrollBar (Orientation orientation)
	: AbstractSlider<int>(orientation),
	  m_last_value(0),
	  pressed_(false)
	{
		if (orientation == Horizontal) {
			m_slide.Resize(32, 14);
			set_size(200, 14);
		} else {
			m_slide.Resize(14, 32);
			set_size(14, 200);
		}

		set_round_type(RoundAll);

		InitOnce();
	}

	ScrollBar::~ScrollBar ()
	{
		glDeleteVertexArrays(2, vao_);
	}

	void ScrollBar::SetSliderPercentage (int percentage)
	{
		if (percentage < 0 || percentage > 100) return;

		if(orientation() == Horizontal) {

			int w = percentage * size().width() / 100;

			if(w < m_slide.size().width())
				w = m_slide.size().width();

			m_slide.Resize(w, m_slide.size().height());

			Refresh();

		} else {

			int h = percentage * size().height() / 100;

			if(h < m_slide.size().height())
				h = m_slide.size().height();

			m_slide.Resize(m_slide.size().width(), h);

			Refresh();

		}
	}

	int ScrollBar::GetSliderPercentage () const
	{
		int percentage = 0;

		if(orientation() == Horizontal) {
			percentage = m_slide.size().width() * 100 / size().width();
		} else {
			percentage = m_slide.size().height() * 100 / size().height();
		}

		return percentage;
	}

	bool ScrollBar::IsExpandX() const
	{
		if (orientation() == Horizontal) {
			return true;
		} else {
			return false;
		}

	}

	bool ScrollBar::IsExpandY() const
	{
		if(orientation() == Vertical) {
			return true;
		} else {
			return false;
		}
	}

	void ScrollBar::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			float radius = std::min(request.size()->width(), request.size()->height()) / 2.f;

			Orientation slot_orient;
			if (orientation() == Vertical) {
				slot_orient = Horizontal;
				m_slide.Resize(radius * 2 + 0.5f, m_slide.size().height());
				m_slide.SetRadius(radius);
			} else {
				slot_orient = Vertical;
				m_slide.Resize(m_slide.size().width(), radius * 2 + 0.5f);
				m_slide.SetRadius(radius);
			}

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;

			if(Theme::instance->scroll().shaded) {

				short shadetop = Theme::instance->scroll().shadetop;
				short shadedown = Theme::instance->scroll().shadedown;
				if(orientation() == Vertical) {
					shadetop = Theme::instance->scroll().shadedown;
					shadedown = Theme::instance->scroll().shadetop;
				}

				GenerateVertices(size(),
						DefaultBorderWidth(),
						round_type(),
						radius,
						slot_orient,
						shadetop,
						shadedown,
						&inner_verts,
						&outer_verts);
			} else {
				GenerateVertices(size(),
						DefaultBorderWidth(),
						round_type(),
						radius,
						&inner_verts,
						&outer_verts);
			}

			buffer_.bind(0);
			buffer_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

			buffer_.bind(1);
			buffer_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

			buffer_.reset();

		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void ScrollBar::PerformOrientationUpdate (Orientation orientation)
	{
	}

	void ScrollBar::PerformMinimumUpdate (int minimum)
	{
		if (value() < minimum) {
			set_value(minimum);
		}
	}

	void ScrollBar::PerformMaximumUpdate (int maximum)
	{
		if (value() > maximum) {
			set_value(maximum);
		}
	}

	void ScrollBar::PerformValueUpdate (int value)
	{
	}

	void ScrollBar::PerformStepUpdate (int step)
	{
	}

	ResponseType ScrollBar::Draw (Profile& profile)
	{
		Shaders::instance->widget_inner_program()->use();

		glUniform3f(Shaders::instance->location(Stock::WIDGET_INNER_POSITION),
		        0.f, 0.f, 0.f);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_INNER_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::WIDGET_INNER_ANTI_ALIAS), 0);

		glUniform4fv(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 1,
				Theme::instance->scroll().inner.data());

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		Shaders::instance->widget_outer_program()->use();

		glUniform3f(Shaders::instance->location(Stock::WIDGET_OUTER_POSITION),
		        0.f, 0.f, 0.f);
		glUniform4fv(Shaders::instance->location(Stock::WIDGET_OUTER_COLOR), 1,
		        Theme::instance->scroll().outline.data());

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		        GetOutlineVertices(round_type()) * 2 + 2);

		if (emboss()) {
			glUniform4f(Shaders::instance->location(Stock::WIDGET_OUTER_COLOR), 1.f,
			        1.f, 1.f, 0.16f);
			glUniform3f(Shaders::instance->location(Stock::WIDGET_OUTER_POSITION),
					0.f, 0.f - 1.f, 0.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
			        GetHalfOutlineVertices(round_type()) * 2);
		}

		glBindVertexArray(0);
		GLSLProgram::reset();

		glm::vec3 pos(0.f, 0.f, 0.f);

		if (orientation() == Horizontal) {
			pos.x += GetSlidePosition();
		} else {
			pos.y += GetSlidePosition();
		}

		m_slide.Draw(pos);
		return Accept;
	}

	void ScrollBar::MouseHoverOutEvent(const MouseEvent& event)
	{
		if(m_slide.highlight()) {
			m_slide.set_highlight(false);
			Refresh();
		}
	}

	ResponseType ScrollBar::MouseMoveEvent (const MouseEvent& event)
	{
		if (pressed_) {

			int new_value = value();

			// DO not fire if cursor is out of range, otherwise too many events
			if (GetNewValue(event.position(), &new_value)) {
				set_value(new_value);
				Refresh();
				fire_slider_moved_event(value());
			}

			return Accept;

		} else {
			if (CursorOnSlideIcon(event.local_position())) {
				m_slide.set_highlight(true);
				Refresh();
			} else {
				m_slide.set_highlight(false);
				Refresh();
			}

			return Accept;
		}
	}

	ResponseType ScrollBar::MousePressEvent (const MouseEvent& event)
	{
		if (CursorOnSlideIcon(event.local_position())) {

			event.frame()->SetCursorFollowedWidget(this);

			m_cursor_origin = event.position();
			m_last_value = value();
			pressed_ = true;
			fire_slider_pressed();

			return Accept;
		} else {
			return Ignore;
		}
	}

	ResponseType ScrollBar::MouseReleaseEvent (const MouseEvent& event)
	{
		if (pressed_) {

			event.frame()->SetCursorFollowedWidget(0);

			pressed_ = false;

			if (CursorOnSlideIcon(event.local_position())) {
				DBG_PRINT_MSG("%s", "hello");
				fire_slider_released();
			}

			Refresh();
		}

		return Accept;
	}

	void ScrollBar::InitOnce ()
	{
		glGenVertexArrays(2, vao_);

		Size slot_size = m_slide.size();
		float slot_radius;
		Orientation slot_orient;
		if(orientation() == Horizontal) {
			slot_size.set_width(size().width());
			slot_radius = m_slide.size().height() / 2.f;
			slot_orient = Vertical;
		} else {
			slot_size.set_height(size().height());
			slot_radius = m_slide.size().width() / 2.f;
			slot_orient = Horizontal;
		}

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if(Theme::instance->scroll().shaded) {

			short shadetop = Theme::instance->scroll().shadetop;
			short shadedown = Theme::instance->scroll().shadedown;
			if(orientation() == Vertical) {
				shadetop = Theme::instance->scroll().shadedown;
				shadedown = Theme::instance->scroll().shadetop;
			}

			GenerateVertices(slot_size,
					DefaultBorderWidth(),
					round_type(),
					slot_radius,
					slot_orient,
					shadetop,
					shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateVertices(slot_size,
					DefaultBorderWidth(),
					round_type(),
					slot_radius,
					&inner_verts,
					&outer_verts);
		}

		buffer_.generate();

		glBindVertexArray(vao_[0]);

		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_INNER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_INNER_COORD), 3, GL_FLOAT, GL_FALSE, 0,
		        BUFFER_OFFSET(0));

		glBindVertexArray(vao_[1]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_OUTER_COORD));
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		buffer_.reset();
	}

	int ScrollBar::GetSpace ()
	{
		int space = 0;

		if (orientation() == Horizontal) {
			space = size().width() - m_slide.size().width();
		} else {
			space = size().height() - m_slide.size().height();
		}

		return space;
	}

	int ScrollBar::GetSlidePosition ()
	{
		int pos = 0;

		if(orientation() == Horizontal) {
			pos = (value () - minimum()) * (size().width() - m_slide.size().width()) / (maximum() - minimum());
		} else {
			pos = (value () - minimum()) * (size().height() - m_slide.size().height()) / (maximum() - minimum());
			pos = size().height() - m_slide.size().height() - pos;
		}

		return pos;
	}

	bool ScrollBar::CursorOnSlideIcon (const Point& local_cursor)
	{
		int slide_pos = GetSlidePosition();

		int xmin, ymin, xmax, ymax;

		if(orientation() == Horizontal) {
			xmin = slide_pos;
			ymin = (size().height() - m_slide.size().height()) / 2;
			xmax = xmin + m_slide.size().width();
			ymax = ymin + m_slide.size().height();
		} else {
			xmin = (size().width() - m_slide.size().width()) / 2;
			ymin = slide_pos;
			xmax = xmin + m_slide.size().width();
			ymax = ymin + m_slide.size().height();
		}

		if(local_cursor.x() < xmin ||
			local_cursor.y() < ymin ||
			local_cursor.x() > xmax ||
			local_cursor.y() > ymax)
		{
			return false;
		}

		glm::vec2 center;
		glm::vec2 cursor_pos(local_cursor.x(), local_cursor.y());
		float distance = 0.f;

		if(orientation() == Horizontal) {

			if(local_cursor.x() < (xmin + m_slide.radius())) {

				center.x = xmin + m_slide.radius();
				center.y = (ymax - ymin) / 2 + ymin;

				distance = glm::distance(center, cursor_pos);

				return distance <= m_slide.radius() ? true : false;

			} else if (local_cursor.x() > (xmax - m_slide.radius())) {

				center.x = xmax - m_slide.radius();
				center.y = (ymax - ymin) / 2 + ymin;

				distance = glm::distance(center, cursor_pos);

				return distance <= m_slide.radius() ? true : false;

			} else {
				return true;
			}

		} else {

			if(local_cursor.y() < (ymin + m_slide.radius())) {

				center.x = (xmax - xmin) / 2 + xmin;
				center.y = ymin + m_slide.radius();

				distance = glm::distance(center, cursor_pos);

				return distance <= m_slide.radius() ? true : false;

			} else if (local_cursor.y() > (ymax - m_slide.radius())) {

				center.x = (xmax - xmin) / 2 + xmin;
				center.y = ymax - m_slide.radius();

				distance = glm::distance(center, cursor_pos);

				return distance <= m_slide.radius() ? true : false;

			} else {
				return true;
			}

		}

		return false;
	}

	bool ScrollBar::GetNewValue (const Point& cursor, int* vout)
	{
		bool ret = false;

		int offset = 0;
		int move_space = GetSpace();

		if(move_space == 0)
			return false;

		if (orientation() == Horizontal) {
			offset = cursor.x() - m_cursor_origin.x();
		} else {
			offset = m_cursor_origin.y() - cursor.y();
		}

		int val = m_last_value + (offset * (maximum() - minimum())) / move_space;

		if(val > maximum()) {
			*vout = maximum();
		} else if(val < minimum()) {
			*vout = minimum();
		} else {
			*vout = val;
			ret = true;
		}

		return ret;
	}

}
