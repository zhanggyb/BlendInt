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
#include <assert.h>
#include <algorithm>

#include <iostream>

#include <BILO/Widget.hpp>
#include <BILO/Types.hpp>
#include <BILO/Coord.hpp>

#include <BILO/Vector.hpp>

#include <BILO/Utilities-inl.hpp>

#include <BILO/Interface.hpp>

#define WIDGET_AA_JITTER 8

namespace BILO {

	static const float jit[WIDGET_AA_JITTER][2] = {
			{ 0.468813, -0.481430}, {-0.155755, -0.352820},
			{ 0.219306, -0.238501}, {-0.393286, -0.110949},
			{-0.024699,  0.013908}, { 0.343805,  0.147431},
			{-0.272855,  0.269918}, { 0.095909,  0.388710}
	};

	const float Widget::quarter_corner_vertexes[9][2] =
	{
			{0.0, 0.0}, {0.195, 0.02}, {0.383, 0.067},
			{0.55, 0.169}, {0.707, 0.293}, {0.831, 0.45},
			{0.924, 0.617}, {0.98, 0.805}, {1.0, 1.0}
	};

	Widget::Widget ()
	: Drawable()
	{

	}

	Widget::Widget (Drawable* parent)
			: Drawable(parent)
	{
		// TODO Auto-generated constructor stub
	}

	Widget::~Widget ()
	{
		// TODO Auto-generated destructor stub
	}

	const Padding& Widget::padding () const
	{
		return m_padding;
	}

	void Widget::set_padding (const Padding& padding)
	{
		Padding new_padding = padding;

		if(update(WidgetPropertyPadding, &new_padding)) m_padding = new_padding;
	}

	void Widget::set_padding (int l, int r, int t, int b)
	{
		Padding new_padding (l, r, t, b);
		if(update(WidgetPropertyPadding, &new_padding)) m_padding = new_padding;
	}


	void Widget::press_key (KeyEvent* event)
	{
	}

	void Widget::press_context_menu (ContextMenuEvent* event)
	{
	}

	void Widget::release_context_menu (ContextMenuEvent* event)
	{
	}

	void Widget::press_mouse (MouseEvent* event)
	{
	}

	void Widget::release_mouse (MouseEvent* event)
	{
	}

	void Widget::move_mouse (MouseEvent* event)
	{
	}

	bool Widget::update (int type, const void* property)
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

	void Widget::render ()
	{
#ifdef DEBUG
		std::set<Drawable*>::const_iterator it;
		Drawable *item = 0;
		for (it = m_children.begin(); it != m_children.end(); it++) {
			item = *it;
			Interface::instance()->dispatch_render_event(item);
		}
#endif

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(m_pos.x(),
					 m_pos.y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (m_buffer.is_buffer(0)) {
			Theme* tm = Theme::instance();

			glColor4ub(tm->themeUI()->wcol_regular.inner.r(),
			        tm->themeUI()->wcol_regular.inner.g(),
			        tm->themeUI()->wcol_regular.inner.b(),
			        tm->themeUI()->wcol_regular.inner.a() * 0.5f);

			/* set antialias line */
//		glEnable(GL_LINE_SMOOTH);
//		draw_roundbox(GL_LINE_LOOP);
//		glDisable(GL_LINE_SMOOTH);

			m_buffer.bind(GL_ARRAY_BUFFER);
			glVertexPointer(2, GL_FLOAT, 0, 0);
			glEnableClientState(GL_VERTEX_ARRAY);

			glDrawArrays(GL_POLYGON, 0, 4);

			glColor4ub(tm->themeUI()->wcol_regular.outline.r(),
			        tm->themeUI()->wcol_regular.outline.g(),
			        tm->themeUI()->wcol_regular.outline.b(),
			        tm->themeUI()->wcol_regular.outline.a());

			glDrawArrays(GL_LINE_LOOP, 0, 4);

			glDisableClientState(GL_VERTEX_ARRAY);

			m_buffer.unbind(GL_ARRAY_BUFFER);
		}

		glDisable(GL_BLEND);

		glPopMatrix();
	}


	void Widget::update_shape(const Size* size)
	{
		m_buffer.generate (1);

		/*
		if(m_corner_radius > 1.0) {	// Do not use radius, bad AA effect

			for (int i = 0; i < 9; i++)
			{
				m_vertexes[i][0] = (1.0 - quarter_corner_vertexes[8 - i][0]) * m_corner_radius;
				m_vertexes[i][1] = quarter_corner_vertexes[8 - i][1] * m_corner_radius;
			}

			for (int i = 0; i < 9; i++)
			{
				m_vertexes[9 + i][0] = size_.width() - (1.0 - quarter_corner_vertexes[i][0]) * m_corner_radius;
				m_vertexes[9 + i][1] = quarter_corner_vertexes[i][1] * m_corner_radius;
			}

			for (int i = 0; i < 9; i++)
			{
				m_vertexes[18 + i][0] = size_.width() - (1.0 - quarter_corner_vertexes[8 - i][0]) * m_corner_radius;
				m_vertexes[18 + i][1] = size_.height() - quarter_corner_vertexes[8 - i][1] * m_corner_radius;
			}

			for (int i = 0; i < 9; i++)
			{
				m_vertexes[27 + i][0] = (1.0 - quarter_corner_vertexes[i][0]) * m_corner_radius;
				m_vertexes[27 + i][1] = size_.height() - quarter_corner_vertexes[i][1] * m_corner_radius;
			}

			m_buffer.bind (GL_ARRAY_BUFFER);
			m_buffer.upload (GL_ARRAY_BUFFER, sizeof(m_vertexes), m_vertexes, GL_STATIC_DRAW);
			m_buffer.unbind (GL_ARRAY_BUFFER);

		} else {
			m_vertexes[0][0] = 0.0;
			m_vertexes[0][1] = 0.0;
			m_vertexes[1][0] = size_.width();
			m_vertexes[1][1] = 0.0;
			m_vertexes[2][0] = size_.width();
			m_vertexes[2][1] = size_.height();
			m_vertexes[3][0] = 0.0;
			m_vertexes[3][1] = size_.height();
			m_buffer.bind (GL_ARRAY_BUFFER);
			m_buffer.upload (GL_ARRAY_BUFFER, sizeof(m_vertexes[0]) * 4, m_vertexes, GL_STATIC_DRAW);
			m_buffer.unbind (GL_ARRAY_BUFFER);
		}
		*/

		float vertexes[4][2];
		vertexes[0][0] = 0;
		vertexes[0][1] = 0;
		vertexes[1][0] = size->width();
		vertexes[1][1] = 0;
		vertexes[2][0] = size->width();
		vertexes[2][1] = size->height();
		vertexes[3][0] = 0;
		vertexes[3][1] = size->height();

		m_buffer.bind (GL_ARRAY_BUFFER);
		m_buffer.upload (GL_ARRAY_BUFFER, sizeof(vertexes[0]) * 4, vertexes, GL_STATIC_DRAW);
		m_buffer.unbind (GL_ARRAY_BUFFER);

	}

	void Widget::draw_roundbox(int mode)
	{
		int j;

		glEnable(GL_BLEND);
		//glGetFloatv(GL_CURRENT_COLOR, color);
		//color[3] *= 0.125f;
		//glColor4fv(color);

		for (j = 0; j < WIDGET_AA_JITTER; j++) {
			glTranslatef(jit[j][0], jit[j][1], 0.0f);
			draw_box (mode, m_padding.left(),
					m_padding.bottom(),
					m_size.width() - m_padding.left() - m_padding.right(),
					m_size.height() - m_padding.top() - m_padding.bottom());
			glTranslatef(-jit[j][0], -jit[j][1], 0.0f);
		}

		glDisable(GL_BLEND);
	}

	void Widget::draw_box(int mode, float minx, float miny, float maxx, float maxy)
	{
		float vec[7][2] = {{0.195, 0.02}, {0.383, 0.067}, {0.55, 0.169}, {0.707, 0.293},
		                   {0.831, 0.45}, {0.924, 0.617}, {0.98, 0.805}};
		int a;

		/* mult */
		for (a = 0; a < 7; a++) {
			vec[a][0] = vec[a][0] * m_corner_radius;
			vec[a][1] = vec[a][1] * m_corner_radius;
		}

		glBegin(mode);

		/* start with corner right-bottom */
		if (m_roundcorner & RoundCornerBottomRight) {
			glVertex2f(maxx - m_corner_radius, miny);
			for (a = 0; a < 7; a++) {
				glVertex2f(maxx - m_corner_radius + vec[a][0], miny + vec[a][1]);
			}
			glVertex2f(maxx, miny + m_corner_radius);
		}
		else {
			glVertex2f(maxx, miny);
		}

		/* corner right-top */
		if (m_roundcorner & RoundCornerTopRight) {
			glVertex2f(maxx, maxy - m_corner_radius);
			for (a = 0; a < 7; a++) {
				glVertex2f(maxx - vec[a][1], maxy - m_corner_radius + vec[a][0]);
			}
			glVertex2f(maxx - m_corner_radius, maxy);
		}
		else {
			glVertex2f(maxx, maxy);
		}

		/* corner left-top */
		if (m_roundcorner & RoundCornerTopLeft) {
			glVertex2f(minx + m_corner_radius, maxy);
			for (a = 0; a < 7; a++) {
				glVertex2f(minx + m_corner_radius - vec[a][0], maxy - vec[a][1]);
			}
			glVertex2f(minx, maxy - m_corner_radius);
		}
		else {
			glVertex2f(minx, maxy);
		}

		/* corner left-bottom */
		if (m_roundcorner & RoundCornerBottomLeft) {
			glVertex2f(minx, miny + m_corner_radius);
			for (a = 0; a < 7; a++) {
				glVertex2f(minx + vec[a][1], miny + m_corner_radius - vec[a][0]);
			}
			glVertex2f(minx + m_corner_radius, miny);
		}
		else {
			glVertex2f(minx, miny);
		}

		glEnd();

	}

} /* namespace BILO */

