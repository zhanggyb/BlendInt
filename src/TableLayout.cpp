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
#include <stdexcept>
#include <algorithm>
#include <iostream>

#include <BlendInt/TableLayout.hpp>
#include <BlendInt/Interface.hpp>

namespace BlendInt {

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

		update(BasicPropertySize, 0);
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

		update(BasicPropertySize, 0);
	}

	bool TableLayout::update (int type, const void* property)
	{
		if(m_parent.type == ParentDrawable) {

			return true;

		} else {

			if (type == BasicPropertySize) {

				if (property) {

					if (generate_layout(static_cast<const Size*>(property))) return true;
					else return false;

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

	void TableLayout::add_single_widget(Widget* widget)
	{

	}

	bool TableLayout::generate_layout(const Size* size)
	{
		if(size->width() < m_minimal_size.width() ||
				size->height() < m_minimal_size.height())
			return false;

		unsigned int total_width = 0;
		unsigned int total_height = 0;

		Drawable* child = 0;

		std::vector<int> row_height(m_rows, 0);
		std::vector<int> column_width(m_columns, 0);

		int w = 0;
		int h = 0;

		int column_num = 0;
		int row_num = 0;

		column_num = total_fixed_width(&w);
		row_num = total_fixed_height(&h);

		if(size->width() > w) {

			for (int j = 0; j < m_columns; j++)
			{
				column_width[j] = fixed_column_width(j);
				if(column_width[j] > 0)
					total_width += column_width[j];
			}

			if(column_num < m_columns) {
				int single_expandable_width = (size->width() - m_margin.left() - m_margin.right() - m_space * (m_columns - 1)- total_width) / (m_columns - column_num);

				for (int j = 0; j < m_columns; j++)
				{
					if(column_width[j] < 0)
						column_width[j] = single_expandable_width;
				}
			}

		} else {

			for (int j = 0; j < m_columns; j++)
			{
				column_width[j] = fixed_column_width(j);
				if(column_width[j] < 0) {
					column_width[j] = minimal_column_width(j);
				}

				total_width += column_width[j];
			}

			if(column_num > 0) {
				int single_fixed_width_diff = m_size.width() - size->width() / column_num;

				for (int j = 0; j < m_columns; j++)
				{
					if(!(fixed_column_width(j) < 0)) {
						column_width[j] = column_width[j] - single_fixed_width_diff;
					}
				}
			}

		}

		if(size->height() > h) {

			for (int i = 0; i < m_rows; i++)
			{
				row_height[i] = fixed_row_height(i);
				if(row_height[i] > 0)
					total_height += row_height[i];
			}

			if(row_num < m_rows) {
				int single_expandable_height = (size->height() - m_margin.top() - m_margin.bottom() - m_space * (m_rows - 1)- total_height) / (m_rows - row_num);

				for (int i = 0; i < m_rows; i++)
				{
					if(row_height[i] < 0)
						row_height[i] = single_expandable_height;
				}
			}

		} else {

			for (int i = 0; i < m_rows; i++)
			{
				row_height[i] = fixed_row_height(i);
				if(row_height[i] < 0) {
					row_height[i] = minimal_row_height(i);
				}

				total_height += row_height[i];
			}

			if(row_num > 0) {
				int single_fixed_height_diff = m_size.height() - size->height() / row_num;

				for (int i = 0; i < m_rows; i++)
				{
					if(!(fixed_row_height(i) < 0)) {
						row_height[i] = row_height[i] - single_fixed_height_diff;
					}
				}
			}

		}

#ifdef DEBUG
		std::cout << "        ";
		for(int j = 0; j < m_columns; j++)
		{
			std::cout << column_width[j] << " ";
		}
		std::cout << std::endl;

		for(int i = 0; i < m_rows; i++)
		{
			std::cout << row_height[i] << std::endl;
		}
#endif

		int x = m_pos.x() + m_margin.left();
		int y = m_pos.y() + size->height() - m_margin.top();
		for(int i = 0; i < m_rows; i++)
		{
			y = y - row_height[i];

			for (int j = 0; j < m_columns; j++)
			{
				child = m_vector[i * m_columns + j];
				if(!child) continue;

				set_pos_priv(child, x, y);

				if(child->expand_y()) {
					resize_priv (child, child->size().width(), row_height[i]);
				}
				if (child->expand_x()) {
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

		return true;
	}

	void TableLayout::generate_default_layout ()
	{
		unsigned int total_width = 0;
		unsigned int total_height = 0;

		Drawable* child = 0;

		std::vector<unsigned int> row_height(m_rows, 0);
		std::vector<unsigned int> column_width(m_columns, 0);

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

				if(child->expand_y()) {
					resize_priv (child, child->size().width(), row_height[i]);
				}
				if (child->expand_x()) {
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

	int TableLayout::fixed_column_width(int column)
	{
		int fixed_width = -1;	// the return value
		Drawable* child = 0;

		for (int i = 0; i < m_rows; i++)
		{
			child = m_vector[i * m_columns + column];
			if(child) {
				if(!child->expand_x()) {
					fixed_width = std::max(fixed_width, static_cast<int>(child->size().width()));
				}
			}
		}

		return fixed_width;
	}

	int TableLayout::fixed_row_height(int row)
	{
		int fixed_height = -1;	// the return value
		Drawable* child = 0;

		for (int j = 0; j < m_columns; j++)
		{
			child = m_vector[row * m_columns + j];
			if(child) {
				if(!child->expand_y()) {
					fixed_height = std::max(fixed_height, static_cast<int>(child->size().height()));
				}
			}
		}

		return fixed_height;
	}

	unsigned int TableLayout::minimal_column_width(int column)
	{
		unsigned int minimal_width = 0;	// the return value
		Drawable* child = 0;

		for (int j = 0; j < m_columns; j++)
		{
			child = m_vector[j * m_columns + column];
			if(child) {
				minimal_width = std::max(minimal_width, child->minimal_size().width());
			}
		}

		return minimal_width;
	}

	unsigned int TableLayout::minimal_row_height(int row)
	{
		unsigned int minimal_height = 0;	// the return value
		Drawable* child = 0;

		for (int j = 0; j < m_columns; j++)
		{
			child = m_vector[row * m_columns + j];
			if(child) {
				minimal_height = std::max(minimal_height, child->minimal_size().height());
			}
		}

		return minimal_height;

	}

	void TableLayout::debug_print()
	{
		int fixed_width = 0;
		int fixed_height = 0;
		int columns = 0;
		int rows = 0;

		columns = total_fixed_width(&fixed_width);

		rows = total_fixed_height(&fixed_height);

		std::cout << "total fixed width, columns: " << columns << " width: " << fixed_width << std::endl;
		std::cout << "total fixed height, rows: " << rows << " height: " << fixed_height << std::endl;
	}

	int TableLayout::total_fixed_width(int* width)
	{
		int total = 0;
		int columns_with_fixed_width = 0;
		int column_width = 0;

		for(int j = 0; j < m_columns; j++)
		{
			column_width = fixed_column_width(j);
			if(column_width < 0) {
				column_width = minimal_column_width(j);
			} else {
				columns_with_fixed_width++;
			}

			total += column_width;
		}

		total += m_margin.left() + m_margin.right() + m_space * (m_columns - 1);

		*width = total;

		return columns_with_fixed_width;
	}

	int TableLayout::total_fixed_height(int* height)
	{
		int total = 0;
		int rows_with_fixed_height = 0;
		int row_height = 0;

		for(int i = 0; i < m_rows; i++)
		{
			row_height = fixed_row_height(i);
			if(row_height < 0) {
				row_height = minimal_row_height(i);
			} else {
				rows_with_fixed_height++;
			}

			total += row_height;
		}

		total += m_margin.top() + m_margin.bottom() + m_space * (m_rows - 1);

		*height = total;

		return rows_with_fixed_height;
	}

}
