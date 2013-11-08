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

#include <BlendInt/ScrollBar.hpp>
#include <BlendInt/Theme.hpp>
#include <BlendInt/Interface.hpp>

namespace BlendInt {

	ScrollControl::ScrollControl ()
	: Frame(), m_pressed(false)
	{
		set_padding(0, 0, 0, 0);
		set_round_type(CornerAll);
		set_emboss(false);
	}

	ScrollControl::ScrollControl(AbstractWidget* parent)
	: Frame(parent), m_pressed(false)
	{
		set_padding(0, 0, 0, 0);
		set_round_type(CornerAll);
		set_emboss(false);
	}

	ScrollControl::~ScrollControl ()
	{

	}

	void ScrollControl::update (int type, const void* data)
	{
		switch(type)
		{
			case FormPropertySize: {
				const Size* size_p = static_cast<const Size*>(data);
				update_shape(size_p);
				break;
			}
			case FormPropertyCorner:
				break;
			default:
				break;
		}
	}

	void ScrollControl::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(),
					 position().y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if(m_pressed) {
			glbuffer().select(WidgetBufferKeyInner);
		} else {
			glbuffer().select(WidgetBufferKeyLast + 1);
		}

		glbuffer().bind();
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
		glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glDrawArrays(GL_POLYGON, 0, glbuffer().vertices());

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glbuffer().unbind();

		// draw outline
		glbuffer().select(WidgetBufferKeyOuter);
		unsigned char tcol[4] = { themes()->scroll.outline.r(),
		        themes()->scroll.outline.g(),
		        themes()->scroll.outline.b(),
		        themes()->scroll.outline.a()};

		tcol[3] = tcol[3] / WIDGET_AA_JITTER;

		glbuffer().bind();

		/* outline */
		glEnableClientState(GL_VERTEX_ARRAY);
		glColor4ubv(tcol);
		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			glTranslatef(jit[j][0], jit[j][1], 0.0f);
			glVertexPointer(2, GL_FLOAT, 0, 0);
			glDrawArrays(GL_QUAD_STRIP, 0, glbuffer().vertices());
			glTranslatef(-jit[j][0], -jit[j][1], 0.0f);
		}
		glDisableClientState(GL_VERTEX_ARRAY);

		glbuffer().unbind();

		glDisable(GL_BLEND);

		glPopMatrix();
	}

	void ScrollControl::move_mouse (MouseEvent* event)
	{
		// if no parent scrollbar, don't react to mouse move
		if(parent().type != ParentForm) return;

		ScrollBar* parent_obj = dynamic_cast<ScrollBar*>(parent().object.form);
		if(!parent_obj) return;

		if(m_pressed) {

			if(parent_obj->orientation() == Horizontal) {

				set_position(m_position_origin.x() + event->position().x() - m_move_start.x(), position().y());

				if(position().x() < (parent_obj->position().x() + parent_obj->padding().left()))
				{
					set_position(parent_obj->position().x() + parent_obj->padding().left(), position().y());
				}
				if(position().x() >
						(int)(parent_obj->position().x() + parent_obj->size().width() - parent_obj->padding().right() - size().width()))
				{
					set_position(parent_obj->position().x() + parent_obj->size().width() - parent_obj->padding().right() - size().width(), position().y());
				}
			}

			if(parent_obj->orientation() == Vertical) {
				set_position(position().x(), m_position_origin.y() + event->position().y() - m_move_start.y());
				if(position().y() < (parent_obj->position().y() + parent_obj->padding().bottom())) {

					set_position(position().x(), parent_obj->position().y() + parent_obj->padding().bottom());
				}
				if(position().y() > (int)(parent_obj->position().y() + parent_obj->size().height() - parent_obj->padding().top() - size().height())) {

					set_position(position().x(), parent_obj->position().y() + parent_obj->size().height() - parent_obj->padding().top() - size().height());

				}
			}

			event->accept(this);
			return;
		}
	}

	void ScrollControl::press_mouse (MouseEvent* event)
	{
		if(contain(event->position())) {
			if (event->button() == MouseButtonLeft) {
				m_pressed = true;
				m_move_start.set_x(event->position().x());
				m_move_start.set_y(event->position().y());
				m_position_origin = position();
				event->accept(this);
			}
		}
	}

	void ScrollControl::release_mouse (MouseEvent* event)
	{
		if (event->button() == MouseButtonLeft) {

		}
		m_pressed = false;
	}

	void ScrollControl::update_shape(const Size* size)
	{
		float outer_v[WIDGET_SIZE_MAX][2];	// vertices for drawing outline
		float inner_v[WIDGET_SIZE_MAX][6];	// vertices for drawing inner

		VerticesSum vert_sum;

		Orientation shadedir = size->width() < size->height() ? Horizontal : Vertical;

		Color color = themes()->scroll.item;

		if(shadedir)
			set_radius(0.5f * size->height());
		else
			set_radius(0.5f * size->width());

		short shadetop = themes()->scroll.shadetop;
		short shadedown = themes()->scroll.shadedown;

		if (shadetop > shadedown)
				shadetop += 20;   /* XXX violates themes... */
		else shadedown += 20;

		if(shadedir) {
			vert_sum = generate_vertices(size,
					color,
					shadetop,
					shadedown,
					shadedir,
					inner_v, outer_v);
		} else {	// swap shadetop and shadedown
			vert_sum = generate_vertices(size,
					color,
					shadedown,
					shadetop,
					shadedir,
					inner_v, outer_v);
		}

		glbuffer().create(WidgetBufferKeyInner);
		glbuffer().select(WidgetBufferKeyInner);
		glbuffer().set_property(vert_sum.total, sizeof(inner_v[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		glbuffer().bind();
		glbuffer().upload(inner_v);
		glbuffer().unbind();

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */
		//float quad_strip_emboss[WIDGET_SIZE_MAX * 2][2]; /* only for emboss */

		verts_to_quad_strip (inner_v, outer_v, vert_sum.total, quad_strip);

		glbuffer().create(WidgetBufferKeyOuter);
		glbuffer().select(WidgetBufferKeyOuter);
		glbuffer().set_property(vert_sum.total * 2 + 2, sizeof(quad_strip[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

		glbuffer().bind();
		glbuffer().upload(quad_strip);
		glbuffer().unbind();

		color.highlight(color, 5);

		if(shadedir) {
			vert_sum = generate_vertices(size,
					color,
					shadetop,
					shadedown,
					shadedir,
					inner_v, outer_v);
		} else {	// swap shadetop and shadedown
			vert_sum = generate_vertices(size,
					color,
					shadedown,
					shadetop,
					shadedir,
					inner_v, outer_v);
		}

		glbuffer().create(WidgetBufferKeyLast + 1);
		glbuffer().select(WidgetBufferKeyLast + 1);
		glbuffer().set_property(vert_sum.total, sizeof(inner_v[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		glbuffer().bind();
		glbuffer().upload(inner_v);
		glbuffer().unbind();

	}

	// ---------------------------- SliderBar -------------------------------

	SliderBar::SliderBar(Orientation orientation)
	: Slider(orientation)
	{
		set_padding(0, 0, 0, 0);
		set_round_type(CornerAll);

		set_control_size(50);

		if (orientation == Vertical) {	// Vertical
			set_radius(slide_button()->size().width()/2);
			resize(slide_button()->size().width(), 400);
			set_expand_y(true);
		} else {
			set_radius(slide_button()->size().height());
			resize(400, slide_button()->size().height());
			set_expand_x(true);
		}

		update(SliderPropertyValue, 0);
	}

	SliderBar::SliderBar(Orientation orientation, AbstractWidget* parent)
	: Slider(orientation, parent)
	{
		set_padding(0, 0, 0, 0);
		set_round_type(CornerAll);

		set_control_size(50);

		if (orientation == Vertical) {	// Vertical
			set_radius(slide_button()->size().width()/2);
			resize(slide_button()->size().width(), 400);
			set_expand_y(true);
		} else {
			set_radius(slide_button()->size().height());
			resize(400, slide_button()->size().height());
			set_expand_x(true);
		}

		update(SliderPropertyValue, 0);
	}

	SliderBar::~SliderBar()
	{

	}

	void SliderBar::update(int type, const void* data)
	{
		if(type == FormPropertySize) {
			const Size* size_p = static_cast<const Size*>(data);
			update_shape(size_p);
			return;
		}

		Slider::update(type, data);
	}

	void SliderBar::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(), position().y(), z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		draw_shaded_gl_buffer(WidgetBufferKeyInner);

		// draw outline
		unsigned char tcol[4] = { themes()->scroll.outline.r(),
		        themes()->scroll.outline.g(),
		        themes()->scroll.outline.b(),
		        themes()->scroll.outline.a()};

		tcol[3] = tcol[3] / WIDGET_AA_JITTER;
		glColor4ubv(tcol);
		draw_gl_buffer_anti_alias(WidgetBufferKeyOuter);

		glColor4f(1.0f, 1.0f, 1.0f, 0.02f);
		draw_gl_buffer_anti_alias(WidgetBufferKeyEmboss);

		glDisable(GL_BLEND);

		glPopMatrix();

		interface()->dispatch_render_event(m_slide_button);
	}


	void SliderBar::update_shape (const Size* size)
	{
		float inner_v[WIDGET_SIZE_MAX][6];	// vertices for drawing inner
		float outer_v[WIDGET_SIZE_MAX][2];	// vertices for drawing outline

		VerticesSum vert_sum;

		Orientation shadedir = orientation() == Horizontal ? Horizontal : Vertical;

		Color color = themes()->scroll.inner;
		short shadetop = themes()->scroll.shadetop;
		short shadedown = themes()->scroll.shadedown;

		if(shadedir)
			vert_sum = generate_vertices(size, color, shadetop, shadedown, shadedir, inner_v, outer_v);
		else					// swap shadetop and shadedown
			vert_sum = generate_vertices(size, color, shadedown, shadetop, shadedir, inner_v, outer_v);

		glbuffer().create(WidgetBufferKeyInner);
		glbuffer().select(WidgetBufferKeyInner);
		glbuffer().set_property(vert_sum.total, sizeof(inner_v[0]),
		        GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		glbuffer().bind();
		glbuffer().upload(inner_v);
		glbuffer().unbind();

		// the quad strip for outline

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */

		verts_to_quad_strip(inner_v, outer_v, vert_sum.total, quad_strip);

		glbuffer().create(WidgetBufferKeyOuter);
		glbuffer().select(WidgetBufferKeyOuter);
		glbuffer().set_property(vert_sum.total * 2 + 2, sizeof(quad_strip[0]),
		        GL_ARRAY_BUFFER, GL_STATIC_DRAW);

		glbuffer().bind();
		glbuffer().upload(quad_strip);
		glbuffer().unbind();

		float quad_strip_emboss[WIDGET_SIZE_MAX * 2][2]; /* only for emboss */

		verts_to_quad_strip_open(outer_v, vert_sum.half, quad_strip_emboss);

		glbuffer().create(WidgetBufferKeyEmboss);
		glbuffer().select(WidgetBufferKeyEmboss);
		glbuffer().set_property(vert_sum.half * 2, sizeof(quad_strip_emboss[0]),
		        GL_ARRAY_BUFFER, GL_STATIC_DRAW);

		glbuffer().bind();
		glbuffer().upload(quad_strip_emboss);
		glbuffer().unbind();

	}
	// ---------------------------- ScrollBar -------------------------------

	ScrollBar::ScrollBar (Orientation orientation)
			: AbstractSlider(orientation), m_scroll_control(0)
	{
		set_padding(0, 0, 0, 0);
		set_round_type(CornerAll);
		set_radius(8);

		m_scroll_control = new ScrollControl(this);

		if (orientation == Vertical) {	// Vertical
			resize(16, 400);
			m_scroll_control->resize(16, 100);
			set_expand_y(true);
		} else {
			resize(400, 16);
			m_scroll_control->resize(100, 16);
			set_expand_x(true);
		}

		m_scroll_control->set_position (position().x() + padding().left(), position().y() + padding().bottom());
		update(SliderPropertyValue, 0);
	}

	ScrollBar::ScrollBar (Orientation orientation, AbstractWidget* parent)
			: AbstractSlider(orientation, parent), m_scroll_control(0)
	{
		set_padding(0, 0, 0, 0);
		set_round_type(CornerAll);
		set_radius(8);

		m_scroll_control = new ScrollControl(this);

		if (orientation == Vertical) {	// Vertical
			resize(16, 400);
			m_scroll_control->resize(16, 100);
			set_expand_y(true);
		} else {
			resize(400, 16);
			m_scroll_control->resize(100, 16);
			set_expand_x(true);
		}

		m_scroll_control->set_position (position().x() + padding().left(), position().y() + padding().bottom());
		update(SliderPropertyValue, 0);
	}

	ScrollBar::~ScrollBar ()
	{
	}

	void ScrollBar::update (int type, const void* data)
	{
		switch (type) {

			case FormPropertyPosition: {
				const Point* new_pos = &(position());
				m_scroll_control->set_position (new_pos->x() + padding().left(), new_pos->y() + padding().bottom());
				break;
			}

			case FormPropertySize: {
				const Size* size_p = static_cast<const Size*>(data);
				update_shape(size_p);
				break;
			}

			case SliderPropertyValue: {
				if(orientation() == Vertical) {	// Vertical is 1
					m_scroll_control->set_position (m_scroll_control->position().x(),
							position().y() + padding().bottom() + value() * get_space() / (float)(maximum() - minimum()));
				} else {	// Horizontal is 0
					m_scroll_control->set_position (position().x() + padding().left() + value() * get_space() / (float)(maximum() - minimum()),
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
			vert_sum = generate_vertices(size, color, shadetop, shadedown, shadedir, inner_v, outer_v);
		else					// swap shadetop and shadedown
			vert_sum = generate_vertices(size, color, shadedown, shadetop, shadedir, inner_v, outer_v);

		glbuffer().create(WidgetBufferKeyInner);
		glbuffer().select(WidgetBufferKeyInner);
		glbuffer().set_property(vert_sum.total, sizeof(inner_v[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		glbuffer().bind();
		glbuffer().upload(inner_v);
		glbuffer().unbind();

		// the quad strip for outline

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */

		verts_to_quad_strip (inner_v, outer_v, vert_sum.total, quad_strip);

		glbuffer().create(WidgetBufferKeyOuter);
		glbuffer().select(WidgetBufferKeyOuter);
		glbuffer().set_property(vert_sum.total * 2 + 2, sizeof(quad_strip[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

		glbuffer().bind();
		glbuffer().upload(quad_strip);
		glbuffer().unbind();

		float quad_strip_emboss[WIDGET_SIZE_MAX * 2][2]; /* only for emboss */

		verts_to_quad_strip_open(outer_v, vert_sum.half, quad_strip_emboss);

		glbuffer().create(WidgetBufferKeyEmboss);
		glbuffer().select(WidgetBufferKeyEmboss);
		glbuffer().set_property(vert_sum.half * 2, sizeof(quad_strip_emboss[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

		glbuffer().bind();
		glbuffer().upload(quad_strip_emboss);
		glbuffer().unbind();
	}

	void ScrollBar::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(position().x(), position().y(), z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glbuffer().select(WidgetBufferKeyInner);

		glbuffer().bind();

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		
		glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
		glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glDrawArrays(GL_POLYGON, 0, glbuffer().vertices());

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glbuffer().unbind();

		// draw outline
		glbuffer().select(WidgetBufferKeyOuter);
		unsigned char tcol[4] = { themes()->scroll.outline.r(),
		        themes()->scroll.outline.g(),
		        themes()->scroll.outline.b(),
		        themes()->scroll.outline.a()};

		tcol[3] = tcol[3] / WIDGET_AA_JITTER;

		glbuffer().bind();

		glEnableClientState(GL_VERTEX_ARRAY);
		glColor4ubv(tcol);
		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			glTranslatef(jit[j][0], jit[j][1], 0.0f);
			glVertexPointer(2, GL_FLOAT, 0, 0);
			glDrawArrays(GL_QUAD_STRIP, 0, glbuffer().vertices());
			glTranslatef(-jit[j][0], -jit[j][1], 0.0f);
		}
		glDisableClientState(GL_VERTEX_ARRAY);

		glbuffer().unbind();

		glbuffer().select(WidgetBufferKeyEmboss);	// emboss
		glbuffer().bind();

		glEnableClientState(GL_VERTEX_ARRAY);

		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			glTranslatef(jit[j][0], jit[j][1], 0.0f);

			glColor4f(1.0f, 1.0f, 1.0f, 0.02f);
			glVertexPointer(2, GL_FLOAT, 0, 0);
			glDrawArrays(GL_QUAD_STRIP, 0, glbuffer().vertices());

			glTranslatef(-jit[j][0], -jit[j][1], 0.0f);
		}

		glDisableClientState(GL_VERTEX_ARRAY);

		glbuffer().unbind();

		glDisable(GL_BLEND);

		glPopMatrix();

		Interface::instance()->dispatch_render_event(m_scroll_control);
	}

	void ScrollBar::move_mouse (MouseEvent* event)
	{
		if(m_scroll_control->pressed()) {
				Interface::instance()->dispatch_mouse_move_event(m_scroll_control, event);

				int value = 0;
				if(orientation() == Vertical) {
					value = (m_scroll_control->position().y() - position().y() - padding().bottom()) / (float)get_space() * (maximum() - minimum());
				} else {
					value = (m_scroll_control->position().x() - position().x() - padding().left()) / (float)get_space() * (maximum() - minimum());
				}

				set_value (value);
				m_slider_moved.fire(value);

				return;
			}

		if(contain(event->position())) {
			Interface::instance()->dispatch_mouse_move_event(m_scroll_control, event);
		}
	}

	void ScrollBar::press_mouse (MouseEvent* event)
	{
		if(m_scroll_control->pressed()) {
			Interface::instance()->dispatch_mouse_press_event(m_scroll_control, event);
			return;
		}

		if(contain(event->position())) {
			interface()->dispatch_mouse_press_event(m_scroll_control, event);
			if(event->accepted()) return;

			Coord2d inner_pos;
			inner_pos.set_x(static_cast<double>(event->position().x() - position().x() - padding().left() - m_scroll_control->size().width() / 2));
			inner_pos.set_y(static_cast<double>(event->position().y() - position().y() - padding().bottom() - m_scroll_control->size().height() / 2));
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

	void ScrollBar::release_mouse (MouseEvent* event)
	{
		if(m_scroll_control->pressed()) {
				Interface::instance()->dispatch_mouse_release_event(m_scroll_control, event);
				return;
		}

		if(contain(event->position())) {
			if (event->button() == MouseButtonLeft) {

			}
			Interface::instance()->dispatch_mouse_release_event(m_scroll_control, event);
		}
	}

	int ScrollBar::get_space ()
	{
		int space = 0;

		if(orientation() == Vertical)
			space = size().height() - padding().top() - padding().bottom() - m_scroll_control->size().height();
		else	// Horizontal is 0
			space = size().width() - padding().left() - padding().right() - m_scroll_control->size().width();

		return space;
	}

}
