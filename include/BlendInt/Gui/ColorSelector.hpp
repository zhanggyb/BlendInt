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

#include <BlendInt/Gui/HBlockLayout.hpp>
#include <BlendInt/Gui/VBlockLayout.hpp>
#include <BlendInt/Gui/Stack.hpp>

#include <BlendInt/Gui/ButtonGroup.hpp>

#include <BlendInt/Gui/VLayout.hpp>

namespace BlendInt {

	class ColorSelector: public VBox
	{
		DISALLOW_COPY_AND_ASSIGN(ColorSelector);

	public:

		ColorSelector ();

		virtual ~ColorSelector();

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request);

		virtual void PerformRoundRadiusUpdate (const RoundRadiusUpdateRequest& request);

		virtual ResponseType Draw (const Profile& profile);

	private:

		void InitializeColorSelector ();

		void OnButtonToggled (int index, bool toggled);

		VBlock* CreateRGBBlock ();

		VBlock* CreateHSVBlock ();

		VBox* CreateHexBlock ();

		Stack* CreateBlockStack ();

		ButtonGroup radio_group_;

		GLuint vaos_[2];

		RefPtr<GLArrayBuffer> inner_;
		RefPtr<GLArrayBuffer> outer_;

		Stack* stack_;

	};
}



#endif /* _BLENDINT_GUI_COLORSELECTOR_HPP_ */
