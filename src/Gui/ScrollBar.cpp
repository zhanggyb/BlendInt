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

namespace BlendInt {

	using Stock::Shaders;

	ScrollBar::ScrollBar (Orientation orientation)
	: AbstractSlider<int>(orientation),
	  m_last_value(0),
	  m_pressed(false)
	{
		if (orientation == Horizontal) {
			m_slide.Resize(32, 14);
			set_size(200, 14);
		} else {
			m_slide.Resize(14, 32);
			set_size(14, 200);
		}

		InitOnce();
	}

	ScrollBar::~ScrollBar ()
	{
		glDeleteVertexArrays(2, m_vao);
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

			const Color& color = Theme::instance->scroll().inner;
			short shadetop = Theme::instance->scroll().shadetop;
			short shadedown = Theme::instance->scroll().shadedown;
			if (orientation() == Vertical) {
				shadetop = Theme::instance->scroll().shadedown;
				shadedown = Theme::instance->scroll().shadetop;
			}

			VertexTool tool;
			tool.GenerateVertices(*request.size(), DefaultBorderWidth(), round_type(), radius, color, slot_orient,
							shadetop, shadedown);
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_data(tool.outer_size(), tool.outer_data());

			set_size(*request.size());
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
		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->use();

		glUniform3f(Shaders::instance->triangle_uniform_position(),
		        (float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 0);

		glBindVertexArray(m_vao[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		glVertexAttrib4fv(Shaders::instance->triangle_attrib_color(),
		        Theme::instance->scroll().outline.data());
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 1);

		glBindVertexArray(m_vao[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		        GetOutlineVertices(round_type()) * 2 + 2);

		if (emboss()) {
			glVertexAttrib4f(Shaders::instance->triangle_attrib_color(), 1.f,
			        1.f, 1.f, 0.16f);
			glUniform3f(Shaders::instance->triangle_uniform_position(),
			        (float) position().x(), (float) position().y() - 1.f, 0.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
			        GetHalfOutlineVertices(round_type()) * 2);
		}

		glBindVertexArray(0);
		program->reset();

		glm::vec3 pos(position().x(), position().y(), 0.f);

		if (orientation() == Horizontal) {
			pos.x += GetSlidePosition();
		} else {
			pos.y += GetSlidePosition();
		}

		m_slide.Draw(pos);
		return Accept;
	}

	ResponseType ScrollBar::MouseMoveEvent (const MouseEvent& event)
	{
		if (m_pressed) {

			int new_value = value();

			// DO not fire if cursor is out of range, otherwise too many events
			if (GetNewValue(event.position(), &new_value)) {
				set_value(new_value);
				fire_slider_moved_event(value());
				Refresh();
			}

			return Accept;

		} else {
			if (CursorOnSlideIcon(event.position())) {
				m_slide.set_highlight(true);

				Refresh();

				return Accept;
			} else {
				m_slide.set_highlight(false);
				Refresh();

				return Accept;
			}
		}
	}

	ResponseType ScrollBar::MousePressEvent (const MouseEvent& event)
	{
		if (CursorOnSlideIcon(event.position())) {

			m_cursor_origin = event.position();
			m_last_value = value();
			m_pressed = true;
			fire_slider_pressed();

			return Accept;
		} else {
			return Ignore;
		}
	}

	ResponseType ScrollBar::MouseReleaseEvent (const MouseEvent& event)
	{
		if (m_pressed) {
			m_pressed = false;

			if (CursorOnSlideIcon(event.position())) {
				fire_slider_released();
			}

			Refresh();
		}

		return Accept;
	}

	void ScrollBar::InitOnce ()
	{
		set_round_type(RoundAll);

		glGenVertexArrays(2, m_vao);

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

		const Color& color = Theme::instance->scroll().inner;
		short shadetop = Theme::instance->scroll().shadetop;
		short shadedown = Theme::instance->scroll().shadedown;
		if(orientation() == Vertical) {
			shadetop = Theme::instance->scroll().shadedown;
			shadedown = Theme::instance->scroll().shadetop;
		}

		VertexTool tool;
		tool.GenerateVertices(slot_size,
						DefaultBorderWidth(),
						round_type(),
						slot_radius,
						color,
						slot_orient,
						shadetop,
						shadedown
						);

		glBindVertexArray(m_vao[0]);
		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6,
		        BUFFER_OFFSET(0));
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6,
		        BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(m_vao[1]);
		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

		GLArrayBuffer::reset();
		glBindVertexArray(0);
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

	ResponseType ScrollBar::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType ScrollBar::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType ScrollBar::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType ScrollBar::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType ScrollBar::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
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

	bool ScrollBar::CursorOnSlideIcon (const Point& cursor)
	{
		int slide_pos = GetSlidePosition();

		int xmin, ymin, xmax, ymax;

		if(orientation() == Horizontal) {
			xmin = position().x() + slide_pos;
			ymin = position().y() + (size().height() - m_slide.size().height()) / 2;
			xmax = xmin + m_slide.size().width();
			ymax = ymin + m_slide.size().height();
		} else {
			xmin = position().x() + (size().width() - m_slide.size().width()) / 2;
			ymin = position().y() + slide_pos;
			xmax = xmin + m_slide.size().width();
			ymax = ymin + m_slide.size().height();
		}

		if(cursor.x() < xmin ||
			cursor.y() < ymin ||
			cursor.x() > xmax ||
			cursor.y() > ymax)
		{
			return false;
		}

		glm::vec2 center;
		glm::vec2 cursor_pos(cursor.x(), cursor.y());
		float distance = 0.f;

		if(orientation() == Horizontal) {

			if(cursor.x() < (xmin + m_slide.radius())) {

				center.x = xmin + m_slide.radius();
				center.y = (ymax - ymin) / 2 + ymin;

				distance = glm::distance(center, cursor_pos);

				return distance <= m_slide.radius() ? true : false;

			} else if (cursor.x() > (xmax - m_slide.radius())) {

				center.x = xmax - m_slide.radius();
				center.y = (ymax - ymin) / 2 + ymin;

				distance = glm::distance(center, cursor_pos);

				return distance <= m_slide.radius() ? true : false;

			} else {
				return true;
			}

		} else {

			if(cursor.y() < (ymin + m_slide.radius())) {

				center.x = (xmax - xmin) / 2 + xmin;
				center.y = ymin + m_slide.radius();

				distance = glm::distance(center, cursor_pos);

				return distance <= m_slide.radius() ? true : false;

			} else if (cursor.y() > (ymax - m_slide.radius())) {

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
