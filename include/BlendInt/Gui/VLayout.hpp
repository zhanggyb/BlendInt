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

#ifndef _BLENDINT_VERTICALLAYOUT_HPP_
#define _BLENDINT_VERTICALLAYOUT_HPP_

#include <BlendInt/Gui/AbstractLayout.hpp>

namespace BlendInt {

	class VLayoutExt: public AbstractLayoutExt
	{
	public:

		VLayoutExt (Context* context, int alignment = AlignCenter, int space = 2);

		~VLayoutExt ();

		virtual void Fill ();

	protected:

		virtual bool AddLayoutItem (AbstractWidget* widget);

		virtual bool RemoveLayoutItem (AbstractWidget* widget);

	private:

		void Distribute ();

		void Align ();

		WidgetDeque m_widgets;

		int m_alignment;

		int m_space;
	};

}

#endif	// _BLENDINT_GUI_VBOX_HPP_
