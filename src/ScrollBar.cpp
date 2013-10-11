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

#include <BILO/ScrollBar.hpp>
#include <BILO/Theme.hpp>
#include <BILO/Interface.hpp>

namespace BILO {

	ScrollControl::ScrollControl ()
	: Widget(), m_pressed(false)
	{
		set_padding(0, 0, 0, 0);
		set_roundcorner(RoundCornerAll);
	}

	ScrollControl::ScrollControl(Drawable* parent)
	: Widget(parent), m_pressed(false)
	{
		set_padding(0, 0, 0, 0);
		set_roundcorner(RoundCornerAll);
	}

	ScrollControl::~ScrollControl ()
	{

	}

	bool ScrollControl::update (int type, const void* property)
	{
		switch(type)
		{
			case WidgetPropertySize:
				update_shape(static_cast<const Size*>(property));
				break;
			case WidgetPropertyRoundCorner:
				break;
			default:
				break;
		}

		return true;
	}

	void ScrollControl::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(m_pos.x(),
					 m_pos.y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if(m_pressed) {
			m_buffer.set_index(0);
		} else {
			m_buffer.set_index(2);
		}

		m_buffer.bind();
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(0));
		glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 6, BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glDrawArrays(GL_POLYGON, 0, m_buffer.vertices());

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		m_buffer.unbind();

		// draw outline
		m_buffer.set_index(1);
		unsigned char tcol[4] = { themes()->scroll.outline.r(),
		        themes()->scroll.outline.g(),
		        themes()->scroll.outline.b(),
		        themes()->scroll.outline.a()};

		tcol[3] = tcol[3] / WIDGET_AA_JITTER;

		m_buffer.bind();

		/* outline */
		glEnableClientState(GL_VERTEX_ARRAY);
		glColor4ubv(tcol);
		for (int j = 0; j < WIDGET_AA_JITTER; j++) {
			glTranslatef(jit[j][0], jit[j][1], 0.0f);
			glVertexPointer(2, GL_FLOAT, 0, 0);
			glDrawArrays(GL_QUAD_STRIP, 0, m_buffer.vertices());
			glTranslatef(-jit[j][0], -jit[j][1], 0.0f);
		}
		glDisableClientState(GL_VERTEX_ARRAY);

		m_buffer.unbind();

		glDisable(GL_BLEND);

		glPopMatrix();
	}

	void ScrollControl::move_mouse (MouseEvent* event)
	{
		// if no parent scrollbar, don't react to mouse move
		if(m_parent.type != ParentDrawable) return;

		ScrollBar* parent = dynamic_cast<ScrollBar*>(m_parent.object.drawable);
		if(!parent) return;

		if(m_pressed) {

			if(parent->orientation() == Horizontal) {
				m_pos.set_x(m_position_origin.x() + event->position().x() - m_move_start.x());
				if(m_pos.x() < (parent->pos().x() + parent->padding().left()))
				{
					m_pos.set_x(parent->pos().x() + parent->padding().left());
				}
				if(m_pos.x() >
						(int)(parent->pos().x() + parent->size().width() - parent->padding().right() - m_size.width()))
				{
					m_pos.set_x(parent->pos().x() + parent->size().width() - parent->padding().right() - m_size.width());
				}
			}

			if(parent->orientation() == Vertical) {
				m_pos.set_y(m_position_origin.y() + event->position().y() - m_move_start.y());
				if(m_pos.y() < (parent->pos().y() + parent->padding().bottom())) {
					m_pos.set_y(parent->pos().y() + parent->padding().bottom());
				}
				if(m_pos.y() > (int)(parent->pos().y() + parent->size().height() - parent->padding().top() - m_size.height())) {
					m_pos.set_y(parent->pos().y() + parent->size().height() - parent->padding().top() - m_size.height());
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
				m_position_origin = m_pos;
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

		Orientation shadedir = size->width() < size->height() ? Horizontal : Vertical;

		Color color = themes()->scroll.item;

		if(shadedir)
			m_corner_radius = 0.5f * size->height();
		else
			m_corner_radius = 0.5f * size->width();

		short shadetop = themes()->scroll.shadetop;
		short shadedown = themes()->scroll.shadedown;

		if (shadetop > shadedown)
				shadetop += 20;   /* XXX violates themes... */
		else shadedown += 20;

		int total_num = 0;

		if(shadedir) {
			total_num = generate_vertices(size,
					color,
					shadetop,
					shadedown,
					shadedir,
					inner_v, outer_v);
		} else {	// swap shadetop and shadedown
			total_num = generate_vertices(size,
					color,
					shadedown,
					shadetop,
					shadedir,
					inner_v, outer_v);
		}

		m_buffer.generate(2);

		m_buffer.set_index(0);
		m_buffer.set_property(total_num, sizeof(inner_v[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		m_buffer.bind();
		m_buffer.upload(inner_v);
		m_buffer.unbind();

		float quad_strip[WIDGET_SIZE_MAX * 2 + 2][2]; /* + 2 because the last pair is wrapped */
		//float quad_strip_emboss[WIDGET_SIZE_MAX * 2][2]; /* only for emboss */

		verts_to_quad_strip (inner_v, outer_v, total_num, quad_strip);

		m_buffer.set_index(1);
		m_buffer.set_property(total_num * 2 + 2, sizeof(quad_strip[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

		m_buffer.bind();
		m_buffer.upload(quad_strip);
		m_buffer.unbind();

		color.highlight(color, 5);

		if(shadedir) {
			total_num = generate_vertices(size,
					color,
					shadetop,
					shadedown,
					shadedir,
					inner_v, outer_v);
		} else {	// swap shadetop and shadedown
			total_num = generate_vertices(size,
					color,
					shadedown,
					shadetop,
					shadedir,
					inner_v, outer_v);
		}

		m_buffer.append(1);

		m_buffer.set_index(2);
		m_buffer.set_property(total_num, sizeof(inner_v[0]), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		m_buffer.bind();
		m_buffer.upload(inner_v);
		m_buffer.unbind();

	}

	// ---------------------------- ScrollBar -------------------------------

	ScrollBar::ScrollBar (Orientation orientation)
			: AbstractSlider(orientation), m_scroll_control(0)
	{
		set_padding(2, 2, 2, 2);

		m_scroll_control = new ScrollControl(this);

		if (orientation == Horizontal) {
			resize(400, 20);
			m_scroll_control->resize(100, 16);
		} else if (orientation == Vertical) {
			resize(20, 400);
			m_scroll_control->resize(16, 100);
		}

		m_scroll_control->set_pos (pos().x() + padding().left(), pos().y() + padding().bottom());
	}

	ScrollBar::ScrollBar (Orientation orientation, Drawable* parent)
			: AbstractSlider(orientation, parent), m_scroll_control(0)
	{
		set_padding(2, 2, 2, 2);

		m_scroll_control = new ScrollControl(this);

		if (orientation == Horizontal) {
			resize(400, 20);
			m_scroll_control->resize(100, 16);
		} else if (orientation == Vertical) {
			resize(20, 400);
			m_scroll_control->resize(16, 100);
		}

		m_scroll_control->set_pos (pos().x() + padding().left(), pos().y() + padding().bottom());
	}

	ScrollBar::~ScrollBar ()
	{
	}

	bool ScrollBar::update (int type, const void* property)
	{
		if(type == WidgetPropertyPosition) {
			const Point* new_pos = static_cast<const Point*>(property);
			m_scroll_control->set_pos (new_pos->x() + padding().left(), new_pos->y() + padding().bottom());
			return true;
		}

		if (type == SliderPropertyValue) {
			if(orientation() == Horizontal) {
//				m_slider_control->set_pos (m_pos.x() + m_padding.left() + value() * get_space() / (float)(maximum() - minimum()),
//						m_slider_control->pos().y());
				return true;
			} else if(orientation() == Vertical) {
//				m_slider_control->set_pos (m_slider_control->pos().x(),
//						m_pos.y() + m_padding.bottom() + value() * get_space() / (float)(maximum() - minimum()));
				return true;
			} else {
				return false;
			}
		}

		return Widget::update(type, property);
	}

	void ScrollBar::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(m_pos.x(), m_pos.y(), z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

		Interface::instance()->dispatch_render_event(m_scroll_control);
	}

	void ScrollBar::move_mouse (MouseEvent* event)
	{
		if(m_scroll_control->pressed()) {
				Interface::instance()->dispatch_mouse_move_event(m_scroll_control, event);

				// TODO: events for own

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
			Interface::instance()->dispatch_mouse_press_event(m_scroll_control, event);
			if(event->accepted()) return;

			// TODO: events for own
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

}
