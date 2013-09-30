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
#include <stdexcept>
#include <algorithm>
#include <iostream>

#include <BILO/TableLayout.hpp>
#include <BILO/Interface.hpp>

namespace BILO {

	TableLayout::TableLayout (int rows, int columns)
			: AbstractLayout(), m_rows(rows), m_columns(columns)
	{
		m_vector.resize(rows * columns, 0);
	}

	TableLayout::TableLayout (int rows, int columns, Drawable* parent)
			: AbstractLayout(parent), m_rows(rows), m_columns(columns)
	{
		m_vector.resize(rows * columns, 0);
	}

	TableLayout::~TableLayout ()
	{
	}

	void TableLayout::add_widget (Widget* widget, int row, int column,
	        int width, int height)
	{
		if (m_children.count(widget)) return;

		for (int i = 0; i < width; i++)
		{
			m_vector[m_columns * row + column + i] = widget;
		}

		for (int i = 0; i < height; i++)
		{
			m_vector[m_columns * (row + i) + column] = widget;
		}

		bind (widget);

#ifdef DEBUG

		for(int i = 0; i < m_rows; i++)
		{
			for(int j = 0; j < m_columns; j++)
			{
				if(m_vector[i * m_columns + j]) {
					std::cout << m_vector[i * m_columns + j]->name() << " ";
				}
			}
			std::cout << std::endl;
		}

#endif

		update(WidgetPropertySize, 0);
	}

	void TableLayout::add_layout (AbstractLayout* layout, int row, int column,
	        int width, int height)
	{
		if (m_children.count(layout)) return;

		for (int i = 0; i < width; i++)
		{
			m_vector[m_columns * row + column + i] = layout;
		}

		for (int i = 0; i < height; i++)
		{
			m_vector[m_columns * (row + i) + column] = layout;
		}

		bind (layout);

		update(WidgetPropertySize, 0);
	}

	bool TableLayout::update (int type, const void* property)
	{
		if(m_parent.type == ParentDrawable) {

			return true;

		} else {

			if (type == WidgetPropertySize) {

				if (property) {
					generate_layout(static_cast<const Size*>(property));

				} else {	// this is called when adding widget or layout

					if(m_vector.size() > static_cast<unsigned int>(m_rows * m_columns))
						throw std::out_of_range("Exceed the table size");

					generate_default_layout();

				}

			}
		}

		return true;
	}

	void TableLayout::render ()
	{
		std::vector<Drawable*>::const_iterator it;
		for (it = m_vector.begin(); it != m_vector.end(); it++) {
			if(*it) {
				Interface::instance()->dispatch_render_event(*it);
			}
		}

#ifdef DEBUG
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(m_pos.x(),
					 m_pos.y(),
					 z());
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

		glPopMatrix();
#endif
	}

	void TableLayout::press_key (KeyEvent* event)
	{
		std::vector<Drawable*>::iterator it;
		for(it = m_vector.begin(); it != m_vector.end(); it++)
		{
			Interface::instance()->dispatch_key_press_event(*it, event);
		}
	}

	void TableLayout::press_context_menu (ContextMenuEvent* event)
	{
	}

	void TableLayout::release_context_menu (ContextMenuEvent* event)
	{
	}

	void TableLayout::press_mouse (MouseEvent* event)
	{
		std::vector<Drawable*>::iterator it;
		for(it = m_vector.begin(); it != m_vector.end(); it++)
		{
			if(*it) {
				Interface::instance()->dispatch_mouse_press_event(*it, event);
			}
		}
	}

	void TableLayout::release_mouse (MouseEvent* event)
	{
		std::vector<Drawable*>::iterator it;
		for(it = m_vector.begin(); it != m_vector.end(); it++)
		{
			if (*it) {
				Interface::instance()->dispatch_mouse_release_event(*it, event);
			}
		}
	}

	void TableLayout::move_mouse (MouseEvent* event)
	{
		std::vector<Drawable*>::iterator it;
		for(it = m_vector.begin(); it != m_vector.end(); it++)
		{
			if (*it) {
				Interface::instance()->dispatch_mouse_move_event(*it, event);
			}
		}
	}


	void TableLayout::generate_layout(const Size* size)
	{

	}

	void TableLayout::generate_default_layout ()
	{
		unsigned int total_width = 0;
		unsigned int total_height = 0;

		Drawable* child = 0;

		std::vector<unsigned int> row_height(m_rows);
		std::vector<unsigned int> column_width(m_columns);

		std::fill(row_height.begin(), row_height.end(), 0);
		std::fill(column_width.begin(), column_width.end(), 0);

		for(int i = 0; i < m_rows; i++)
		{
			for (int j = 0; j < m_columns; j++)
			{
				child = m_vector[i * m_columns + j];
				if(child) {
					column_width[j] = std::max(column_width[j], child->size().width());
					row_height[i] = std::max(row_height[i], child->size().height());
				}
			}
		}

		for(int i = 0; i < m_rows; i++) total_height += row_height[i];
		for(int j = 0; j < m_columns; j++) total_width += column_width[j];

		total_width += m_margin.left() + m_margin.right() + m_space * (m_columns - 1);
		total_height += m_margin.top() + m_margin.bottom() + m_space * (m_rows - 1);

		int x = m_pos.x() + m_margin.left();
		int y = m_pos.y() + total_height - m_margin.top();
		for(int i = 0; i < m_rows; i++)
		{
			y = y - row_height[i];

			for (int j = 0; j < m_columns; j++)
			{
				child = m_vector[i * m_columns + j];
				if(!child) continue;

				set_pos_priv(child, x, y);

				if(child->vexpand()) {
					resize_priv (child, child->size().width(), row_height[i]);
				}
				if (child->hexpand()) {
					resize_priv (child, column_width[j], child->size().height());
				}

				if (m_alignment & AlignTop) {
						set_pos_priv(child, child->pos().x(),
						        child->pos().y() + row_height[i] - child->size().height());
				} else if (m_alignment & AlignBottom) {
						set_pos_priv(child, child->pos().x(),
								child->pos().y());
				} else if (m_alignment & AlignHorizontalCenter) {
						set_pos_priv(child, child->pos().x(),
								child->pos().y() + (row_height[i] - child->size().height()) / 2);
				}
				x = x + column_width[j] + m_space;
			}

			x = m_pos.x() + m_margin.left();
			y = y - m_space;
		}

		m_size.set_width(total_width);
		m_size.set_height(total_height);
	}

}
