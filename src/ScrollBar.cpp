/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>

#include <BIL/ScrollBar.hpp>

namespace BILO {

	const float ScrollBar::scroll_circle_vert [20][2] = {
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

	/*
	static const unsigned int scroll_circle_face[14][3] = {
		{0, 1, 2}, {2, 0, 3}, {3, 0, 15}, {3, 15, 4}, {4, 15, 14}, {4, 14, 5}, {5, 14, 13}, {5, 13, 6},
		{6, 13, 12}, {6, 12, 7}, {7, 12, 11}, {7, 11, 8}, {8, 11, 10}, {8, 10, 9}
	};
	*/


	ScrollBar::ScrollBar(Orientation orientation, Drawable* parent)
	: AbstractSlider(orientation, parent)
	{
		if(orientation == Horizontal) {
			resize (400, 25);
		} else if (orientation == Vertical) {
			resize (25, 400);
		}
		update (0);
	}

	ScrollBar::~ScrollBar()
	{
	}

	void ScrollBar::update (int property)
	{
		if (orientation() == Horizontal) {

			int radius = (size_.height() - padding_.top() - padding_.bottom()) / 2;

			if (radius > 0) {
				for (int i = 0; i < 11; i++)
				{
					m_vertex[i][0] = padding_.left() + radius - radius * scroll_circle_vert[i][1];
					m_vertex[i][1] = padding_.bottom() + radius + radius * scroll_circle_vert[i][0];
				}
				for (int i = 10; i < 20; i++)
				{
					m_vertex[i + 1][0] = size_.width() - padding_.right() - radius - radius * scroll_circle_vert[i][1];
					m_vertex[i + 1][1] = padding_.bottom() + radius + radius * scroll_circle_vert[i][0];
				}
				m_vertex[21][0] = size_.width() - padding_.right() - radius - radius * scroll_circle_vert[0][1];
				m_vertex[21][1] = padding_.bottom() + radius + radius * scroll_circle_vert[0][0];
			}

		} else if (orientation() == Vertical) {

			int radius = (size_.width() - padding_.left() - padding_.right()) / 2;

			if (radius > 0) {
				for (int i = 0; i < 11; i++)
				{
					m_vertex[i][0] = padding_.left() + radius - radius * scroll_circle_vert[i][0];
					m_vertex[i][1] = padding_.bottom() + radius - radius * scroll_circle_vert[i][1];
				}
				for (int i = 10; i < 20; i++)
				{
					m_vertex[i + 1][0] = padding_.left() + radius - radius * scroll_circle_vert[i][0];
					m_vertex[i + 1][1] = size_.height() - padding_.top() - radius - radius * scroll_circle_vert[i][1];
				}
				m_vertex[21][0] = padding_.left() + radius - radius * scroll_circle_vert[0][0];
				m_vertex[21][1] = size_.height() - padding_.top() - radius - radius * scroll_circle_vert[0][1];
			}

		}

		//glBindBuffer (GL_ARRAY_BUFFER, m_buffer);
		m_buffer.bind (GL_ARRAY_BUFFER);

		//glBufferData (GL_ARRAY_BUFFER, sizeof(m_vertex), m_vertex, GL_STATIC_DRAW);
		m_buffer.upload (GL_ARRAY_BUFFER, sizeof(m_vertex), m_vertex, GL_STATIC_DRAW);

		//glBindBuffer (GL_ARRAY_BUFFER, 0);
		m_buffer.unbind(GL_ARRAY_BUFFER);

	}

	void ScrollBar::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(pos_.x(),
					 pos_.y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glEnable(GL_POLYGON_SMOOTH);
		//glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

		Theme* tm = Theme::instance();

		glColor4ub(tm->themeUI()->wcol_scroll.item.r(),
				tm->themeUI()->wcol_scroll.item.g(),
				tm->themeUI()->wcol_scroll.item.b(),
				tm->themeUI()->wcol_scroll.item.a());
		//glColor3f (1.0f, 0.2f, 0.9f);
		//glBindBuffer (GL_ARRAY_BUFFER, m_buffer);
		m_buffer.bind(GL_ARRAY_BUFFER);
		glVertexPointer (2, GL_FLOAT, 0, 0);
		glEnableClientState (GL_VERTEX_ARRAY);

		//glDrawArrays(GL_POLYGON, 0, sizeof(m_vertex));
		glDrawArrays(GL_POLYGON, 0, 22);
		//glDisable(GL_POLYGON_SMOOTH);

		glColor4ub(tm->themeUI()->wcol_scroll.outline.r(),
				tm->themeUI()->wcol_scroll.outline.g(),
				tm->themeUI()->wcol_scroll.outline.b(),
				tm->themeUI()->wcol_scroll.outline.a());

		//glEnable(GL_LINE_SMOOTH);
		//glLineWidth(1.25);
		glDrawArrays(GL_LINE_LOOP, 0, 22);
		//glDisable(GL_LINE_SMOOTH);

		glDisableClientState (GL_VERTEX_ARRAY);

		//glBindBuffer (GL_ARRAY_BUFFER, 0);
		m_buffer.unbind(GL_ARRAY_BUFFER);

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
	}

}
