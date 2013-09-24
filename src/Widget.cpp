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

namespace BILO {

	const float Widget::quarter_corner_vertexes[9][2] =
	{
			{0.0, 0.0}, {0.195, 0.02}, {0.383, 0.067},
			{0.55, 0.169}, {0.707, 0.293}, {0.831, 0.45},
			{0.924, 0.617}, {0.98, 0.805}, {1.0, 1.0}
	};

	const float Widget::circle_vertexes[20][2] =
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

	void Widget::press_key (KeyEvent* event)
	{
		event->accept();
	}

	void Widget::press_context_menu (ContextMenuEvent* event)
	{
		event->accept();
	}

	void Widget::release_context_menu (ContextMenuEvent* event)
	{
		event->accept();
	}

	void Widget::press_mouse (MouseEvent* event)
	{
		event->accept();
	}

	void Widget::release_mouse (MouseEvent* event)
	{
		event->accept();
	}

	void Widget::move_mouse (MouseEvent* event)
	{
		event->accept();
	}

	void Widget::update (int property)
	{
		switch(property)
		{
			case WidgetPropertySize:
				update_shape();
				break;
			case WidgetPropertyRoundCorner:
				update_shape();
				break;
			default:
				break;
		}
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

		glTranslatef(pos_.x(),
					 pos_.y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Theme* tm = Theme::instance();

		glColor4ub(tm->themeUI()->wcol_regular.inner.r(),
				tm->themeUI()->wcol_regular.inner.g(),
				tm->themeUI()->wcol_regular.inner.b(),
				tm->themeUI()->wcol_regular.inner.a());

		m_buffer.bind(GL_ARRAY_BUFFER);
		glVertexPointer (2, GL_FLOAT, 0, 0);
		glEnableClientState (GL_VERTEX_ARRAY);

		//glEnable(GL_LINE_SMOOTH);
		//glEnable(GL_POLYGON_SMOOTH);
		glDrawArrays(GL_POLYGON, 0, m_corner_radius > 1.0 ? 36:4);

		glColor4ub(tm->themeUI()->wcol_regular.outline.r(),
				tm->themeUI()->wcol_regular.outline.g(),
				tm->themeUI()->wcol_regular.outline.b(),
				tm->themeUI()->wcol_regular.outline.a());

		glDrawArrays(GL_LINE_LOOP, 0, m_corner_radius > 1.0? 36: 4);
		//glDisable(GL_LINE_SMOOTH);
		//glDisable(GL_POLYGON_SMOOTH);

		glDisableClientState (GL_VERTEX_ARRAY);

		m_buffer.unbind(GL_ARRAY_BUFFER);

		glDisable(GL_BLEND);

		glPopMatrix();
	}


	void Widget::update_shape()
	{
		m_buffer.generate (1);

		if(m_corner_radius > 1.0) {

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

	}

} /* namespace BILO */

