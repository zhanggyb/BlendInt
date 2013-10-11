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

namespace BILO {

	ScrollControl::ScrollControl ()
	: Widget(), m_hover(false), m_pressed(false)
	{
		set_padding(0, 0, 0, 0);
		set_roundcorner(RoundCornerAll);
	}

	ScrollControl::ScrollControl(Drawable* parent)
	: Widget(parent), m_hover(false), m_pressed(false)
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

		ThemeManager* tm = ThemeManager::instance();

//		glColor4ub(tm->themeUI()->regular.inner.r(),
//		        tm->themeUI()->regular.inner.g(),
//		        tm->themeUI()->regular.inner.b(),
//		        tm->themeUI()->regular.inner.a());

		m_buffer.set_index(0);

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
		unsigned char tcol[4] = { tm->themes()->scroll.outline.r(),
		        tm->themes()->scroll.outline.g(),
		        tm->themes()->scroll.outline.b(),
		        tm->themes()->scroll.outline.a()};

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
		// if no parent slider, don't react to mouse move
		if(m_parent.type != ParentDrawable) return;

		if(m_pressed) {
			m_hover = false;

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

	void ScrollControl::press_mouse (MouseEvent* event)
	{
		if(contain(event->position())) {
			if (event->button() == MouseButtonLeft) {
				m_pressed = true;
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

		ThemeManager* tm = ThemeManager::instance();
		Color color = tm->themes()->scroll.item;

		if(shadedir)
			m_corner_radius = 0.5f * size->height();
		else
			m_corner_radius = 0.5f * size->width();

		short shadetop = tm->themes()->scroll.shadetop;
		short shadedown = tm->themes()->scroll.shadedown;

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
	}

	ScrollBar::ScrollBar (Orientation orientation)
			: AbstractSlider(orientation)
	{
		buffer().generate(1);

		if (orientation == Horizontal) {
			resize(400, 25);
		} else if (orientation == Vertical) {
			resize(25, 400);
		}
	}

	ScrollBar::ScrollBar (Orientation orientation, Drawable* parent)
			: AbstractSlider(orientation, parent)
	{
		buffer().generate(1);

		if (orientation == Horizontal) {
			resize(400, 25);
		} else if (orientation == Vertical) {
			resize(25, 400);
		}
	}

	ScrollBar::~ScrollBar ()
	{
	}

	bool ScrollBar::update (int type, const void* property)
	{
		/*
		 if (orientation() == Horizontal) {

		 int radius = (size_.height() - padding_.top() - padding_.bottom()) / 2;

		 if (radius > 0) {
		 for (int i = 0; i < 11; i++)
		 {
		 m_vertex[i][0] = padding_.left() + radius - radius * circle_vertexes[i][1];
		 m_vertex[i][1] = padding_.bottom() + radius + radius * circle_vertexes[i][0];
		 }
		 for (int i = 10; i < 20; i++)
		 {
		 m_vertex[i + 1][0] = size_.width() - padding_.right() - radius - radius * circle_vertexes[i][1];
		 m_vertex[i + 1][1] = padding_.bottom() + radius + radius * circle_vertexes[i][0];
		 }
		 m_vertex[21][0] = size_.width() - padding_.right() - radius - radius * circle_vertexes[0][1];
		 m_vertex[21][1] = padding_.bottom() + radius + radius * circle_vertexes[0][0];
		 }

		 } else if (orientation() == Vertical) {

		 int radius = (size_.width() - padding_.left() - padding_.right()) / 2;

		 if (radius > 0) {
		 for (int i = 0; i < 11; i++)
		 {
		 m_vertex[i][0] = padding_.left() + radius - radius * circle_vertexes[i][0];
		 m_vertex[i][1] = padding_.bottom() + radius - radius * circle_vertexes[i][1];
		 }
		 for (int i = 10; i < 20; i++)
		 {
		 m_vertex[i + 1][0] = padding_.left() + radius - radius * circle_vertexes[i][0];
		 m_vertex[i + 1][1] = size_.height() - padding_.top() - radius - radius * circle_vertexes[i][1];
		 }
		 m_vertex[21][0] = padding_.left() + radius - radius * circle_vertexes[0][0];
		 m_vertex[21][1] = size_.height() - padding_.top() - radius - radius * circle_vertexes[0][1];
		 }

		 }


		 //glBindBuffer (GL_ARRAY_BUFFER, m_buffer);
		 m_buffer.bind (GL_ARRAY_BUFFER);

		 //glBufferData (GL_ARRAY_BUFFER, sizeof(m_vertex), m_vertex, GL_STATIC_DRAW);
		 m_buffer.upload (GL_ARRAY_BUFFER, sizeof(m_vertex), m_vertex, GL_STATIC_DRAW);

		 //glBindBuffer (GL_ARRAY_BUFFER, 0);
		 m_buffer.unbind(GL_ARRAY_BUFFER);
		 */
		return true;
	}

	void ScrollBar::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(m_pos.x(), m_pos.y(), z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (m_buffer.is_buffer(0)) {

			m_buffer.set_index(0);
			ThemeManager* tm = ThemeManager::instance();

			glColor4ub(tm->themes()->scroll.item.r(),
			        tm->themes()->scroll.item.g(),
			        tm->themes()->scroll.item.b(),
			        tm->themes()->scroll.item.a());
			//glColor3f (1.0f, 0.2f, 0.9f);
			//glBindBuffer (GL_ARRAY_BUFFER, m_buffer);
			m_buffer.bind();
			glVertexPointer(2, GL_FLOAT, 0, 0);
			glEnableClientState(GL_VERTEX_ARRAY);

			//glDrawArrays(GL_POLYGON, 0, sizeof(m_vertex));
			glDrawArrays(GL_POLYGON, 0, 22);
			//glDisable(GL_POLYGON_SMOOTH);

			glColor4ub(tm->themes()->scroll.outline.r(),
			        tm->themes()->scroll.outline.g(),
			        tm->themes()->scroll.outline.b(),
			        tm->themes()->scroll.outline.a());

			//glEnable(GL_LINE_SMOOTH);
			//glLineWidth(1.25);
			glDrawArrays(GL_LINE_LOOP, 0, 22);
			//glDisable(GL_LINE_SMOOTH);

			glDisableClientState(GL_VERTEX_ARRAY);

			//glBindBuffer (GL_ARRAY_BUFFER, 0);
			m_buffer.unbind();

		}

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
	}

}
