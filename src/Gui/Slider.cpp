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
#include <BlendInt/Service/StockItems.hpp>

#include <iostream>

namespace BlendInt {

	// -------------------- Slider ---------------------------

	Slider::Slider(Orientation orientation)
	: AbstractSlider(orientation)
	{
		m_switch = StockItems::instance->icon_slide();

		// set default size
		if (orientation == Vertical) {
			set_size (18, 200);
			set_expand_y(true);
		} else {
			set_size (200, 18);
			set_expand_x(true);
		}
	}

	Slider::~Slider()
	{
	}

	void Slider::SetSwitchSize (size_t size)
	{
		if(orientation() == Vertical) {	// Vertical
			m_switch->Resize(m_switch->size().width(), size);
		} else {
			m_switch->Resize(size, m_switch->size().height());
		}
	}

	bool Slider::Update (int type, const void* data)
	{
		switch (type) {
			case FormPosition: {
//				const Point* pos = static_cast<const Point*>(data);
//				m_slide_button->SetPosition(m_slide_button->position().x() + (pos->x() - position().x()),
//						m_slide_button->position().y() + (pos->y() - position().y()));
				return true;
			}

			case FormSize: {
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

		//glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(1.15, 1.15, 1.15));
		//glm::mat4 rotate = glm::rotate(glm::mat4(1.0), (glm::mediump_float)(M_PI * 1.5), glm::vec3(0.0, 0.0, 1.0));
		//glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(icon->size().width()/2.f, icon->size().height()/2.f, 0.0));

		m_switch->Draw(mvp);

		event->accept(this);

		return;

		glColor4ub(themes()->scroll.outline.r(),
				themes()->scroll.outline.g(),
				themes()->scroll.outline.b(),
				themes()->scroll.outline.a());

		int space = 0;

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		if(orientation() == Vertical) {
			space = size().height();
			glTranslatef(size().width() / 2.0, 0, 0);
			glBegin(GL_LINES);
				glVertex2i(0, 0);
				glVertex2i(0, space);
			glEnd();
		} else {
			space = size().width();
			glTranslatef(0, size().height() / 2.0 - 0.5, 0);
			glBegin(GL_LINES);
				glVertex2i(0, 0);
				glVertex2i(space, 0);
			glEnd();
		}

		glPopMatrix();

		//m_switch->Draw(event->pv_matrix());

#ifdef DEBUG
		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
			glVertex2i(0, 0);
			glVertex2i(size().width(), 0);
			glVertex2i(size().width(), size().height());
			glVertex2i(0, size().height());
		glEnd();

		glDisable(GL_LINE_STIPPLE);
#endif
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
