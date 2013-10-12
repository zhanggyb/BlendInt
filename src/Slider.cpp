/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>

#include <BlendInt/Slider.hpp>

#include <BlendInt/Interface.hpp>
#include <BlendInt/Theme.hpp>

#include <iostream>

namespace BlendInt {


	const float SliderControl::circle_vertexes[20][2] =
	{
			{1.000000, 0.000000},	// cos(0), sin(0)
			{0.951057, 0.309017},	// cos(18), sin(18)
			{0.809017, 0.587785},	// cos(36), sin(36)
			{0.587785, 0.809017},	// cos(54), sin(54)
			{0.309017, 0.951057},	// cos(72), sin(72)
			{0.000000, 1.000000},	// cos(90), sin(90)
			{-0.309017, 0.951057},
			{-0.587785, 0.809017},
			{-0.809017, 0.587785},
			{-0.951057, 0.309017},
			{-1.000000, 0.000000},
			{-0.951057, -0.309017},
			{-0.809017, -0.587785},
			{-0.587785, -0.809017},
			{-0.309017, -0.951057},
			{0.000000, -1.000000},
			{0.309017, -0.951057},
			{0.587785, -0.809017},
			{0.809017, -0.587785},
			{0.951057, -0.309017}
	};


	SliderControl::SliderControl()
	:  Widget(), m_hover(false), m_pressed(false)
	{
		buffer().generate(1);
		buffer().set_target(GL_ARRAY_BUFFER);
		buffer().set_usage(GL_STATIC_DRAW);

		set_padding (0, 0, 0, 0);
		set_radius(7);
		resize (m_radius * 2, m_radius * 2);
	}

	SliderControl::SliderControl(Drawable* parent)
	:  Widget(parent), m_hover(false), m_pressed(false)
	{
		buffer().generate(1);
		buffer().set_target(GL_ARRAY_BUFFER);
		buffer().set_usage(GL_STATIC_DRAW);

		set_padding (0, 0, 0, 0);
		set_radius(7);
		resize (m_radius * 2, m_radius * 2);
	}

	SliderControl::~SliderControl()
	{

	}

	void SliderControl::set_radius(unsigned int radius)
	{
		if (radius < 4) return;	// too small

		m_radius = radius;

		float vertexes[20][2];

		for (int i = 0; i < 20; i++)
		{
			vertexes[i][0] = circle_vertexes[i][0] * m_radius;
			vertexes[i][1] = circle_vertexes[i][1] * m_radius;
		}

		m_buffer.set_vertices(20);
		m_buffer.set_unit_size(sizeof(vertexes[0]));
		m_buffer.bind ();
		m_buffer.upload (vertexes);
		m_buffer.unbind ();
	}

	bool SliderControl::update (int type, const void* property)
	{
		// not allow changing size
		if (type == WidgetPropertySize) {
			m_size.set_width(m_radius * 2);
			m_size.set_height(m_radius * 2);
			return false;
		}

		// not allow changing padding
		if (type == WidgetPropertyPadding) {
			m_padding.set_left(0);
			m_padding.set_right(0);
			m_padding.set_top(0);
			m_padding.set_bottom(0);
			return false;
		}

		return Widget::update(type, property);
	}

	void SliderControl::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(m_pos.x(),
					 m_pos.y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4ub(themes()->scroll.outline.r(),
				themes()->scroll.outline.g(),
				themes()->scroll.outline.b(),
				themes()->scroll.outline.a());

		if (m_pressed) {
			glColor4ub(themes()->scroll.inner_sel.r(),
					themes()->scroll.inner_sel.g(),
					themes()->scroll.inner_sel.b(),
					themes()->scroll.inner_sel.a());
		} else if (m_hover) {
				glColor4ub(themes()->scroll.item.highlight_red(),
						themes()->scroll.item.highlight_green(),
						themes()->scroll.item.highlight_blue(),
						themes()->scroll.item.a());
		} else {
			glColor4ub(themes()->scroll.item.r(),
					themes()->scroll.item.g(),
					themes()->scroll.item.b(),
					themes()->scroll.item.a());
		}

		glTranslatef(m_radius,
					 m_radius,
					 0);

		m_buffer.bind();
		glVertexPointer (2, GL_FLOAT, 0, 0);
		glEnableClientState (GL_VERTEX_ARRAY);
		glDrawArrays(GL_POLYGON, 0, 20);

		glColor4ub(themes()->scroll.outline.r(),
				themes()->scroll.outline.g(),
				themes()->scroll.outline.b(),
				themes()->scroll.outline.a());

		glDrawArrays(GL_LINE_LOOP, 0, 20);

		glDisableClientState (GL_VERTEX_ARRAY);

		m_buffer.unbind();

		glDisable(GL_BLEND);

		glPopMatrix();
	}

	void SliderControl::move_mouse(MouseEvent* event)
	{
		// if no parent slider, don't react to mouse move
		if(m_parent.type != ParentDrawable) return;

		Slider* parent = dynamic_cast<Slider*>(m_parent.object.drawable);
		if(!parent) return;

		if(m_pressed) {
			m_hover = false;

			if(parent->orientation() == Horizontal) {

				m_pos.set_x(m_position_origin.x() + event->position().x() - m_move_start.x());
				if(m_pos.x() < (parent->pos().x() + parent->padding().left())) {
					m_pos.set_x(parent->pos().x() + parent->padding().left());
				}
				if(m_pos.x() >
						(int)(parent->pos().x() + parent->size().width() - parent->padding().right() - m_radius * 2)) {
					m_pos.set_x(parent->pos().x() + parent->size().width() - parent->padding().right() - m_radius * 2);
				}

			}
			if(parent->orientation() == Vertical) {

				m_pos.set_y(m_position_origin.y() + event->position().y() - m_move_start.y());
				if(m_pos.y() < (parent->pos().y() + parent->padding().bottom())) {
					m_pos.set_y(parent->pos().y() + parent->padding().bottom());
				}
				if(m_pos.y() > (int)(parent->pos().y() + parent->size().height() - parent->padding().top() - m_radius * 2)) {
					m_pos.set_y(parent->pos().y() + parent->size().height() - parent->padding().top() - m_radius * 2);
				}

			}
			event->accept(this);
			return;

		} else {

			if(contain(event->position())) {
				if (m_pressed) {
					m_hover = false;
				} else {
					m_hover = true;
				}
				event->accept(this);
			} else {
				m_hover = false;
			}
		}
	}

	void SliderControl::press_mouse(MouseEvent* event)
	{
		if(contain(event->position())) {
			if (event->button() == MouseButtonLeft) {
				m_pressed = true;
				m_move_start.set_x(event->position().x());
				m_move_start.set_y(event->position().y());
				m_position_origin = m_pos;
				event->accept(this);
			}
		}
	}

	void SliderControl::release_mouse(MouseEvent* event)
	{
		if (event->button() == MouseButtonLeft) {

		}
		m_pressed = false;

	}

	Slider::Slider(Orientation orientation)
	: AbstractSlider(orientation), m_slider_control(0)
	{
		m_slider_control = new SliderControl(this);

		// set default size
		if (orientation) {
			resize (25, 200);
			set_vexpand(true);
		} else {
			resize (200, 25);
			set_hexpand(true);
		}

		m_slider_control->set_pos (pos().x() + padding().left(), pos().y() + padding().bottom());
		update(SliderPropertyValue, 0);
	}

	Slider::Slider(Orientation orientation, Drawable* parent)
	: AbstractSlider(orientation, parent), m_slider_control(0)
	{
		m_slider_control = new SliderControl(this);

		// set default size
		if (orientation) {
			resize (25, 200);
			set_vexpand(true);
		} else {
			resize (200, 25);
			set_hexpand(true);
		}

		m_slider_control->set_pos (pos().x() + padding().left(), pos().y() + padding().bottom());
		update(SliderPropertyValue, 0);
	}

	Slider::~Slider()
	{

	}

	bool Slider::update (int type, const void* property)
	{
		if(type == WidgetPropertyPosition) {
			const Point* new_pos = static_cast<const Point*>(property);
			m_slider_control->set_pos (new_pos->x() + padding().left(), new_pos->y() + padding().bottom());
			return true;
		}

		if (type == SliderPropertyValue) {

			if(orientation()) {
				m_slider_control->set_pos (m_slider_control->pos().x(),
						m_pos.y() + m_padding.bottom() + value() * get_space() / (float)(maximum() - minimum()));
			} else {
				m_slider_control->set_pos (m_pos.x() + m_padding.left() + value() * get_space() / (float)(maximum() - minimum()),
						m_slider_control->pos().y());
			}

			return true;
		}

		return true;
	}

	void Slider::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(m_pos.x(),
					 m_pos.y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPushMatrix();

		glTranslatef(m_padding.left(),
					 m_padding.bottom(), 0);

		glColor4ub(themes()->scroll.outline.r(),
				themes()->scroll.outline.g(),
				themes()->scroll.outline.b(),
				themes()->scroll.outline.a());

		int space = 0;

		if(orientation()) {
			space = m_size.height() - m_padding.top() - m_padding.bottom() - m_slider_control->radius() * 2;
			glTranslatef(m_slider_control->radius(), m_slider_control->radius(), 0);
			glBegin(GL_LINES);
				glVertex2i(0, 0);
				glVertex2i(0, space);
			glEnd();
			glTranslatef(0, value() * space / ((float)maximum() - (float)minimum()), 0);
		} else {
			space = m_size.width() - m_padding.left() - m_padding.right() - m_slider_control->radius() * 2;
			glTranslatef(m_slider_control->radius(), m_slider_control->radius(), 0);
			glBegin(GL_LINES);
				glVertex2i(0, 0);
				glVertex2i(space, 0);
			glEnd();
			glTranslatef(value() * space / ((float)maximum() - (float)minimum()), 0, 0);
		}

		glPopMatrix();

#ifdef DEBUG
		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
			glVertex2i(0, 0);
			glVertex2i(m_size.width(), 0);
			glVertex2i(m_size.width(), m_size.height());
			glVertex2i(0, m_size.height());
		glEnd();

		glDisable(GL_LINE_STIPPLE);
#endif

		glDisable(GL_BLEND);

		glPopMatrix();

		Interface::instance()->dispatch_render_event(m_slider_control);
	}

	void Slider::move_mouse (MouseEvent* event)
	{
		if(m_slider_control->pressed()) {
			Interface::instance()->dispatch_mouse_move_event(m_slider_control, event);
			int value = 0;

			if (orientation()) {
				value = (m_slider_control->pos().y() - m_pos.y() - m_padding.bottom()) / (float)get_space() * (maximum() - minimum());
			} else {
				value = (m_slider_control->pos().x() - m_pos.x() - m_padding.left()) / (float)get_space() * (maximum() - minimum());
			}

			set_value (value);
			m_slider_moved.fire(value);
			return;
		}

		if(contain(event->position())) {
			Interface::instance()->dispatch_mouse_move_event(m_slider_control, event);
		}
	}

	void Slider::press_mouse (MouseEvent* event)
	{
		if(m_slider_control->pressed()) {
			Interface::instance()->dispatch_mouse_press_event(m_slider_control, event);
			return;
		}

		if(contain(event->position())) {
			Interface::instance()->dispatch_mouse_press_event(m_slider_control, event);
			if(event->accepted()) return;

			Coord2d inner_pos;
			inner_pos.set_x(static_cast<double>(event->position().x() - m_pos.x() - m_padding.left() - m_slider_control->radius()));
			inner_pos.set_y(static_cast<double>(event->position().y() - m_pos.y() - m_padding.bottom() - m_slider_control->radius()));
			int space = get_space();
			int value;

			if (orientation()) {
				if(inner_pos.y() < space) {
					value = (maximum() - minimum()) * inner_pos.y() / (double) space;
					set_value(value);
					m_slider_moved.fire(value);
				}
			} else {
				if(inner_pos.x() < space) {
					value = (maximum() - minimum()) * inner_pos.x() / (double) space;
					set_value(value);
					m_slider_moved.fire(value);
				}
			}
		}
	}

	void Slider::release_mouse (MouseEvent* event)
	{
		if(m_slider_control->pressed()) {
			Interface::instance()->dispatch_mouse_release_event(m_slider_control, event);
			return;
		}
		if(contain(event->position())) {
			if (event->button() == MouseButtonLeft) {

			}
			Interface::instance()->dispatch_mouse_release_event(m_slider_control, event);
		}
	}

	int Slider::get_space ()
	{
		int space = 0;
		if(orientation() == Horizontal)
			space = m_size.width() - m_padding.left() - m_padding.right() - m_slider_control->radius() * 2;
		else
			space = m_size.height() - m_padding.top() - m_padding.bottom() - m_slider_control->radius() * 2;

		return space;
	}

}

