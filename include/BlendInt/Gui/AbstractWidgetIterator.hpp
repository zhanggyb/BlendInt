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

#ifndef _BLENDINT_GUI_ABSTRACTWIDGETITERATOR_HPP_
#define _BLENDINT_GUI_ABSTRACTWIDGETITERATOR_HPP_

#include <BlendInt/Gui/AbstractWidget.hpp>

namespace BlendInt {

	/**
	 * @brief A virtual iterator to be instanced for widgets in container
	 */
	class AbstractWidgetIterator
	{
	public:

		AbstractWidgetIterator()
		{
		}

		virtual ~AbstractWidgetIterator ()
		{
		}

		virtual AbstractWidget& operator *() const = 0;

		virtual AbstractWidget* operator -> () const = 0;

		virtual void next () = 0;

	};

}


#endif /* _BLENDINT_GUI_ABSTRACTWIDGETITERATOR_HPP_ */
