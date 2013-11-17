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

#include <GL/glew.h>

#include <BlendInt/Slider.hpp>

#include <BlendInt/Theme.hpp>

#include <iostream>

namespace BlendInt {

	SlideButton::SlideButton()
	:  Button()
	{
	}

	SlideButton::SlideButton(AbstractWidget* parent)
	:  Button(parent)
	{
	}

	SlideButton::~SlideButton()
	{

	}

	void SlideButton::update (int type, const void* data)
	{
		switch(type)
		{
			case FormSize: {
				const Size* size_p = static_cast<const Size*>(data);
				update_shape(size_p);
				break;
			}

			case FormRoundRadius: {
				const Size* size_p = static_cast<const Size*>(data);
				update_shape(size_p);
				break;
			}

			default:
				break;
		}
	}

	void SlideButton::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(),
					 position().y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if(m_status_down) {
			buffer()->select(0);

		} else {
			buffer()->select(2);
		}

		buffer()->bind();
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
		glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glDrawArrays(GL_POLYGON, 0, buffer()->vertices());

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		buffer()->unbind();

		// draw outline
		buffer()->select(1);

		unsigned char tcol[4] = { themes()->scroll.outline.r(),
		        themes()->scroll.outline.g(),
		        themes()->scroll.outline.b(),
		        themes()->scroll.outline.a()};

		tcol[3] = tcol[3] / WIDGET_AA_JITTER;

		buffer()->bind();

		/* outline */
		glEnableClientState(GL_VERTEX_ARRAY);
		glColor4ubv(tcol);
		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			glTranslatef(jit[j][0], jit[j][1], 0.0f);
			glVertexPointer(2, GL_FLOAT, 0, 0);
			glDrawArrays(GL_QUAD_STRIP, 0, buffer()->vertices());
			glTranslatef(-jit[j][0], -jit[j][1], 0.0f);
		}
		glDisableClientState(GL_VERTEX_ARRAY);

		buffer()->unbind();

		glDisable(GL_BLEND);

		glPopMatrix();
	}

	void SlideButton::move_mouse(MouseEvent* event)
	{
		// if no parent slider, don't react to mouse move
		if(parent().type != ParentForm) return;

		AbstractSlider* parent_obj = dynamic_cast<AbstractSlider*>(parent().object.form);
		if(!parent_obj) return;

		if(m_status_down) {
			m_status_hover = false;

			if(parent_obj->orientation() == Vertical) {	// Vertical

				set_position(position().x(), m_position_origin.y() + event->position().y() - m_move_start.y());

				if(position().y() < (parent_obj->position().y())) {

					set_position(position().x(), parent_obj->position().y());

				}
				if(position().y() > (int)(parent_obj->position().y() + parent_obj->size().height() - size().height())) {
					set_position(position().x(), parent_obj->position().y() + parent_obj->size().height() - size().height());

				}

			} else {

				set_position(m_position_origin.x() + event->position().x() - m_move_start.x(), position().y());
				if(position().x() < (parent_obj->position().x())) {
					set_position(parent_obj->position().x(), position().y());
				}
				if(position().x() >
						(int)(parent_obj->position().x() + parent_obj->size().width() - size().width())) {
					set_position(parent_obj->position().x() + parent_obj->size().width() - size().width(), position().y());
				}

			}
			event->accept(this);
			return;

		} else {

			if(contain(event->position())) {
				if (m_status_down) {
					m_status_hover = false;
				} else {
					m_status_hover = true;
				}
				event->accept(this);
			} else {
				m_status_hover = false;
			}
		}
	}

	void SlideButton::press_mouse(MouseEvent* event)
	{
		if(contain(event->position())) {
			if (event->button() == MouseButtonLeft) {
				m_status_down = true;
				m_move_start.set_x(event->position().x());
				m_move_start.set_y(event->position().y());
				m_position_origin = position();
				event->accept(this);
			}
		}
	}

	void SlideButton::release_mouse(MouseEvent* event)
	{
		if (event->button() == MouseButtonLeft) {

		}
		m_status_down = false;

	}

	void SlideButton::update_shape(const Size* size)
	{
		float outer_v[WIDGET_SIZE_MAX][2];	// vertices for drawing outline
		float inner_v[WIDGET_SIZE_MAX][6];	// vertices for drawing inner

		VerticesSum vert_sum;

		Orientation shadedir;

		if(parent().type != ParentForm) {
			shadedir = size->width() < size->height() ? Horizontal : Vertical;
		} else {
			AbstractSlider* parent_obj = dynamic_cast<AbstractSlider*>(parent().object.form);
			if(parent_obj) {
				shadedir = parent_obj->orientation() == Horizontal ? Horizontal : Vertical;
			}
		}
		Color color = themes()->scroll.item;

//		if(shadedir == Vertical)
//			set_radius(0.5f * size->height());
//		else
//			set_radius(0.5f * size->width());

		short shadetop = themes()->scroll.shadetop;
		short shadedown = themes()->scroll.shadedown;

		if(shadedir == Vertical) {
			vert_sum = generate_vertices(size,
					border_width(),
					color,
					shadetop,
					shadedown,
					shadedir,
					inner_v, outer_v);
		} else {	// swap shadetop and shadedown
			vert_sum = generate_vertices(size,
					border_width(),
					color,
					shadedown,
					shadetop,
					shadedir,
					inner_v, outer_v);
		}

		buffer()->generate(3);
		buffer()->select(0);

		buffer()->set_property(vert_sum.total, sizeof(inner_v[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		buffer()->bind();
		buffer()->upload(inner_v);
		buffer()->unbind();

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */

		verts_to_quad_strip (inner_v, outer_v, vert_sum.total, quad_strip);

		buffer()->select(1);
		buffer()->set_property(vert_sum.total * 2 + 2, sizeof(quad_strip[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

		buffer()->bind();
		buffer()->upload(quad_strip);
		buffer()->unbind();

		color.highlight(color, 5);

		if(shadedir == Vertical) {
			vert_sum = generate_vertices(size,
					border_width(),
					color,
					shadetop,
					shadedown,
					shadedir,
					inner_v, outer_v);
		} else {	// swap shadetop and shadedown
			vert_sum = generate_vertices(size,
					border_width(),
					color,
					shadedown,
					shadetop,
					shadedir,
					inner_v, outer_v);
		}

		buffer()->select(2);

		buffer()->set_property(vert_sum.total, sizeof(inner_v[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		buffer()->bind();
		buffer()->upload(inner_v);
		buffer()->unbind();

	}

	// -------------------- Slider ---------------------------

	Slider::Slider(Orientation orientation)
	: AbstractSlider(orientation), m_slide_button(0)
	{
		m_slide_button = new SlideButton(this);

		// set default size
		if (orientation == Vertical) {
			resize (25, 200);
			set_expand_y(true);
		} else {
			resize (200, 25);
			set_expand_x(true);
		}

		size_t button_size = std::min (size().width(), size().height());

		m_slide_button->resize(button_size, button_size);
		//m_slide_button->set_position (position().x() + padding().left(), position().y() + padding().bottom());
		update(SliderPropertyValue, 0);
	}

	Slider::Slider(Orientation orientation, AbstractWidget* parent)
	: AbstractSlider(orientation, parent), m_slide_button(0)
	{
		m_slide_button = new SlideButton(this);

		// set default size
		if (orientation == Vertical) {
			resize (25, 200);
			set_expand_y(true);
		} else {
			resize (200, 25);
			set_expand_x(true);
		}

		size_t button_size = std::min (size().width(), size().height());

		m_slide_button->resize(button_size, button_size);

		//m_slide_button->set_position (position().x() + padding().left(), position().y() + padding().bottom());
		update(SliderPropertyValue, 0);
	}

	Slider::~Slider()
	{

	}

	void Slider::set_control_size (size_t size)
	{
		if(orientation() == Vertical) {	// Vertical
			m_slide_button->resize(m_slide_button->size().width(), size);
		} else {
			m_slide_button->resize(size, m_slide_button->size().height());
		}
	}

	void Slider::set_control_widget(Button* widget)
	{
		if(widget) {
			if(m_slide_button)
				delete m_slide_button;

			bind(widget);
			m_slide_button = widget;
		}
	}

	void Slider::update (int type, const void* data)
	{
		switch (type) {
			case FormPosition: {
				m_slide_button->set_position (position().x(), position().y());
				return;
			}

			case FormSize: {

				size_t button_size = std::min (size().width(),
						size().height());

				m_slide_button->resize(button_size, button_size);

				if(orientation() == Vertical) {
					m_slide_button->set_position (m_slide_button->position().x(),
							position().y() + value() * get_space() / (float)(maximum() - minimum()));
				} else {
					m_slide_button->set_position (position().x() + value() * get_space() / (float)(maximum() - minimum()),
							m_slide_button->position().y());
				}

				return;
			}

			case SliderPropertyValue: {

				if(orientation() == Vertical) {
					m_slide_button->set_position (m_slide_button->position().x(),
							position().y() + value() * get_space() / (float)(maximum() - minimum()));
				} else {
					m_slide_button->set_position (position().x() + value() * get_space() / (float)(maximum() - minimum()),
							m_slide_button->position().y());
				}

				return;
			}

			default:
				break;
		}
	}

	void Slider::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(),
					 position().y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4ub(themes()->scroll.outline.r(),
				themes()->scroll.outline.g(),
				themes()->scroll.outline.b(),
				themes()->scroll.outline.a());

		int space = 0;

		if(orientation() == Vertical) {
			space = size().height();
			glTranslatef(m_slide_button->size().width() / 2.0, 0, 0);
			glBegin(GL_LINES);
				glVertex2i(0, 0);
				glVertex2i(0, space);
			glEnd();
		} else {
			space = size().width();
			glTranslatef(0, m_slide_button->size().height() / 2.0 - 0.5, 0);
			glBegin(GL_LINES);
				glVertex2i(0, 0);
				glVertex2i(space, 0);
			glEnd();
		}

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

		glDisable(GL_BLEND);

		glPopMatrix();

		dispatch_render(m_slide_button);
	}

	void Slider::move_mouse (MouseEvent* event)
	{
		if(m_slide_button->down()) {
			dispatch_mouse_move_event(m_slide_button, event);

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
			dispatch_mouse_move_event(m_slide_button, event);
		}
	}

	void Slider::press_mouse (MouseEvent* event)
	{
		if(m_slide_button->down()) {
			dispatch_mouse_press_event(m_slide_button, event);
			return;
		}

		if(contain(event->position())) {
			dispatch_mouse_press_event(m_slide_button, event);
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
	}

	void Slider::release_mouse (MouseEvent* event)
	{
		if(m_slide_button->down()) {
			dispatch_mouse_release_event(m_slide_button, event);
			return;
		}
		if(contain(event->position())) {
			if (event->button() == MouseButtonLeft) {

			}
			dispatch_mouse_release_event(m_slide_button, event);
		}
	}

	int Slider::get_space ()
	{
		int space = 0;
		if(orientation() == Vertical)	// Vertical
			space = size().height() - m_slide_button->size().height();
		else
			space = size().width() - m_slide_button->size().width();

		return space;
	}

}
