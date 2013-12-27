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

#include <BlendInt/ScrollBar.hpp>
#include <BlendInt/Theme.hpp>

namespace BlendInt {

	ScrollControl::ScrollControl ()
	: AbstractButton()
	{
		m_buffer.reset(new GLBufferMultiple);
		set_round_type(RoundAll);

		Init ();
	}

	ScrollControl::ScrollControl(AbstractWidget* parent)
	: AbstractButton(parent)
	{
		m_buffer.reset(new GLBufferMultiple);
		set_round_type(RoundAll);

		Init ();
	}

	ScrollControl::~ScrollControl ()
	{

	}

	void ScrollControl::Update (int type, const void* data)
	{
		switch(type)
		{
			case FormSize: {
				const Size* size_p = static_cast<const Size*>(data);
				Orientation shadedir =
				        size_p->width() < size_p->height() ?
				                Horizontal : Vertical;
				const Color& color = themes()->scroll.item;
				short shadetop = themes()->scroll.shadetop;
				short shadedown = themes()->scroll.shadedown;

				GenerateShadedFormBuffer(size_p, DefaultBorderWidth(), round_type(),
				        radius(), color, shadetop, shadedown, shadedir, 5,
				        m_buffer.get());
				break;
			}

			case FormRoundRadius: {
				const Size* size_p = &(size());
				Orientation shadedir =
				        size_p->width() < size_p->height() ?
				                Horizontal : Vertical;
				const float* radius_p = static_cast<const float*>(data);
				const Color& color = themes()->scroll.item;
				short shadetop = themes()->scroll.shadetop;
				short shadedown = themes()->scroll.shadedown;

				GenerateShadedFormBuffer(size_p, DefaultBorderWidth(), round_type(),
				        *radius_p, color, shadetop, shadedown, shadedir, 5,
				        m_buffer.get());
				break;
			}

			default:
				break;
		}
	}

	void ScrollControl::Render ()
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
		        themes()->scroll.outline.g(),
		        themes()->scroll.outline.b(),
		        themes()->scroll.outline.a()};

		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		glColor4ubv(tcol);

		draw_outline_buffer(m_buffer.get(), 1);

		glDisable(GL_BLEND);

		glPopMatrix();
	}

	void ScrollControl::MouseMoveEvent (MouseEvent* event)
	{
		// if no parent scrollbar, don't react to mouse move
		if(parent().type != ParentForm) return;

		ScrollBar* parent_obj = dynamic_cast<ScrollBar*>(parent().object.form);
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

		GenerateShadedFormBuffer(size_p, DefaultBorderWidth(), round_type(), radius(),
		        color, shadetop, shadedown, shadedir, 5, m_buffer.get());
	}

	// ---------------------------- SliderBar -------------------------------

	SliderBar::SliderBar(Orientation orientation)
	: AbstractSlider(orientation), m_control_button(0)
	{
		m_buffer.reset(new GLBufferMultiple);
		m_control_button = new ScrollControl(this);

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

	SliderBar::SliderBar(Orientation orientation, AbstractWidget* parent)
	: AbstractSlider(orientation, parent), m_control_button(0)
	{
		m_buffer.reset(new GLBufferMultiple);
		m_control_button = new ScrollControl(this);

		set_round_type(RoundAll);

		set_control_size(50);

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

	void SliderBar::Update (int type, const void* data)
	{
		switch (type) {

			case FormPosition: {
				const Point* pos = static_cast<const Point*>(data);
				m_control_button->SetPosition(m_control_button->position().x() + (pos->x() - position().x()),
						m_control_button->position().y() + (pos->y() - position().y()));
				return;
			}

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(data);
				Orientation shadedir =
				        size_p->width() < size_p->height() ?
				                Horizontal : Vertical;
				const Color& color = themes()->scroll.item;
				short shadetop = themes()->scroll.shadetop;
				short shadedown = themes()->scroll.shadedown;

				GenerateShadedFormBuffer(size_p, DefaultBorderWidth(), round_type(),
				        radius(), color, shadetop, shadedown, shadedir, 5,
				        m_buffer.get());
				break;
			}

			case FormRoundRadius: {
				const Size* size_p = &(size());
				Orientation shadedir =
				        size_p->width() < size_p->height() ?
				                Horizontal : Vertical;
				const float* radius_p = static_cast<const float*>(data);
				const Color& color = themes()->scroll.item;
				short shadetop = themes()->scroll.shadetop;
				short shadedown = themes()->scroll.shadedown;

				GenerateShadedFormBuffer(size_p, DefaultBorderWidth(), round_type(),
				        *radius_p, color, shadetop, shadedown, shadedir, 5,
				        m_buffer.get());
				break;
			}

			default:
				break;
		}
	}

	void SliderBar::Render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(), position().y(), z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		draw_shaded_inner_buffer(m_buffer.get(), 0);

		// draw outline
		unsigned char tcol[4] = { themes()->scroll.outline.r(),
		        themes()->scroll.outline.g(),
		        themes()->scroll.outline.b(),
		        themes()->scroll.outline.a()};

		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		glColor4ubv(tcol);

		draw_outline_buffer(m_buffer.get(), 1);

//		glColor4f(1.0f, 1.0f, 1.0f, 0.02f);
//		draw_gl_buffer_anti_alias(WidgetBufferKeyEmboss);

		glDisable(GL_BLEND);

		glPopMatrix();

		DispatchRender(m_control_button);
	}

	void SliderBar::MouseMoveEvent (MouseEvent* event)
	{
		if(m_control_button->down()) {
				dispatch_mouse_move_event(m_control_button, event);

				int value = 0;
				if(orientation() == Vertical) {
					value = (m_control_button->position().y() - position().y()) / (float)GetSpace() * (maximum() - minimum());
				} else {
					value = (m_control_button->position().x() - position().x()) / (float)GetSpace() * (maximum() - minimum());
				}

				set_value (value);
				m_slider_moved.fire(value);

				return;
			}

		if(contain(event->position())) {
			dispatch_mouse_move_event(m_control_button, event);
		}
	}

	void SliderBar::MousePressEvent (MouseEvent* event)
	{
		if(m_control_button->down()) {
			dispatch_mouse_press_event(m_control_button, event);
			return;
		}

		if(contain(event->position())) {
			dispatch_mouse_press_event(m_control_button, event);
			if(event->accepted()) return;

			Coord2d inner_pos;
			inner_pos.set_x(static_cast<double>(event->position().x() - position().x() - m_control_button->size().width() / 2));
			inner_pos.set_y(static_cast<double>(event->position().y() - position().y() - m_control_button->size().height() / 2));
//			inner_pos.set_x(static_cast<double>(event->position().x() - m_pos.x() - padding().left() - m_control_button->size().width() / 2));
//			inner_pos.set_y(static_cast<double>(event->position().y() - m_pos.y() - padding().bottom() - m_control_button->size().height() / 2));
			int space = GetSpace();
			int value;

			if (orientation() == Vertical) {
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

	void SliderBar::MouseReleaseEvent (MouseEvent* event)
	{
		if(m_control_button->down()) {
				dispatch_mouse_release_event(m_control_button, event);
				return;
		}

		if(contain(event->position())) {
			if (event->button() == MouseButtonLeft) {

			}
			dispatch_mouse_release_event(m_control_button, event);
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

		GenerateShadedFormBuffer(size_p, DefaultBorderWidth(), round_type(), radius(),
		        color, shadetop, shadedown, shadedir, 0, m_buffer.get());
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
			: AbstractSlider(orientation), m_scroll_control(0)
	{
		m_buffer.reset(new GLBufferMultiple);
		SetRoundType(RoundAll);
		SetRadius(8);

		m_scroll_control = new ScrollControl(this);

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
		Update(SliderPropertyValue, 0);
	}

	ScrollBar::ScrollBar (Orientation orientation, AbstractWidget* parent)
			: AbstractSlider(orientation, parent), m_scroll_control(0)
	{
		m_buffer.reset(new GLBufferMultiple);

		SetRoundType(RoundAll);
		SetRadius(8);

		m_scroll_control = new ScrollControl(this);

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
		Update(SliderPropertyValue, 0);
	}

	ScrollBar::~ScrollBar ()
	{
	}

	void ScrollBar::Update (int type, const void* data)
	{
		switch (type) {

			case FormPosition: {
				const Point* pos = static_cast<const Point*>(data);
				m_scroll_control->SetPosition (m_scroll_control->position().x() + (pos->x() - position().x()),
						m_scroll_control->position().y() + (pos->y() - position().y()));
				break;
			}

			case FormSize: {
				const Size* size_p = static_cast<const Size*>(data);
				update_shape(size_p);
				break;
			}

			case SliderPropertyValue: {
				if(orientation() == Vertical) {	// Vertical is 1
					m_scroll_control->SetPosition (m_scroll_control->position().x(),
							position().y() + value() * get_space() / (float)(maximum() - minimum()));
				} else {	// Horizontal is 0
					m_scroll_control->SetPosition (position().x() + value() * get_space() / (float)(maximum() - minimum()),
							m_scroll_control->position().y());
				}
				break;
			}

			default:
				break;
		}
	}

	void ScrollBar::update_shape(const Size* size)
	{
		float inner_v[WIDGET_SIZE_MAX][6];	// vertices for drawing inner
		float outer_v[WIDGET_SIZE_MAX][2];	// vertices for drawing outline

		VerticesSum vert_sum;

		Orientation shadedir = orientation() == Horizontal ? Horizontal : Vertical;

		Color color = themes()->scroll.inner;
		short shadetop = themes()->scroll.shadetop;
		short shadedown = themes()->scroll.shadedown;

		if(shadedir)
			vert_sum = generate_vertices(size, DefaultBorderWidth(), color, shadetop, shadedown, shadedir, inner_v, outer_v);
		else					// swap shadetop and shadedown
			vert_sum = generate_vertices(size, DefaultBorderWidth(), color, shadedown, shadetop, shadedir, inner_v, outer_v);

		m_buffer.get()->Generate(2);
		m_buffer.get()->select(0);
		m_buffer.get()->SetProperty(vert_sum.total, sizeof(inner_v[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		m_buffer.get()->Bind();
		m_buffer.get()->Upload(inner_v);
		m_buffer.get()->Unbind();

		// the quad strip for outline

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */

		verts_to_quad_strip (inner_v, outer_v, vert_sum.total, quad_strip);

		m_buffer.get()->select(1);
		m_buffer.get()->SetProperty(vert_sum.total * 2 + 2, sizeof(quad_strip[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

		m_buffer.get()->Bind();
		m_buffer.get()->Upload(quad_strip);
		m_buffer.get()->Unbind();

		float quad_strip_emboss[WIDGET_SIZE_MAX * 2][2]; /* only for emboss */

		verts_to_quad_strip_open(outer_v, vert_sum.half, quad_strip_emboss);

//		m_buffer.get()->generate(WidgetBufferKeyEmboss);
//		m_buffer.get()->select(WidgetBufferKeyEmboss);
//		m_buffer.get()->set_property(vert_sum.half * 2, sizeof(quad_strip_emboss[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
//
//		m_buffer.get()->bind();
//		m_buffer.get()->upload(quad_strip_emboss);
//		m_buffer.get()->unbind();
	}

	void ScrollBar::Render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(), position().y(), z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		draw_shaded_inner_buffer(m_buffer.get(), 0);

		// draw outline
		unsigned char tcol[4] = { themes()->scroll.outline.r(),
		        themes()->scroll.outline.g(),
		        themes()->scroll.outline.b(),
		        themes()->scroll.outline.a()};

		tcol[3] = tcol[3] / WIDGET_AA_JITTER;

		glColor4ubv(tcol);

		draw_outline_buffer(m_buffer.get(), 1);

		glDisable(GL_BLEND);

		glPopMatrix();

		DispatchRender(m_scroll_control);
	}

	void ScrollBar::MouseMoveEvent (MouseEvent* event)
	{
		if(m_scroll_control->down()) {
				dispatch_mouse_move_event(m_scroll_control, event);

				int value = 0;
				if(orientation() == Vertical) {
					value = (m_scroll_control->position().y() - position().y()) / (float)get_space() * (maximum() - minimum());
				} else {
					value = (m_scroll_control->position().x() - position().x()) / (float)get_space() * (maximum() - minimum());
				}

				set_value (value);
				m_slider_moved.fire(value);

				return;
			}

		if(contain(event->position())) {
			dispatch_mouse_move_event(m_scroll_control, event);
		}
	}

	void ScrollBar::MousePressEvent (MouseEvent* event)
	{
		if(m_scroll_control->down()) {
			dispatch_mouse_press_event(m_scroll_control, event);
			return;
		}

		if(contain(event->position())) {
			dispatch_mouse_press_event(m_scroll_control, event);
			if(event->accepted()) return;

			Coord2d inner_pos;
			inner_pos.set_x(static_cast<double>(event->position().x() - position().x() - m_scroll_control->size().width() / 2));
			inner_pos.set_y(static_cast<double>(event->position().y() - position().y() - m_scroll_control->size().height() / 2));
//			inner_pos.set_x(static_cast<double>(event->position().x() - m_pos.x() - padding().left() - m_scroll_control->size().width() / 2));
//			inner_pos.set_y(static_cast<double>(event->position().y() - m_pos.y() - padding().bottom() - m_scroll_control->size().height() / 2));
			int space = get_space();
			int value;

			if (orientation() == Vertical) {
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

	void ScrollBar::MouseReleaseEvent (MouseEvent* event)
	{
		if(m_scroll_control->down()) {
				dispatch_mouse_release_event(m_scroll_control, event);
				return;
		}

		if(contain(event->position())) {
			if (event->button() == MouseButtonLeft) {

			}
			dispatch_mouse_release_event(m_scroll_control, event);
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
