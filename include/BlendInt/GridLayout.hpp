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

#ifndef _BLENDINT_GRIDLAYOUT_HPP_
#define _BLENDINT_GRIDLAYOUT_HPP_

#include <BlendInt/AbstractLayout.hpp>

namespace BlendInt {

	/**
	 * Grid layout
	 */
	class GridLayout: public AbstractLayout
	{
		DISALLOW_COPY_AND_ASSIGN(GridLayout);

	public:

		GridLayout ();

		GridLayout (AbstractWidget* parent);

		virtual ~GridLayout ();

		virtual void add_widget (Frame* object);

		virtual void add_layout (AbstractLayout* layout);

		virtual bool remove (AbstractWidget* object);

		virtual bool erase (AbstractWidget* object);

	protected:

		virtual void Draw ();

		virtual void add_single_widget(Frame* widget);
	};

}

#endif /* GRIDLAYOUT_HPP_ */
