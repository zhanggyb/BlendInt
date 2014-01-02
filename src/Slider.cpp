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

#include <BlendInt/Slider.hpp>
#include <BlendInt/Theme.hpp>

#include <iostream>

namespace BlendInt {

	SlideButton::SlideButton()
	:  AbstractButton()
	{
		m_buffer.reset(new GLArrayBufferMultiple);
	}

	SlideButton::SlideButton(AbstractWidget* parent)
	:  AbstractButton(parent)
	{
		m_buffer.reset(new GLArrayBufferMultiple);
	}

	SlideButton::~SlideButton()
	{

	}

	void SlideButton::Update (int type, const void* data)
	{
		switch(type)
		{
			case FormSize: {
				const Size* size_p = static_cast<const Size*>(data);
				Orientation shadedir = size_p->width() < size_p->height() ? Horizontal : Vertical;
				const Color& color = themes()->scroll.item;
				short shadetop = themes()->scroll.shadetop;
				short shadedown = themes()->scroll.shadedown;

				GenerateShadedFormBuffers(size_p,
						DefaultBorderWidth(),
						round_type(),
						radius(),
						color,
						shadetop,
						shadedown,
						shadedir,
						5,
						m_buffer.get()
						);
				break;
			}

			case FormRoundRadius: {
				const Size* size_p = &(size());
				Orientation shadedir = size_p->width() < size_p->height() ? Horizontal : Vertical;
				const float* radius_p = static_cast<const float*>(data);
				const Color& color = themes()->scroll.item;
				short shadetop = themes()->scroll.shadetop;
				short shadedown = themes()->scroll.shadedown;

				GenerateShadedFormBuffers(size_p,
						DefaultBorderWidth(),
						round_type(),
						*radius_p,
						color,
						shadetop,
						shadedown,
						shadedir,
						5,
						m_buffer.get()
						);
				break;
			}

			default:
				break;
		}
	}

	void SlideButton::Draw ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(),
					 position().y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if(down()) {
			draw_shaded_inner_buffer(m_buffer.get(), 0);
		} else {
			draw_shaded_inner_buffer(m_buffer.get(), 2);
		}

		// draw outline
		unsigned char tcol[4] = { themes()->scroll.outline.r(),
		        themes()->scroll.outline.g(), themes()->scroll.outline.b(),
		        themes()->scroll.outline.a() };

		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		glColor4ubv(tcol);

		draw_outline_buffer(m_buffer.get(), 1);

		glDisable(GL_BLEND);

		glPopMatrix();
	}

	void SlideButton::MouseMoveEvent(MouseEvent* event)
	{
		// if no parent slider, don't react to mouse move
		if(parent().type != ParentForm) return;

		AbstractSlider* parent_obj = dynamic_cast<AbstractSlider*>(parent().object.form);
		if(!parent_obj) return;

		if (down()) {
			set_hover(false);
			if (parent_obj->orientation() == Vertical) {
				SetPosition(position().x(),
				        m_position_origin.y() + event->position().y()
				                - m_move_start.y());

				if (position().y() < (parent_obj->position().y())) {
					SetPosition(position().x(), parent_obj->position().y());
				}
				if (position().y()
				        > (int) (parent_obj->position().y()
				                + parent_obj->size().height() - size().height())) {
					SetPosition(position().x(),
					        parent_obj->position().y()
					                + parent_obj->size().height()
					                - size().height());
				}
			} else {
				SetPosition(
				        m_position_origin.x() + event->position().x()
				                - m_move_start.x(), position().y());
				if (position().x() < (parent_obj->position().x())) {
					SetPosition(parent_obj->position().x(), position().y());
				}
				if (position().x()
				        > (int) (parent_obj->position().x()
				                + parent_obj->size().width() - size().width())) {
					SetPosition(
					        parent_obj->position().x()
					                + parent_obj->size().width()
					                - size().width(), position().y());
				}
			}
			event->accept(this);
			return;

		} else {

			if(contain(event->position())) {
				if (down()) {
					set_hover(false);
				} else {
					set_hover(true);
				}
				event->accept(this);
			} else {
				set_hover(false);
			}
		}
	}

	void SlideButton::MousePressEvent(MouseEvent* event)
	{
		if(contain(event->position())) {
			if (event->button() == MouseButtonLeft) {
				set_down(true);
				m_move_start.set_x(event->position().x());
				m_move_start.set_y(event->position().y());
				m_position_origin = position();
				event->accept(this);
			}
		}
	}

	void SlideButton::MouseReleaseEvent(MouseEvent* event)
	{
		if (event->button() == MouseButtonLeft) {

		}
		set_down(false);
	}

	// -------------------- Slider ---------------------------

	Slider::Slider(Orientation orientation)
	: AbstractSlider(orientation), m_slide_button(0)
	{
		m_slide_button = new SlideButton(this);

		// set default size
		if (orientation == Vertical) {
			set_size (18, 200);
			set_expand_y(true);
		} else {
			set_size (200, 18);
			set_expand_x(true);
		}

		unsigned int button_size = static_cast<unsigned int>(std::min (size().width(), size().height()));

		m_slide_button->Resize(button_size, button_size);

		m_slide_button->SetRoundType(RoundAll);
		m_slide_button->SetRadius(button_size / 2.0f);

		m_slide_button->SetPosition (position().x(), position().y());

//		if(orientation == Vertical) {
//			m_slide_button->SetPosition (m_slide_button->position().x(),
//					position().y() + value() * get_space() / (float)(maximum() - minimum()));
//		} else {
//			m_slide_button->SetPosition (position().x() + value() * get_space() / (float)(maximum() - minimum()),
//					m_slide_button->position().y());
//		}
	}

	Slider::Slider(Orientation orientation, AbstractWidget* parent)
	: AbstractSlider(orientation, parent), m_slide_button(0)
	{
		m_slide_button = new SlideButton(this);

		// set default size
		if (orientation == Vertical) {
			set_size (18, 200);
			set_expand_y(true);
		} else {
			set_size (200, 18);
			set_expand_x(true);
		}

		unsigned int button_size = static_cast<unsigned int>(std::min (size().width(), size().height()));

		m_slide_button->Resize(button_size, button_size);

		m_slide_button->SetRoundType(RoundAll);
		m_slide_button->SetRadius(button_size / 2.0f);

		m_slide_button->SetPosition (position().x(), position().y());
//		Update(SliderPropertyValue, 0);
	}

	Slider::~Slider()
	{

	}

	void Slider::set_control_size (size_t size)
	{
		if(orientation() == Vertical) {	// Vertical
			m_slide_button->Resize(m_slide_button->size().width(), size);
		} else {
			m_slide_button->Resize(size, m_slide_button->size().height());
		}
	}

	void Slider::set_control_widget(SlideButton* widget)
	{
		if(widget) {
			if(m_slide_button)
				delete m_slide_button;

			Bind(widget);
			m_slide_button = widget;
		}
	}

	void Slider::Update (int type, const void* data)
	{
		switch (type) {
			case FormPosition: {
				const Point* pos = static_cast<const Point*>(data);
				m_slide_button->SetPosition(m_slide_button->position().x() + (pos->x() - position().x()),
						m_slide_button->position().y() + (pos->y() - position().y()));
				return;
			}

			case FormSize: {
				unsigned int button_size = static_cast<unsigned int>(std::min(size().width(),
						size().height()));

				m_slide_button->Resize(button_size, button_size);

				if(orientation() == Vertical) {
					m_slide_button->SetPosition (m_slide_button->position().x(),
							position().y() + value() * get_space() / (float)(maximum() - minimum()));
				} else {
					m_slide_button->SetPosition (position().x() + value() * get_space() / (float)(maximum() - minimum()),
							m_slide_button->position().y());
				}

				return;
			}

			case SliderPropertyValue: {

				if(orientation() == Vertical) {
					m_slide_button->SetPosition (m_slide_button->position().x(),
							position().y() + value() * get_space() / (float)(maximum() - minimum()));
				} else {
					m_slide_button->SetPosition (position().x() + value() * get_space() / (float)(maximum() - minimum()),
							m_slide_button->position().y());
				}

				return;
			}

			default:
				break;
		}
	}

	void Slider::Draw ()
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

#ifdef DEBUG
		glPushMatrix();
#endif

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

#ifdef DEBUG
		glPopMatrix();
#endif

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

		DispatchRender(m_slide_button);
	}

	void Slider::MouseMoveEvent (MouseEvent* event)
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

	void Slider::MousePressEvent (MouseEvent* event)
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

	void Slider::MouseReleaseEvent (MouseEvent* event)
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
