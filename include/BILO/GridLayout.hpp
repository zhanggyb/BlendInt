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

#ifndef _BILO_GRIDLAYOUT_HPP_
#define _BILO_GRIDLAYOUT_HPP_

#include <BILO/AbstractLayout.hpp>

namespace BILO {

	/**
	 * Grid layout
	 */
	class GridLayout: public AbstractLayout
	{
		DISALLOW_COPY_AND_ASSIGN(GridLayout);

	public:

		GridLayout ();

		GridLayout (Drawable* parent);

		virtual ~GridLayout ();

		virtual void add_widget (Widget* object);

		virtual void add_layout (AbstractLayout* layout);

		virtual bool remove (Drawable* object);

		virtual bool erase (Drawable* object);

	protected:

		virtual void render ();
	};

}

#endif /* GRIDLAYOUT_HPP_ */
