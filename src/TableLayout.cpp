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

#include <BILO/TableLayout.hpp>

namespace BILO {

	TableLayout::TableLayout (int rows, int columns)
			: AbstractLayout(), m_rows(rows), m_columns(columns), m_array(0)
	{
	}

	TableLayout::TableLayout (int rows, int columns, Drawable* parent)
			: AbstractLayout(parent), m_rows(rows), m_columns(columns), m_array(
			        0)
	{
	}

	BILO::TableLayout::~TableLayout ()
	{
	}

	void TableLayout::add_widget (Widget* widget)
	{
	}

	void TableLayout::add_widget (Widget* widget, int row, int column,
	        int width, int height)
	{
	}

	void TableLayout::add_layout (AbstractLayout* layout)
	{
	}

	void TableLayout::add_layout (AbstractLayout* layout, int row, int column,
	        int width, int height)
	{
	}

	bool TableLayout::remove (Drawable* object)
	{
		if (!m_children.count(object)) return false;

		return true;
	}

	bool TableLayout::erase (Drawable* object)
	{
		if (!m_children.count(object)) return false;

		return true;
	}

	void TableLayout::render ()
	{
	}

}
