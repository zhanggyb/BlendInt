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

#include <stdexcept>

#include <BILO/TableLayout.hpp>

namespace BILO {

	TableLayout::TableLayout (int rows, int columns)
			: AbstractLayout(), m_rows(rows), m_columns(columns)
	{
		m_vector.resize(rows * columns);
	}

	TableLayout::TableLayout (int rows, int columns, Drawable* parent)
			: AbstractLayout(parent), m_rows(rows), m_columns(columns)
	{
		m_vector.resize(rows * columns);
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
			m_vector[m_rows * row + column + i] = widget;
		}

		for (int i = 0; i < height; i++)
		{
			m_vector[m_rows * (row + i) + column] = widget;
		}

		bind (widget);

		update(WidgetPropertySize, 0);
	}

	void TableLayout::add_layout (AbstractLayout* layout, int row, int column,
	        int width, int height)
	{
		if (m_children.count(layout)) return;

		for (int i = 0; i < width; i++)
		{
			m_vector[m_rows * row + column + i] = layout;
		}

		for (int i = 0; i < height; i++)
		{
			m_vector[m_rows * (row + i) + column] = layout;
		}

		bind (layout);

		update(WidgetPropertySize, 0);
	}

	bool TableLayout::update (int type, const void* property)
	{
		if(m_parent.type == ParentDrawable) {
			const Point* new_pos = &m_pos;
			if (type == WidgetPropertyPosition)
				new_pos = static_cast<const Point*>(property);

			//if (property == WidgetPropertySize) {
			unsigned int total_width = 0;
			unsigned int total_height = 0;
			unsigned int max_widget_height = 0;

			std::vector<Drawable*>::const_iterator it;
			Drawable* child = 0;
			total_width = m_margin.left();
			for (it = m_vector.begin(); it != m_vector.end(); it++) {
				child = *it;
				set_pos_priv(child, new_pos->x() + total_width,
				        new_pos->y() + m_margin.bottom());
				total_width = total_width + child->size().width();
				total_height = std::max(total_height,
				        m_margin.top() + child->size().height() + m_margin.bottom());
				max_widget_height = std::max(max_widget_height,
				        child->size().height());
				total_width += m_space;
			}

			total_width = total_width - m_space + m_margin.right();

			for (it = m_vector.begin(); it != m_vector.end(); it++) {
				child = *it;

				if (m_alignment & AlignTop) {
					set_pos_priv(child, child->pos().x(),
					        new_pos->y()
					                + (total_height
					                        - (m_margin.top()
					                                + child->size().height())));
				} else if (m_alignment & AlignBottom) {
					set_pos_priv(child, child->pos().x(),
					        new_pos->y()
					                + m_margin.bottom());
				} else if (m_alignment & AlignHorizontalCenter) {
					set_pos_priv(child, child->pos().x(),
					        new_pos->y() + m_margin.bottom()
					                + (max_widget_height - child->size().height())
					                        / 2 );
				}

			}

			m_size.set_width(total_width);
			m_size.set_height(total_height);
			//}

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

		return false;
	}

	void TableLayout::render ()
	{
	}

	void TableLayout::generate_layout(const Size* size)
	{

	}

	void TableLayout::generate_default_layout ()
	{

	}

}
