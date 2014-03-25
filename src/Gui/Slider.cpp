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
			AbstractSlider(orientation), m_vao(0), m_line_width(0), m_pressed(false)
	{
		m_switch.Resize(14, 14);

		if (orientation == Vertical) {
			set_size(18, 200);
			set_expand_y(true);

			m_line_start.set_x(18 / 2);
			m_line_start.set_y(7);
		} else {
			set_size(200, 18);
			set_expand_x(true);

			m_line_start.set_x(7);
			m_line_start.set_y(18 / 2);
		}

		m_line_width = 200 - 7 * 2;

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
					const Size* size_p = static_cast<const Size*>(request.data());

					int switch_radius = std::min(m_switch.size().width(),
					        m_switch.size().height()) / 2;

					if (orientation() == Vertical) {
						m_line_start.set_x(size_p->width() / 2);
						m_line_start.set_y(switch_radius);
						m_line_width = size_p->height() - switch_radius * 2;
					} else {
						m_line_start.set_x(switch_radius);
						m_line_start.set_y(size_p->height() / 2);
						m_line_width = size_p->width() - switch_radius * 2;
					}
					return true;
				}

				case SliderPropertyValue: {

					return true;
				}

				case SliderPropertyOrientation: {
					const Orientation* orient_p = static_cast<const Orientation*>(request.data());
					glBindVertexArray(m_vao);
					m_line->Bind();

					GLfloat* buf_p = (GLfloat*)m_line->Map(GL_READ_WRITE);
					if(*orient_p == Horizontal) {
						*(buf_p + 2) = *(buf_p + 2) + m_line_width;
						*(buf_p + 3) = static_cast<float>(m_line_start.y());
					} else {
						*(buf_p + 2) = static_cast<float>(m_line_start.x());
						*(buf_p + 3) = *(buf_p + 3) + m_line_width;
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
		RefPtr<GLSLProgram> program = ShaderManager::instance->default_form_program();

		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));

		program->SetVertexAttrib4f(
						"color",
						themes()->scroll.outline.r() / 255.f,
						themes()->scroll.outline.g() / 255.f,
						themes()->scroll.outline.b() / 255.f,
						themes()->scroll.outline.a() / 255.f
						);

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
			                (float) (m_line_start.y() - m_line_start.x()),
			                0.0));
		} else {
			// m_line_start.y() == switch_radius
			switch_mvp = glm::translate(mvp,
			        glm::vec3((float) (m_line_start.x() - m_line_start.y()),
			                get_position(), 0.0));
		}

		m_switch.Draw(switch_mvp);

		return Accept;
	}

	void Slider::MouseMoveEvent (MouseEvent* event)
	{
		if(m_pressed) {

			int new_value = value();

			// DO not fire if cursor is out of range, otherwise too many events
			if(GetNewValue(event->position(), &new_value)) {
				set_value(new_value);
				fire_slider_moved_event(value());
				Refresh();
			}

			event->accept(this);

		} else {
			if(CursorOnSlideIcon(event->position())) {
				m_switch.set_highlight(true);

				Refresh();
				event->accept(this);
			} else {
				m_switch.set_highlight(false);
				Refresh();
				event->ignore(this);
			}
		}
	}

	void Slider::MousePressEvent (MouseEvent* event)
	{
		if(CursorOnSlideIcon(event->position())) {
			m_pressed = true;
			fire_slider_pressed();
			event->accept(this);
		} else {
			event->ignore(this);
		}
	}

	void Slider::MouseReleaseEvent (MouseEvent* event)
	{
		if(m_pressed) {
			m_pressed = false;

			if(CursorOnSlideIcon(event->position())) {
				fire_slider_released();
			}

			Refresh();
		}

		event->accept(this);
	}

	void Slider::InitOnce ()
	{
		glGenVertexArrays(1, &m_vao);

		glBindVertexArray(m_vao);

		m_line.reset(new GLArrayBuffer);
		m_line->Generate();
		m_line->Bind();

		GLfloat vertices[] = {
					static_cast<float>(m_line_start.x()), static_cast<float>(m_line_start.y()),
					static_cast<float>(m_line_start.x()), static_cast<float>(m_line_start.y())
		};

		if(orientation() == Horizontal) {
			vertices[2] = vertices[2] + m_line_width;
		} else {
			vertices[3] = vertices[3] + m_line_width;
		}

		m_line->SetData (sizeof(vertices), vertices);

		m_line->Reset();

		glBindVertexArray(0);

	}

	int Slider::GetSpace ()
	{
		int space = 0;

		if(orientation() == Horizontal) {
			space = size().width() - m_line_start.x() * 2;	// m_line_start.x() is the radius of m_switch
		} else {
			space = size().height() - m_line_start.y() * 2;	// m_line_start.y() is the radius of m_switch
		}

		return space;
	}

	bool Slider::CursorOnSlideIcon (const Point& cursor)
	{
		bool ret = false;

		glm::vec2 icon_center;	// slide switch center position

		if(orientation() == Horizontal) {
			icon_center.x = position().x() + m_line_start.x() + get_position();
			icon_center.y = position().y() + m_line_start.y();
		} else {
			icon_center.x = position().x() + m_line_start.x();
			icon_center.y = position().y() + m_line_start.y() + get_position();
		}

		glm::vec2 cursor_pos (cursor.x(), cursor.y());
		float distance = glm::distance(icon_center, cursor_pos);

		if(orientation() == Horizontal && distance <= m_line_start.x()) {
			ret = true;
		} else if (orientation() == Vertical && distance <= m_line_start.y()) {
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
		if(orientation() == Horizontal) {
			offset = cursor.x() - position().x() - m_line_start.x();

		} else {
			offset = cursor.y() - position().y() - m_line_start.y();
		}

		if(offset < 0) {
			*vout = minimum();
		} else if(offset > m_line_width) {
			*vout = maximum();
		} else {
			*vout = (offset * (maximum() - minimum())) / m_line_width;
			ret = true;
		}

		return ret;
	}

}
