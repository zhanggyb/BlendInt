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

#ifndef _BILO_TABLELAYOUT_HPP_
#define _BILO_TABLELAYOUT_HPP_

#include <BILO/AbstractLayout.hpp>

namespace BILO {

	/**
	 * The table layout
	 */
	class TableLayout: public AbstractLayout
	{
		DISALLOW_COPY_AND_ASSIGN(TableLayout);

	public:

		TableLayout (int rows, int columns);

		TableLayout (int rows, int columns, Drawable* parent);

		virtual ~TableLayout ();

		virtual void add_widget (Widget* widget);

		void add_widget (Widget* widget, int row, int column, int width, int height);

		virtual void add_layout (AbstractLayout* layout);

		void add_layout (AbstractLayout* layout, int row, int column, int width, int height);

		/**
		 * @brief remove the object from layout
		 * @param widget
		 * @return
		 *
		 * @warning: after removing from layout, the drawable object will bind to nothing, it must be deleted manually
		 */
		virtual bool remove (Drawable* object);

		/**
		 * @brief erase the object from layout
		 * @param widget
		 * @return
		 *
		 * Same as remove but will delete the child object
		 */
		virtual bool erase (Drawable* object);

	protected:

		virtual void render ();

	private:

		int m_rows;

		int m_columns;

		/**
		 * a 2 dimension array to store the object pointer
		 */
		Drawable* m_array;

	};
}

#endif /* TABLELAYOUT_HPP_ */
