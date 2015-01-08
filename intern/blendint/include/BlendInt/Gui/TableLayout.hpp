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

#ifndef _BLENDINT_GUI_TABLELAYOUT_HPP_
#define _BLENDINT_GUI_TABLELAYOUT_HPP_

#include <BlendInt/Gui/AbstractLayout.hpp>

namespace BlendInt {

	class TableLayout: public AbstractLayout
	{
	public:

		TableLayout (unsigned int row, unsigned int column);

		virtual bool AddWidget (AbstractWidget* widget);

		virtual bool InsertWidget (int index, AbstractWidget* widget);

		virtual bool InsertWidget (int row, int column, AbstractWidget* widget);

	protected:

		virtual void PerformMarginUpdate (const Margin& margin);

	private:

		unsigned int row_;

		unsigned int column_;

	};

}

#endif /* _BLENDINT_GUI_TABLELAYOUT_HPP_ */
