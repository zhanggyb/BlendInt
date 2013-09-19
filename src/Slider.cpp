/*
 * This file is part of BILO (Blender-like Interface Library in
 * OpenGL).
 *
 * BILO (Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>

#include <BILO/Slider.hpp>

#include <BILO/Interface.hpp>

#include <iostream>

namespace BILO {

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

	SliderControl::SliderControl(Drawable* parent)
	:  Widget(parent), m_hover(false), m_pressed(false)
	{
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

		for (int i = 0; i < 20; i++)
		{
			m_vertexes[i][0] = circle_vertexes[i][0] * m_radius;
			m_vertexes[i][1] = circle_vertexes[i][1] * m_radius;
		}

		m_buffer.bind (GL_ARRAY_BUFFER);

		m_buffer.upload (GL_ARRAY_BUFFER, sizeof(m_vertexes), m_vertexes, GL_STATIC_DRAW);

		m_buffer.unbind (GL_ARRAY_BUFFER);

	}
	void SliderControl::update (int type)
	{
		// not allow changing size
		if (type == WidgetPropertySize) {
			size_.set_width(m_radius * 2);
			size_.set_height(m_radius * 2);
		}

		// not allow changing padding
		if (type == WidgetPropertyPadding) {
			padding_.set_left(0);
			padding_.set_right(0);
			padding_.set_top(0);
			padding_.set_bottom(0);
		}
	}

	void SliderControl::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(pos_.x(),
					 pos_.y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Theme* tm = Theme::instance();

		glColor4ub(tm->themeUI()->wcol_scroll.outline.r(),
				tm->themeUI()->wcol_scroll.outline.g(),
				tm->themeUI()->wcol_scroll.outline.b(),
				tm->themeUI()->wcol_scroll.outline.a());

		if (m_pressed) {
			glColor4ub(tm->themeUI()->wcol_scroll.inner_sel.r(),
					tm->themeUI()->wcol_scroll.inner_sel.g(),
					tm->themeUI()->wcol_scroll.inner_sel.b(),
					tm->themeUI()->wcol_scroll.inner_sel.a());
		} else if (m_hover) {
				glColor4ub(tm->themeUI()->wcol_scroll.item.highlight_red(),
						tm->themeUI()->wcol_scroll.item.highlight_green(),
						tm->themeUI()->wcol_scroll.item.highlight_blue(),
						tm->themeUI()->wcol_scroll.item.a());
		} else {
			glColor4ub(tm->themeUI()->wcol_scroll.item.r(),
					tm->themeUI()->wcol_scroll.item.g(),
					tm->themeUI()->wcol_scroll.item.b(),
					tm->themeUI()->wcol_scroll.item.a());
		}

		glTranslatef(m_radius,
					 m_radius,
					 0);

		m_buffer.bind(GL_ARRAY_BUFFER);
		glVertexPointer (2, GL_FLOAT, 0, 0);
		glEnableClientState (GL_VERTEX_ARRAY);
		glDrawArrays(GL_POLYGON, 0, 20);

		glColor4ub(tm->themeUI()->wcol_scroll.outline.r(),
				tm->themeUI()->wcol_scroll.outline.g(),
				tm->themeUI()->wcol_scroll.outline.b(),
				tm->themeUI()->wcol_scroll.outline.a());

		glDrawArrays(GL_LINE_LOOP, 0, 20);

		glDisableClientState (GL_VERTEX_ARRAY);

		m_buffer.unbind(GL_ARRAY_BUFFER);

		glDisable(GL_BLEND);

		glPopMatrix();
	}

	void SliderControl::move_mouse(MouseEvent* event)
	{
		if(contain(event->position())) {
			if (m_pressed) {
				m_hover = false;
			} else {
				m_hover = true;
			}
			event->accept();
		} else {
			m_hover = false;
		}
	}

	void SliderControl::press_mouse(MouseEvent* event)
	{
		if(contain(event->position())) {
			if (event->button() == MouseButtonLeft) {
				m_pressed = true;
				event->accept();
			}
		}
	}

	void SliderControl::release_mouse(MouseEvent* event)
	{
		if (event->button() == MouseButtonLeft) {

		}
		m_pressed = false;

	}

	Slider::Slider(Orientation orientation, Drawable* parent)
	: AbstractSlider(orientation, parent), m_slider_control(0), m_pressed(false)
	{
		m_slider_control = new SliderControl(this);

		// set default size
		if (this->orientation() == Horizontal) {
			resize (200, 25);
		} else if (this->orientation() == Vertical) {
			resize (25, 200);
		}

		m_slider_control->set_pos (pos().x() + padding().left(), pos().y() + padding().bottom());
	}

	Slider::~Slider()
	{

	}

	void Slider::update (int property)
	{
		if(property == WidgetPropertyPosition) {
			m_slider_control->set_pos (pos().x() + padding().left(), pos().y() + padding().bottom());
		}
//		if (property == WidgetPropertySize)
			//update_shape();

//		if (property == WidgetPropertyPadding)
			//update_shape();
	}

	void Slider::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(pos_.x(),
					 pos_.y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPushMatrix();

		Theme* tm = Theme::instance();

		glTranslatef(padding_.left(),
					 padding_.bottom(), 0);

		glColor4ub(tm->themeUI()->wcol_scroll.outline.r(),
				tm->themeUI()->wcol_scroll.outline.g(),
				tm->themeUI()->wcol_scroll.outline.b(),
				tm->themeUI()->wcol_scroll.outline.a());

		int space = 0;
		if(orientation() == Horizontal) {
			// move radius
			space = size_.width() - padding_.left() - padding_.right();
			glTranslatef(0, m_slider_control->radius(), 0);
			glBegin(GL_LINES);
				glVertex2i(0, 0);
				glVertex2i(space, 0);
			glEnd();
			glTranslatef(value() * space / ((float)maximum() - (float)minimum()), 0, 0);
		} else {
			space = size_.height() - padding_.top() - padding_.bottom();
			glTranslatef(m_slider_control->radius(), 0, 0);
			glBegin(GL_LINES);
				glVertex2i(0, 0);
				glVertex2i(0, space);
			glEnd();
			glTranslatef(0, value() * space / ((float)maximum() - (float)minimum()), 0);
		}

		glPopMatrix();

#ifdef DEBUG
		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
			glVertex2i(0, 0);
			glVertex2i(size_.width(), 0);
			glVertex2i(size_.width(), size_.height());
			glVertex2i(0, size_.height());
		glEnd();

		glDisable(GL_LINE_STIPPLE);
#endif

		glDisable(GL_BLEND);

		glPopMatrix();
		Interface::instance()->dispatch_render_event(m_z, m_slider_control);
	}

	void Slider::move_mouse (MouseEvent* event)
	{
		//std::wcout << event->pos().x() << " " << event->pos().y() << std::endl;

		if(contain(event->position())) {
			if (m_pressed) {

			} else {

			}
			/*
			event->set_pos(event->window_pos().x() - m_slider_control->pos().x(),
					event->window_pos().y() - m_slider_control->pos().y());*/
			Interface::instance()->dispatch_mouse_move_event(m_slider_control, event);
		} else {

		}
	}

	void Slider::press_mouse (MouseEvent* event)
	{
		if(contain(event->position())) {
			if (event->button() == MouseButtonLeft) {
				m_pressed = true;
				m_press_pos = event->position();
				Interface::instance()->dispatch_mouse_press_event(m_slider_control, event);
			}
		}
	}

	void Slider::release_mouse (MouseEvent* event)
	{
		if(contain(event->position())) {
			if (event->button() == MouseButtonLeft) {
				m_pressed = false;
				m_press_pos = event->position();
				Interface::instance()->dispatch_mouse_release_event(m_slider_control, event);
			}
		}
	}

	int Slider::get_space ()
	{
		int space = 0;
		if(orientation() == Horizontal)
			space = size_.width() - padding_.left() - padding_.right();
		else
			space = size_.height() - padding_.top() - padding_.bottom();

		return space;
	}

}

