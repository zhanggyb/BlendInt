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
#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

#include <iostream>

namespace BlendInt {

	Slider::Slider (Orientation orientation) :
			AbstractSlider(orientation), m_vao(0), m_last_value(0), m_pressed(false)
	{
		m_slide.Resize(14, 14);

		if (orientation == Vertical) {
			set_size(18, 200);
			set_expand_y(true);
		} else {
			set_size(200, 18);
			set_expand_x(true);
		}

		InitOnce();
	}

	Slider::~Slider()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	bool Slider::Update (const UpdateRequest& request)
	{
		if(request.source() == Predefined) {

			switch (request.type()) {
				case FormPosition: {
					// don't care position change
					return true;
				}

				case FormSize: {
					/*
					const Size* size_p = static_cast<const Size*>(request.data());

					int switch_radius = std::min(m_slide.size().width(),
					        m_slide.size().height()) / 2;
					*/

					return true;
				}

				case SliderPropertyValue: {

					return true;
				}

				case SliderPropertyMinimum: {

					const int* min_p = static_cast<const int*>(request.data());

					if(*min_p >= maximum())
						return false;

					if(value() < *min_p) {
						set_value(*min_p);
					}

					return true;
				}

				case SliderPropertyMaximum: {

					const int* max_p = static_cast<const int*>(request.data());

					if(*max_p <= minimum())
						return false;

					if(value() > *max_p) {
						set_value(*max_p);
					}

					return true;
				}

				case SliderPropertyOrientation: {
					const Orientation* orient_p = static_cast<const Orientation*>(request.data());
					glBindVertexArray(m_vao);
					m_line->Bind();

					GLfloat* buf_p = (GLfloat*)m_line->Map(GL_READ_WRITE);
					if(*orient_p == Horizontal) {
						*(buf_p + 0) = m_slide.size().width() / 2;
						*(buf_p + 1) = size().height() / 2;
						*(buf_p + 2) = size().width() - m_slide.size().width() / 2;
						*(buf_p + 3) = *(buf_p + 0);
					} else {
						*(buf_p + 0) = size().width() / 2;
						*(buf_p + 1) = m_slide.size().height() / 2;
						*(buf_p + 2) = *(buf_p + 0);
						*(buf_p + 3) = size().height() - m_slide.size().height() / 2;
					}
					m_line->Unmap();

					m_line->Reset();
					glBindVertexArray(0);
					Refresh();

					return true;
				}

				default:
					return true;
			}

		} else {
			return false;
		}
	}

	ResponseType Slider::Draw (const RedrawEvent& event)
	{
		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		// ----- draw line

		glBindVertexArray(m_vao);
		RefPtr<GLSLProgram> program = ShaderManager::instance->default_line_program();

		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));

		program->SetVertexAttrib4f(
						"Color",
						themes()->scroll.outline.r() / 255.f,
						themes()->scroll.outline.g() / 255.f,
						themes()->scroll.outline.b() / 255.f,
						themes()->scroll.outline.a() / 255.f
						);

		program->SetUniform1i("AA", 0);
		program->SetUniform1i("Gamma", 0);

		glEnableVertexAttribArray(0);

		m_line->Bind();
		glVertexAttribPointer(0, // attribute
							  2,		// number of elements per vertex, here (x,y)
							  GL_FLOAT,	// the type of each element
							  GL_FALSE,	// take our values as-is
							  0,		// no extra data between each position
							  BUFFER_OFFSET(0)	// the first element
							  );

		glLineWidth(1.0);
		glDrawArrays(GL_LINES, 0, 2);

		m_line->Reset();

		glDisableVertexAttribArray(0);

		program->Reset();

		glBindVertexArray(0);

		// ----- end of draw line

		glm::mat4 switch_mvp;

		if (orientation() == Horizontal) {
			// m_line_start.x() == switch_radius
			switch_mvp = glm::translate(mvp,
			        glm::vec3(get_position(),
			        			size().height() / 2 - m_slide.size().height() / 2,
			        			0.f));
		} else {
			// m_line_start.y() == switch_radius
			switch_mvp = glm::translate(mvp,
			        glm::vec3(size().width() / 2 - m_slide.size().width() / 2,
			                get_position(), 0.0));
		}

		m_slide.Draw(switch_mvp);

		return Accept;
	}

	ResponseType Slider::MouseMoveEvent (const MouseEvent& event)
	{
		if(m_pressed) {

			int new_value = value();

			// DO not fire if cursor is out of range, otherwise too many events
			if(GetNewValue(event.position(), &new_value)) {
				set_value(new_value);
				fire_slider_moved_event(value());
				Refresh();
			}

			return Accept;

		} else {
			if(CursorOnSlideIcon(event.position())) {
				m_slide.set_highlight(true);

				Refresh();

				return Accept;
			} else {
				m_slide.set_highlight(false);
				Refresh();
				return Ignore;
			}
		}
	}

	ResponseType Slider::MousePressEvent (const MouseEvent& event)
	{
		if(CursorOnSlideIcon(event.position())) {
			m_pressed = true;
			m_last_value = value();
			m_last_cursor = event.position();
			fire_slider_pressed();

			return Accept;
		} else {
			return Ignore;
		}
	}

	ResponseType Slider::MouseReleaseEvent (const MouseEvent& event)
	{
		if(m_pressed) {
			m_pressed = false;

			if(CursorOnSlideIcon(event.position())) {
				fire_slider_released();
			}

			Refresh();
		}

		return Accept;
	}

	void Slider::InitOnce ()
	{
		glGenVertexArrays(1, &m_vao);

		glBindVertexArray(m_vao);

		m_line.reset(new GLArrayBuffer);
		m_line->Generate();
		m_line->Bind();

		GLfloat vertices[4];

		if(orientation() == Horizontal) {
			vertices[0] = m_slide.size().width() / 2;
			vertices[1] = size().height() / 2;
			vertices[2] = size().width() - m_slide.size().width() / 2;
			vertices[3] = vertices[1];
		} else {
			vertices[0] = size().width() / 2;
			vertices[1] = m_slide.size().height() / 2;
			vertices[2] = vertices[0];
			vertices[3] = size().height() - m_slide.size().height() / 2;
		}

		m_line->SetData (sizeof(vertices), vertices);

		m_line->Reset();

		glBindVertexArray(0);

	}

	int Slider::GetSpace ()
	{
		int space = 0;

		if(orientation() == Horizontal) {
			space = size().width() - m_slide.size().width();	// m_line_start.x() is the radius of m_switch
		} else {
			space = size().height() - m_slide.size().height();	// m_line_start.y() is the radius of m_switch
		}

		return space;
	}

	bool Slider::CursorOnSlideIcon (const Point& cursor)
	{
		bool ret = false;

		glm::vec2 icon_center;	// slide switch center position

		int radius = 0;

		if(orientation() == Horizontal) {
			radius = m_slide.size().width() / 2;
			icon_center.x = position().x() + radius + get_position();
			icon_center.y = position().y() + size().height() / 2;
		} else {
			radius = m_slide.size().height() / 2;
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
