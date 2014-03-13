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

#include <BlendInt/Gui/ScrollBar.hpp>
#include <BlendInt/Service/Theme.hpp>

namespace BlendInt {

	ScrollControl::ScrollControl ()
	: AbstractButton(), m_inner_buffer(0), m_outer_buffer(0)
	{
		m_inner_buffer.reset(new GLArrayBuffer);
		m_outer_buffer.reset(new GLArrayBuffer);
		m_highlight_buffer.reset(new GLArrayBuffer);

		set_round_type(RoundAll);

		Init ();
	}

	ScrollControl::~ScrollControl ()
	{
	}

	bool ScrollControl::Update (const UpdateRequest& request)
	{
		if (request.id() == Predefined) {

			switch (request.type()) {
				case FormSize: {
					const Size* size_p =
					        static_cast<const Size*>(request.data());
					Orientation shadedir =
					        size_p->width() < size_p->height() ?
					                Horizontal : Vertical;
					const Color& color = themes()->scroll.item;
					short shadetop = themes()->scroll.shadetop;
					short shadedown = themes()->scroll.shadedown;

					GenerateShadedFormBuffers(size_p, round_type(), radius(),
					        color, shadetop, shadedown, shadedir, 5,
					        m_inner_buffer.get(), m_outer_buffer.get(),
					        m_highlight_buffer.get());
					return true;
				}

				case FormRoundRadius: {
					const Size* size_p = &(size());
					Orientation shadedir =
					        size_p->width() < size_p->height() ?
					                Horizontal : Vertical;
					const float* radius_p =
					        static_cast<const float*>(request.data());
					const Color& color = themes()->scroll.item;
					short shadetop = themes()->scroll.shadetop;
					short shadedown = themes()->scroll.shadedown;

					GenerateShadedFormBuffers(size_p, round_type(), *radius_p,
					        color, shadetop, shadedown, shadedir, 5,
					        m_inner_buffer.get(), m_outer_buffer.get(),
					        m_highlight_buffer.get());
					return true;
				}

				default:
					return false;
			}
		} else {
			return false;
		}
	}

	void ScrollControl::Draw (RedrawEvent* event)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(),
					 position().y(),
					 z());

		if(down()) {
			DrawShadedInnerBuffer(m_inner_buffer.get());
		} else {
			DrawShadedInnerBuffer(m_highlight_buffer.get());
		}

		// draw outline
		unsigned char tcol[4] = { themes()->scroll.outline.r(),
		        themes()->scroll.outline.g(),
		        themes()->scroll.outline.b(),
		        themes()->scroll.outline.a()};

		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		glColor4ubv(tcol);

		DrawOutlineBuffer(m_outer_buffer.get());

		glPopMatrix();
	}

	void ScrollControl::MouseMoveEvent (MouseEvent* event)
	{
		// if no parent scrollbar, don't react to mouse move

		ScrollBar* parent_obj = dynamic_cast<ScrollBar*>(container());

		if(!parent_obj) return;

		if(down()) {

			if(parent_obj->orientation() == Horizontal) {

				SetPosition(m_position_origin.x() + event->position().x() - m_move_start.x(), position().y());

				if(position().x() < (parent_obj->position().x()))
				{
					SetPosition(parent_obj->position().x(), position().y());
				}
				if(position().x() >
						(int)(parent_obj->position().x() + parent_obj->size().width() - size().width()))
				{
					SetPosition(parent_obj->position().x() + parent_obj->size().width() - size().width(), position().y());
				}
			}

			if(parent_obj->orientation() == Vertical) {
				SetPosition(position().x(), m_position_origin.y() + event->position().y() - m_move_start.y());
				if(position().y() < (parent_obj->position().y())) {

					SetPosition(position().x(), parent_obj->position().y());
				}
				if(position().y() > (int)(parent_obj->position().y() + parent_obj->size().height() - size().height())) {

					SetPosition(position().x(), parent_obj->position().y() + parent_obj->size().height() - size().height());

				}
			}

			event->accept(this);
			return;
		}
	}

	void ScrollControl::MousePressEvent (MouseEvent* event)
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

	void ScrollControl::MouseReleaseEvent (MouseEvent* event)
	{
		if (event->button() == MouseButtonLeft) {

		}
		set_down(false);
	}

	void ScrollControl::Init ()
	{
		const Size* size_p = &(size());
		Orientation shadedir =
		        size_p->width() < size_p->height() ? Horizontal : Vertical;
		const Color& color = themes()->scroll.item;
		short shadetop = themes()->scroll.shadetop;
		short shadedown = themes()->scroll.shadedown;

		GenerateShadedFormBuffers(size_p, round_type(), radius(),
		        color, shadetop, shadedown, shadedir, 5, m_inner_buffer.get(), m_outer_buffer.get(), m_highlight_buffer.get());
	}

	// ---------------------------- SliderBar -------------------------------

	SliderBar::SliderBar(Orientation orientation)
	: AbstractSlider(orientation), m_inner_buffer(0), m_outer_buffer(0), m_control_button(0)
	{
		m_inner_buffer.reset(new GLArrayBuffer);
		m_outer_buffer.reset(new GLArrayBuffer);

		m_control_button.reset(new ScrollControl);

		set_round_type(RoundAll);

		if (orientation == Vertical) {	// Vertical
			set_size(16, 400);
			m_control_button->Resize(16, 100);
			m_control_button->SetRadius(8.0);
			set_radius(m_control_button->size().width()/2);
			set_size(m_control_button->size().width(), 400);
			set_expand_y(true);
		} else {
			set_size(400, 16);
			m_control_button->Resize(100, 16);
			m_control_button->SetRadius(8.0);
			set_radius(m_control_button->size().height());
			set_size(400, m_control_button->size().height());
			set_expand_x(true);
		}

		Init();
		m_control_button->SetPosition (position().x(), position().y());
	}

	SliderBar::~SliderBar()
	{
	}

	bool SliderBar::Update (const UpdateRequest& request)
	{
		if(request.id() == Predefined) {

			switch (request.type()) {

				case FormPosition: {
					const Point* pos = static_cast<const Point*>(request.data());
					m_control_button->SetPosition(m_control_button->position().x() + (pos->x() - position().x()),
							m_control_button->position().y() + (pos->y() - position().y()));
					return true;
				}

				case FormSize: {
					const Size* size_p = static_cast<const Size*>(request.data());
					Orientation shadedir =
					        size_p->width() < size_p->height() ?
					                Horizontal : Vertical;
					const Color& color = themes()->scroll.item;
					short shadetop = themes()->scroll.shadetop;
					short shadedown = themes()->scroll.shadedown;

					GenerateShadedFormBuffers(size_p, round_type(),
					        radius(), color, shadetop, shadedown, shadedir, 5,
					        m_inner_buffer.get(), m_outer_buffer.get(), 0);
					return true;
				}

				case FormRoundRadius: {
					const Size* size_p = &(size());
					Orientation shadedir =
					        size_p->width() < size_p->height() ?
					                Horizontal : Vertical;
					const float* radius_p = static_cast<const float*>(request.data());
					const Color& color = themes()->scroll.item;
					short shadetop = themes()->scroll.shadetop;
					short shadedown = themes()->scroll.shadedown;

					GenerateShadedFormBuffers(size_p, round_type(),
					        *radius_p, color, shadetop, shadedown, shadedir, 5,
					        m_inner_buffer.get(), m_outer_buffer.get(), 0);
					return true;
				}

				default:
					return true;
			}

		} else {
			return false;
		}
	}

	void SliderBar::Draw (RedrawEvent* event)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(), position().y(), z());

		DrawShadedInnerBuffer(m_inner_buffer.get());

		// draw outline
		unsigned char tcol[4] = { themes()->scroll.outline.r(),
		        themes()->scroll.outline.g(),
		        themes()->scroll.outline.b(),
		        themes()->scroll.outline.a()};

		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		glColor4ubv(tcol);

		DrawOutlineBuffer(m_outer_buffer.get());

		glPopMatrix();

		DispatchRender(m_control_button.get());
	}

	void SliderBar::MouseMoveEvent (MouseEvent* event)
	{
		if(m_control_button->down()) {
				dispatch_mouse_move_event(m_control_button.get(), event);

				int value = 0;
				if(orientation() == Vertical) {
					value = (m_control_button->position().y() - position().y()) / (float)GetSpace() * (maximum() - minimum());
				} else {
					value = (m_control_button->position().x() - position().x()) / (float)GetSpace() * (maximum() - minimum());
				}

				set_value (value);
				//m_slider_moved.fire(value);

				return;
			}

		if(contain(event->position())) {
			dispatch_mouse_move_event(m_control_button.get(), event);
		}
	}

	void SliderBar::MousePressEvent (MouseEvent* event)
	{
		if(m_control_button->down()) {
			dispatch_mouse_press_event(m_control_button.get(), event);
			return;
		}

		if(contain(event->position())) {
			dispatch_mouse_press_event(m_control_button.get(), event);
			if(event->accepted()) return;

			Point inner_pos;
			inner_pos.set_x(event->position().x() - position().x() - m_control_button->size().width() / 2);
			inner_pos.set_y(event->position().y() - position().y() - m_control_button->size().height() / 2);
//			inner_pos.set_x(static_cast<double>(event->position().x() - m_pos.x() - padding().left() - m_control_button->size().width() / 2));
//			inner_pos.set_y(static_cast<double>(event->position().y() - m_pos.y() - padding().bottom() - m_control_button->size().height() / 2));
			int space = GetSpace();
			int value;

			if (orientation() == Vertical) {
				if(inner_pos.y() < space) {
					value = (maximum() - minimum()) * inner_pos.y() / (double) space;
					set_value(value);
					//m_slider_moved.fire(value);
				}
			} else {
				if(inner_pos.x() < space) {
					value = (maximum() - minimum()) * inner_pos.x() / (double) space;
					set_value(value);
					//m_slider_moved.fire(value);
				}
			}
		}

	}

	void SliderBar::MouseReleaseEvent (MouseEvent* event)
	{
		if(m_control_button->down()) {
				dispatch_mouse_release_event(m_control_button.get(), event);
				return;
		}

		if(contain(event->position())) {
			if (event->button() == MouseButtonLeft) {

			}
			dispatch_mouse_release_event(m_control_button.get(), event);
		}
	}


	void SliderBar::set_control_size (size_t size)
	{
		if(orientation() == Vertical) {	// Vertical
			m_control_button->Resize(m_control_button->size().width(), static_cast<unsigned int>(size));
		} else {
			m_control_button->Resize(static_cast<unsigned int>(size), m_control_button->size().height());
		}
	}

	void SliderBar::Init ()
	{
		const Size* size_p = &(size());
		Orientation shadedir =
		        size_p->width() < size_p->height() ? Horizontal : Vertical;

		Color color = themes()->scroll.inner;
		short shadetop = themes()->scroll.shadetop;
		short shadedown = themes()->scroll.shadedown;

		GenerateShadedFormBuffers(size_p, round_type(), radius(),
		        color, shadetop, shadedown, shadedir, 0, m_inner_buffer.get(), m_outer_buffer.get(), 0);
	}

	int SliderBar::GetSpace ()
	{
		int space = 0;

		if(orientation() == Vertical)
			space = size().height() - m_control_button->size().height();
		else	// Horizontal is 0
			space = size().width() - m_control_button->size().width();

		return space;
	}

	// ---------------------------- ScrollBar -------------------------------

	ScrollBar::ScrollBar (Orientation orientation)
			: AbstractSlider(orientation), m_scroll_control(0), m_inner_buffer(0), m_outer_buffer(0)
	{
		m_inner_buffer.reset(new GLArrayBuffer);
		m_outer_buffer.reset(new GLArrayBuffer);

		m_scroll_control.reset(new ScrollControl);

		SetRoundType(RoundAll);
		SetRadius(8);

		if (orientation == Vertical) {	// Vertical
			Resize(16, 400);
			m_scroll_control->Resize(16, 100);
			m_scroll_control->SetRadius(8.0);
			SetExpandY(true);
		} else {
			Resize(400, 16);
			m_scroll_control->Resize(100, 16);
			m_scroll_control->SetRadius(8.0);
			SetExpandX(true);
		}

		m_scroll_control->SetPosition (position().x(), position().y());
		//Update(SliderPropertyValue, 0);
	}

	ScrollBar::~ScrollBar ()
	{
	}

	bool ScrollBar::Update (const UpdateRequest& request)
	{
		if(request.id() == Predefined) {

			switch (request.type()) {

				case FormPosition: {
					const Point* pos = static_cast<const Point*>(request.data());
					m_scroll_control->SetPosition (m_scroll_control->position().x() + (pos->x() - position().x()),
							m_scroll_control->position().y() + (pos->y() - position().y()));

					return true;
				}

				case FormSize: {
					const Size* size_p = static_cast<const Size*>(request.data());
					update_shape(size_p);

					return true;
				}

				case SliderPropertyValue: {
					if(orientation() == Vertical) {	// Vertical is 1
						m_scroll_control->SetPosition (m_scroll_control->position().x(),
								position().y() + value() * get_space() / (float)(maximum() - minimum()));
					} else {	// Horizontal is 0
						m_scroll_control->SetPosition (position().x() + value() * get_space() / (float)(maximum() - minimum()),
								m_scroll_control->position().y());
					}

					return true;
				}

				default:
					return true;
			}

		} else {
			return false;
		}
	}

	void ScrollBar::update_shape(const Size* size)
	{
		Orientation shadedir = orientation() == Horizontal ? Horizontal : Vertical;

		Color color = themes()->scroll.inner;
		short shadetop = themes()->scroll.shadetop;
		short shadedown = themes()->scroll.shadedown;

		GenerateShadedFormBuffers(size, round_type(), radius(), color, shadetop,
		        shadedown, shadedir, 0, m_inner_buffer.get(), m_outer_buffer.get(), 0);
	}

	void ScrollBar::Draw (RedrawEvent* event)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(), position().y(), z());

		DrawShadedInnerBuffer(m_inner_buffer.get());

		// draw outline
		unsigned char tcol[4] = { themes()->scroll.outline.r(),
		        themes()->scroll.outline.g(),
		        themes()->scroll.outline.b(),
		        themes()->scroll.outline.a()};

		tcol[3] = tcol[3] / WIDGET_AA_JITTER;

		glColor4ubv(tcol);

		DrawOutlineBuffer(m_outer_buffer.get());

		glPopMatrix();

		DispatchRender(m_scroll_control.get());
	}

	void ScrollBar::MouseMoveEvent (MouseEvent* event)
	{
		if(m_scroll_control->down()) {
				dispatch_mouse_move_event(m_scroll_control.get(), event);

				int value = 0;
				if(orientation() == Vertical) {
					value = (m_scroll_control->position().y() - position().y()) / (float)get_space() * (maximum() - minimum());
				} else {
					value = (m_scroll_control->position().x() - position().x()) / (float)get_space() * (maximum() - minimum());
				}

				set_value (value);
				//m_slider_moved.fire(value);

				return;
			}

		if(contain(event->position())) {
			dispatch_mouse_move_event(m_scroll_control.get(), event);
		}
	}

	void ScrollBar::MousePressEvent (MouseEvent* event)
	{
		if(m_scroll_control->down()) {
			dispatch_mouse_press_event(m_scroll_control.get(), event);
			return;
		}

		if(contain(event->position())) {
			dispatch_mouse_press_event(m_scroll_control.get(), event);
			if(event->accepted()) return;

			Point inner_pos;
			inner_pos.set_x(event->position().x() - position().x() - m_scroll_control->size().width() / 2);
			inner_pos.set_y(event->position().y() - position().y() - m_scroll_control->size().height() / 2);
//			inner_pos.set_x(static_cast<double>(event->position().x() - m_pos.x() - padding().left() - m_scroll_control->size().width() / 2));
//			inner_pos.set_y(static_cast<double>(event->position().y() - m_pos.y() - padding().bottom() - m_scroll_control->size().height() / 2));
			int space = get_space();
			int value;

			if (orientation() == Vertical) {
				if(inner_pos.y() < space) {
					value = (maximum() - minimum()) * inner_pos.y() / (double) space;
					set_value(value);
					//m_slider_moved.fire(value);
				}
			} else {
				if(inner_pos.x() < space) {
					value = (maximum() - minimum()) * inner_pos.x() / (double) space;
					set_value(value);
					//m_slider_moved.fire(value);
				}
			}
		}

	}

	void ScrollBar::MouseReleaseEvent (MouseEvent* event)
	{
		if(m_scroll_control->down()) {
				dispatch_mouse_release_event(m_scroll_control.get(), event);
				return;
		}

		if(contain(event->position())) {
			if (event->button() == MouseButtonLeft) {

			}
			dispatch_mouse_release_event(m_scroll_control.get(), event);
		}
	}

	int ScrollBar::get_space ()
	{
		int space = 0;

		if(orientation() == Vertical)
			space = size().height() - m_scroll_control->size().height();
		else	// Horizontal is 0
			space = size().width() - m_scroll_control->size().width();

		return space;
	}

}
