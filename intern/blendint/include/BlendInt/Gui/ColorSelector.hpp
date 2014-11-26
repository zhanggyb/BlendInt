/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_GUI_COLORSELECTOR_HPP_
#define _BLENDINT_GUI_COLORSELECTOR_HPP_

#include <BlendInt/Gui/Block.hpp>
#include <BlendInt/Gui/StackLayout.hpp>
#include <BlendInt/Gui/VLayout.hpp>

#include <BlendInt/Gui/ButtonGroup.hpp>

#include <BlendInt/Gui/PopupFrame.hpp>
#include <BlendInt/Gui/Stack.hpp>

namespace BlendInt {

	class ColorSelector: public PopupFrame
	{
		DISALLOW_COPY_AND_ASSIGN(ColorSelector);

	public:

		ColorSelector ();

		virtual ~ColorSelector();

	private:

		void InitializeColorSelector ();

		void OnButtonToggled (int index, bool toggled);

		Block* CreateRGBBlock ();

		Block* CreateHSVBlock ();

		VLayout* CreateHexBlock ();

		Stack* CreateBlockStack ();

		ButtonGroup radio_group_;

		Stack* stack_;
	};
}



#endif /* _BLENDINT_GUI_COLORSELECTOR_HPP_ */
