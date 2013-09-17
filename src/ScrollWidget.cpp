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

#include <BIL/ScrollWidget.hpp>
#include <BIL/Theme.hpp>

namespace BIL {

	const float ScrollBar::scroll_circle_vert[16][2] = {
			{1.000000, 0.000000},
			{0.923880, 0.382683},
			{0.707107, 0.707107},
			{0.382684, 0.923879},
			{0.000000, 1.000000},
			{-0.382684, 0.923879},
			{-0.707107, 0.707107},
			{-0.923880, 0.382683},
			{-1.000000, 0.000000},
			{-0.923880, -0.382683},
			{-0.707107, -0.707107},
			{-0.382684, -0.923879},
			{0.000000, -1.000000},
			{0.382684, -0.923879},
			{0.707107, -0.707107},
			{0.923880, -0.382683},
	};

	/*
	static const unsigned int scroll_circle_face[14][3] = {
		{0, 1, 2}, {2, 0, 3}, {3, 0, 15}, {3, 15, 4}, {4, 15, 14}, {4, 14, 5}, {5, 14, 13}, {5, 13, 6},
		{6, 13, 12}, {6, 12, 7}, {7, 12, 11}, {7, 11, 8}, {8, 11, 10}, {8, 10, 9}
	};
	*/

	ScrollBar::ScrollBar(Direction direction, Drawable* parent)
	: Widget(parent), m_direction (Horizontal), m_buffer(0)
	{
		resize (200, 25);

		update ();
	}

	ScrollBar::~ScrollBar()
	{
		glDeleteBuffers (1, &m_buffer);
	}

	void ScrollBar::update ()
	{
		if (m_direction == Horizontal) {

			int radius = (size_.height() - padding_.top() - padding_.bottom()) / 2;

			if (radius > 0) {
				for (int i = 0; i < 9; i++)
				{
					m_vertex[i][0] = padding_.left() + radius - radius * scroll_circle_vert[i][1];
					m_vertex[i][1] = padding_.bottom() + radius + radius * scroll_circle_vert[i][0];
				}
				for (int i = 8; i < 16; i++)
				{
					m_vertex[i + 1][0] = size_.width() - padding_.right() - radius - radius * scroll_circle_vert[i][1];
					m_vertex[i + 1][1] = padding_.bottom() + radius + radius * scroll_circle_vert[i][0];
				}
				m_vertex[17][0] = size_.width() - padding_.right() - radius - radius * scroll_circle_vert[0][1];
				m_vertex[17][1] = padding_.bottom() + radius + radius * scroll_circle_vert[0][0];
			}

			glGenBuffers (1, &m_buffer);
			glBindBuffer (GL_ARRAY_BUFFER, m_buffer);

			glBufferData (GL_ARRAY_BUFFER, sizeof(m_vertex), m_vertex, GL_STATIC_DRAW);

			glBindBuffer (GL_ARRAY_BUFFER, 0);

		} else if (m_direction == Vertical) {

		}
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

		Theme* tm = Theme::instance();

		glColor4ub(tm->themeUI()->wcol_scroll.item.r(),
				tm->themeUI()->wcol_scroll.item.g(),
				tm->themeUI()->wcol_scroll.item.b(),
				tm->themeUI()->wcol_scroll.item.a());
		//glColor3f (1.0f, 0.2f, 0.9f);
		glBindBuffer (GL_ARRAY_BUFFER, m_buffer);
		glVertexPointer (2, GL_FLOAT, 0, 0);
		glEnableClientState (GL_VERTEX_ARRAY);

		//glDrawArrays(GL_POLYGON, 0, sizeof(m_vertex));
		glDrawArrays(GL_POLYGON, 0, 18);

		glColor4ub(tm->themeUI()->wcol_scroll.outline.r(),
				tm->themeUI()->wcol_scroll.outline.g(),
				tm->themeUI()->wcol_scroll.outline.b(),
				tm->themeUI()->wcol_scroll.outline.a());
		glDrawArrays(GL_LINE_LOOP, 0, 18);

		glDisableClientState (GL_VERTEX_ARRAY);

		glBindBuffer (GL_ARRAY_BUFFER, 0);

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

	ScrollWidget::ScrollWidget(Drawable* parent)
	: Widget(parent), m_viewport(0)
	{

	}

	ScrollWidget::~ScrollWidget ()
	{

	}

	void ScrollWidget::set_viewport(Widget* widget)
	{
		// TODO: more code needed here
		add_child(widget);
	}

	void ScrollWidget::render ()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(pos_.x(),
					 pos_.y(),
					 z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glScissor (100, 100, 200, 200);
		glEnable (GL_SCISSOR_TEST);

		glColor3f(0.6f, 1.0f, 0.2f);
		glBegin(GL_POLYGON);
			glVertex2d(50,50);
			glVertex2d(600,50);
			glVertex2d(600,500);
			glVertex2d(50,500);
		glEnd();

		glDisable(GL_SCISSOR_TEST);

		glDisable(GL_BLEND);

		glPopMatrix();
	}

}
