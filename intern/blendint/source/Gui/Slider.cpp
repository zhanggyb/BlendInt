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

#include <BlendInt/Gui/Slider.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	Slider::Slider (Orientation orientation)
	: AbstractSlider<int>(orientation),
	  //vao_(0),
	  m_last_value(0),
	  m_pressed(false)
	{
		m_slide_icon.Resize(14, 14);

		if (orientation == Vertical) {
			set_size(18, 200);
		} else {
			set_size(200, 18);
		}

		InitOnce();
	}

	Slider::~Slider()
	{
		//glDeleteVertexArrays(1, &vao_);
	}

	bool Slider::IsExpandX() const
	{
		if (orientation() == Horizontal) {
			return true;
		} else {
			return false;
		}
	}

	bool Slider::IsExpandY() const
	{
		if(orientation() == Vertical) {
			return true;
		} else {
			return false;
		}
	}

	Size Slider::GetPreferredSize() const
	{
		Size prefer;

		if(orientation() == Horizontal) {
			prefer.set_width(200);
			prefer.set_height(18);
		} else {
			prefer.set_width(18);
			prefer.set_height(200);
		}

		return prefer;
	}

	void Slider::PerformOrientationUpdate (Orientation orientation)
	{
		/*
		m_line->bind();
		GLfloat* buf_p = (GLfloat*) m_line->map(GL_READ_WRITE);
		if (orientation == Horizontal) {
			*(buf_p + 0) = m_slide_icon.size().width() / 2;
			*(buf_p + 1) = size().height() / 2;
			*(buf_p + 2) = size().width() - m_slide_icon.size().width() / 2;
			*(buf_p + 3) = *(buf_p + 0);
		} else {
			*(buf_p + 0) = size().width() / 2;
			*(buf_p + 1) = m_slide_icon.size().height() / 2;
			*(buf_p + 2) = *(buf_p + 0);
			*(buf_p + 3) = size().height()
							- m_slide_icon.size().height() / 2;
		}
		m_line->unmap();
		m_line->reset();

		RequestRedraw();
		*/
	}

	void Slider::PerformMinimumUpdate (int minimum)
	{
		if (value() < minimum) {
			set_value(minimum);
		}
	}

	void Slider::PerformMaximumUpdate (int maximum)
	{
		if (value() > maximum) {
			set_value(maximum);
		}
	}

	void Slider::PerformValueUpdate (int value)
	{
	}

	void Slider::PerformStepUpdate (int step)
	{
	}

	ResponseType Slider::Draw (const Context* context)
	{
		using Stock::Shaders;

		// ----- draw line

		/*
		glBindVertexArray(vao_);
		RefPtr<GLSLProgram> program = Shaders::instance->line_program();
		program->Use();

		program->SetUniform3f("u_position", (float) position().x(), (float) position().y(), 0.f);
		program->SetUniform1i("u_gamma", 0);
		program->SetUniform1i("u_AA", 0);

		program->SetVertexAttrib4fv("a_color",
		        Theme::instance->scroll().outline.data());

		glEnableVertexAttribArray(0);
		m_line->bind();
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,	0,	BUFFER_OFFSET(0));
		glLineWidth(1.0);
		glDrawArrays(GL_LINES, 0, 2);
		m_line->reset();

		glDisableVertexAttribArray(0);
		program->reset();
		glBindVertexArray(0);
		*/

		// ----- end of draw line

		float x = position().x();
		float y = position().y();

		if (orientation() == Horizontal) {
			x += get_position();
			y += size().height() / 2.f - m_slide_icon.size().height() / 2.f;
		} else {
			x += size().width() / 2.f - m_slide_icon.size().width() / 2.f;
			y += get_position();
		}

		m_slide_icon.Draw(x, y);

		return Finish;
	}

	ResponseType Slider::MouseMoveEvent (const Context* context)
	{
		if(m_pressed) {

			int new_value = value();

			// DO not fire if cursor is out of range, otherwise too many events
			if(GetNewValue(context->cursor_position(), &new_value)) {
				set_value(new_value);
				fire_slider_moved_event(value());
				RequestRedraw();
			}

			return Finish;

		} else {
			if(CursorOnSlideIcon(context->cursor_position())) {

				//m_slide_icon.set_highlight(true);

				RequestRedraw();

				return Finish;
			} else {

				//m_slide_icon.set_highlight(false);
				RequestRedraw();
				return Ignore;
			}
		}
	}

	ResponseType Slider::MousePressEvent (const Context* context)
	{
		if(CursorOnSlideIcon(context->cursor_position())) {
			m_pressed = true;
			m_last_value = value();
			m_last_cursor = context->cursor_position();
			fire_slider_pressed();

			return Finish;
		} else {
			return Ignore;
		}
	}

	ResponseType Slider::MouseReleaseEvent (const Context* context)
	{
		if(m_pressed) {
			m_pressed = false;

			if(CursorOnSlideIcon(context->cursor_position())) {
				fire_slider_released();
			}

			RequestRedraw();
		}

		return Finish;
	}

	void Slider::InitOnce ()
	{
		/*
		glGenVertexArrays(1, &vao_);

		glBindVertexArray(vao_);

		m_line.reset(new GLArrayBuffer);
		m_line->generate();
		m_line->bind();

		GLfloat vertices[4];

		if(orientation() == Horizontal) {
			vertices[0] = m_slide_icon.size().width() / 2;
			vertices[1] = size().height() / 2;
			vertices[2] = size().width() - m_slide_icon.size().width() / 2;
			vertices[3] = vertices[1];
		} else {
			vertices[0] = size().width() / 2;
			vertices[1] = m_slide_icon.size().height() / 2;
			vertices[2] = vertices[0];
			vertices[3] = size().height() - m_slide_icon.size().height() / 2;
		}

		m_line->set_data (sizeof(vertices), vertices);

		m_line->reset();

		glBindVertexArray(0);
		*/
	}

	int Slider::GetSpace ()
	{
		int space = 0;

		if(orientation() == Horizontal) {
			space = size().width() - m_slide_icon.size().width();	// m_line_start.x() is the radius of m_switch
		} else {
			space = size().height() - m_slide_icon.size().height();	// m_line_start.y() is the radius of m_switch
		}

		return space;
	}

	bool Slider::CursorOnSlideIcon (const Point& cursor)
	{
		bool ret = false;

		glm::vec2 icon_center;	// slide switch center position

		int radius = 0;

		if(orientation() == Horizontal) {
			radius = m_slide_icon.size().width() / 2;
			icon_center.x = position().x() + radius + get_position();
			icon_center.y = position().y() + size().height() / 2;
		} else {
			radius = m_slide_icon.size().height() / 2;
			icon_center.x = position().x() + size().width() / 2;
			icon_center.y = position().y() + radius + get_position();
		}

		glm::vec2 cursor_pos (cursor.x(), cursor.y());
		float distance = glm::distance(icon_center, cursor_pos);

		if(orientation() == Horizontal && distance <= radius) {
			ret = true;
		} else if (orientation() == Vertical && distance <= radius) {
			ret = true;
		} else {
			ret = false;
		}

		return ret;
	}

	bool Slider::GetNewValue (const Point& cursor, int* vout)
	{
		bool ret = false;

		int offset = 0;
		int move_space = GetSpace();

		if(move_space == 0)
			return false;

		if(orientation() == Horizontal) {
			offset = cursor.x() - m_last_cursor.x();

		} else {
			offset = cursor.y() - m_last_cursor.y();
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
