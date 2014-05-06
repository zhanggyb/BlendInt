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

#ifndef _BLENDINT_GUI_LAYOUTBASE_HPP_
#define _BLENDINT_GUI_LAYOUTBASE_HPP_

#include <BlendInt/Gui/AbstractWidget.hpp>
#include <BlendInt/Gui/AbstractContainer.hpp>

namespace BlendInt {

	class LayoutBase
	{
	public:

		explicit LayoutBase (AbstractContainer* container);

		~LayoutBase ();

		AbstractContainer* container () const
		{
			return m_container;
		}

	protected:

		void SetPosition (AbstractWidget* widget, int x, int y);

		void SetPosition (AbstractWidget* widget, const Point& pos);

		void Resize (AbstractWidget* widget, unsigned int width, unsigned int height);

		void Resize (AbstractWidget* widget, const Size& size);

	private:

		AbstractContainer* m_container;
	};

}

#endif /* _BLENDINT_GUI_LAYOUTBASE_HPP_ */
