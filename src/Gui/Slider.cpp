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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/Slider.hpp>
#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

#include <iostream>

namespace BlendInt {

	Slider::Slider (Orientation orientation) :
					AbstractSlider(orientation), m_line_width(0)
	{
		int switch_radius = std::min(m_switch.size().width(), m_switch.size().height()) / 2;

		if (orientation == Vertical) {
			set_size(18, 200);
			set_expand_y(true);

			m_line_start.set_x(18 / 2);
			m_line_start.set_y(switch_radius);
		} else {
			set_size(200, 18);
			set_expand_x(true);

			m_line_start.set_x(switch_radius);
			m_line_start.set_y(18 / 2);
		}

		m_line_width = 200 - switch_radius * 2;

	}

	Slider::~Slider()
	{
	}

	bool Slider::Update (int type, const void* data)
	{
		switch (type) {
			case FormPosition: {

				// don't care position

//				const Point* pos = static_cast<const Point*>(data);
//				m_slide_button->SetPosition(m_slide_button->position().x() + (pos->x() - position().x()),
//						m_slide_button->position().y() + (pos->y() - position().y()));
				return true;
			}

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(data);

				int switch_radius = std::min(m_switch.size().width(), m_switch.size().height()) / 2;

				if (orientation() == Vertical) {
					m_line_start.set_x(size_p->width() / 2);
					m_line_start.set_y(switch_radius);
					m_line_width = size_p->height() - switch_radius * 2;
				} else {
					m_line_start.set_x(switch_radius);
					m_line_start.set_y(size_p->height() / 2);
					m_line_width = size_p->width() - switch_radius * 2;
				}

				//unsigned int button_size = static_cast<unsigned int>(std::min(size().width(),
				//		size().height()));

				//m_switch->Resize(button_size, button_size);

				if(orientation() == Vertical) {
					//m_slide_button->SetPosition (m_slide_button->position().x(),
							//position().y() + value() * get_space() / (float)(maximum() - minimum()));
				} else {
					//m_slide_button->SetPosition (position().x() + value() * get_space() / (float)(maximum() - minimum()),
							//m_slide_button->position().y());
				}

				return true;
			}

			case SliderPropertyValue: {

				if(orientation() == Vertical) {
					//m_slide_button->SetPosition (m_slide_button->position().x(),
						//	position().y() + value() * get_space() / (float)(maximum() - minimum()));
				} else {
					//m_slide_button->SetPosition (position().x() + value() * get_space() / (float)(maximum() - minimum()),
					//		m_slide_button->position().y());
				}

				return true;
			}

			default:
				return true;
		}
	}

	void Slider::Draw (RedrawEvent* event)
	{
		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event->pv_matrix(), pos);

		// ----- draw line

		RefPtr<GLSLProgram> program = ShaderManager::instance->default_form_program();

		program->Use();

		GLint xy_attrib = program->GetAttributeLocation("xy");

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));

		float vertices[2][2] = {
						{static_cast<float>(m_line_start.x()), static_cast<float>(m_line_start.y())},
						{static_cast<float>(m_line_start.x()), static_cast<float>(m_line_start.y())} };

		if(orientation() == Horizontal) {
			vertices[1][0] = vertices[1][0] + m_line_width;
		} else {
			vertices[1][1] = vertices[1][1] + m_line_width;
		}

		program->SetVertexAttrib4f(
						"color",
						themes()->scroll.outline.r() / 255.f,
						themes()->scroll.outline.g() / 255.f,
						themes()->scroll.outline.b() / 255.f,
						themes()->scroll.outline.a() / 255.f
						);

		glEnableVertexAttribArray(xy_attrib);

		glVertexAttribPointer(xy_attrib, // attribute
							  2,		// number of elements per vertex, here (x,y)
							  GL_FLOAT,	// the type of each element
							  GL_FALSE,	// take our values as-is
							  0,		// no extra data between each position
							  vertices	// the first element
							  );

		glDrawArrays(GL_LINES, 0, 2);

		glDisableVertexAttribArray(xy_attrib);

		program->Reset();

		// ----- end of draw line

		glm::mat4 switch_mvp;

		if (orientation() == Horizontal) {
			// m_line_start.x() == switch_radius
			switch_mvp = glm::translate(mvp, glm::vec3(0.0, (float)(m_line_start.y() - m_line_start.x()), 0.0));
		} else {
			// m_line_start.y() == switch_radius
			switch_mvp = glm::translate(mvp, glm::vec3((float)(m_line_start.x() - m_line_start.y()), 0.0, 0.0));
		}

		m_switch.Draw(switch_mvp);

		event->accept(this);
	}

	void Slider::MouseMoveEvent (MouseEvent* event)
	{
		/*
		if(m_slide_button->down()) {
			dispatch_mouse_move_event(m_slide_button.get(), event);

			int value = 0;

			if (orientation() == Vertical) {
				int ymin = position().y() + m_slide_button->size().height() / 2;
				int ymax = position().y() + size().height() - m_slide_button->size().height() / 2;
				if(event->position().y() < ymin ||	event->position().y() > ymax)
					return;	// if the mouse move too far, don't count the value repeatedly

				value = (m_slide_button->position().y() - position().y()) / (float) get_space()
				        * (maximum() - minimum());

			} else {
				int xmin = position().x() + m_slide_button->size().width() / 2;
				int xmax = position().x() + size().width() - m_slide_button->size().width() / 2;
				if(event->position().x() < xmin ||	event->position().x() > xmax)
					return;	// if the mouse move too far, don't count the value repeatedly

				value = (m_slide_button->position().x() - position().x()) / (float) get_space()
				        * (maximum() - minimum());
			}

			set_value(value);
			m_slider_moved.fire(value);

			return;
		}

		if(contain(event->position())) {
			dispatch_mouse_move_event(m_slide_button.get(), event);
		}
		*/
	}

	void Slider::MousePressEvent (MouseEvent* event)
	{
		/*
		if(m_slide_button->down()) {
			dispatch_mouse_press_event(m_slide_button.get(), event);
			return;
		}

		if(contain(event->position())) {
			dispatch_mouse_press_event(m_slide_button.get(), event);
			if(event->accepted()) return;

			// Move to where mouse click
//			Coord2d inner_pos;
//			inner_pos.set_x(static_cast<double>(event->position().x() - m_pos.x() - padding().left() - m_slider_control->size().width() / 2));
//			inner_pos.set_y(static_cast<double>(event->position().y() - m_pos.y() - padding().bottom() - m_slider_control->size().height() / 2));
//			int space = get_space();
//			int value;
//
//			if (orientation()) {
//				value = (maximum() - minimum()) * inner_pos.y() / (double) space;
//			} else {
//				value = (maximum() - minimum()) * inner_pos.x() / (double) space;
//			}
//			if(value < minimum()) value = minimum();
//			if(value > maximum()) value = maximum();

//			int space = get_space();

			int val;

			if (orientation() == Vertical) {
				if(event->position().y() < m_slide_button->position().y())
					val = value() - step();
				else
					val = value() + step();
			} else {
				if(event->position().x() < m_slide_button->position().x())
					val = value() - step();
				else
					val = value() + step();
			}
			if(val < minimum()) val = minimum();
			if(val > maximum()) val = maximum();

			set_value(val);
			m_slider_moved.fire(val);
		}
		*/
	}

	void Slider::MouseReleaseEvent (MouseEvent* event)
	{
		/*
		if(m_slide_button->down()) {
			dispatch_mouse_release_event(m_slide_button.get(), event);
			return;
		}
		if(contain(event->position())) {
			if (event->button() == MouseButtonLeft) {

			}
			dispatch_mouse_release_event(m_slide_button.get(), event);
		}
		*/
	}

	int Slider::get_space ()
	{
		int space = 0;
		/*
		if(orientation() == Vertical)	// Vertical
			space = size().height() - m_slide_button->size().height();
		else
			space = size().width() - m_slide_button->size().width();
	*/
		return space;
	}

}
