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

#ifndef _BLENDINT_TABLELAYOUT_HPP_
#define _BLENDINT_TABLELAYOUT_HPP_

#include <BlendInt/Gui/AbstractLayout.hpp>

namespace BlendInt {

	/**
	 * The table layout
	 */
	class TableLayout: public AbstractLayout
	{
		DISALLOW_COPY_AND_ASSIGN(TableLayout);

	public:

		TableLayout (int rows, int columns);

		virtual ~TableLayout ();

		void add_widget (AbstractWidget* widget, int row, int column, int width = 1, int height = 1);

		void add_layout (AbstractLayout* layout, int row, int column, int width = 1, int height = 1);

		int rows () const {return m_rows;}

		int columns () const {return m_columns;}

		void debug_print ();

	protected:

		virtual void Update (int property_type);

		virtual void Update (int type, const void* property);

		virtual void Draw ();

		virtual void AddItem (AbstractWidget* object);

		virtual void RemoveItem (AbstractWidget* object);

	private:

		bool generate_layout (const Size* size);

		void generate_default_layout ();

		/**
		 * @brief calculate and return the fixed width of a column
		 * @param column
		 * @return -1 if all cells are horizontally expandable
		 */
		int fixed_column_width (int column);

		/**
		 * @brief calculate and return the fixed height of a row
		 * @param row
		 * @return -1 if all cells are vertically expandable
		 */
		int fixed_row_height (int row);

		/**
		 * @brief the total fixed width of this layout
		 * @param[out] width the minimal total fixed width
		 * @return how many columns have fixed width
		 */
		int total_fixed_width (int *width);

		/**
		 * @brief the total fixed height of this layout
		 * @param[out] height the minimal total fixed height
		 * @return how many columns have fixed width
		 */
		int total_fixed_height (int *height);

		unsigned int minimal_column_width (int column);

		unsigned int minimal_row_height (int row);

		int m_rows;

		int m_columns;

		std::vector<AbstractWidget*> m_items;
	};
}

#endif /* TABLELAYOUT_HPP_ */
