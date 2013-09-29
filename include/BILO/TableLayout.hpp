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

		void add_widget (Widget* widget, int row, int column, int width = 1, int height = 1);

		void add_layout (AbstractLayout* layout, int row, int column, int width = 1, int height = 1);

		int rows () const {return m_rows;}

		int columns () const {return m_columns;}

	protected:

		virtual bool update (int type, const void* property);

		virtual void render ();

	private:

		void generate_layout (const Size* size);

		void generate_default_layout ();

		int m_rows;

		int m_columns;

	};
}

#endif /* TABLELAYOUT_HPP_ */
