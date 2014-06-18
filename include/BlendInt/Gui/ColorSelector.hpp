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

#include <BlendInt/Gui/HBlock.hpp>
#include <BlendInt/Gui/VBlock.hpp>
#include <BlendInt/Gui/Stack.hpp>

#include <BlendInt/Gui/ButtonGroup.hpp>

#include <BlendInt/Gui/VBox.hpp>

namespace BlendInt {

	class ColorSelector: public VBox
	{
		DISALLOW_COPY_AND_ASSIGN(ColorSelector);

	public:

		ColorSelector ();

		virtual ~ColorSelector();

	protected:

		virtual void UpdateGeometry (const WidgetUpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

	private:

		void InitializeColorSelector ();

		void OnButtonToggled (int index, bool toggled);

		VBlock* CreateRGBBlock ();

		VBlock* CreateHSVBlock ();

		VBox* CreateHexBlock ();

		Stack* CreateBlockStack ();

		ButtonGroup m_radio_group;

		GLuint m_vao;

		RefPtr<GLArrayBuffer> m_inner;
		RefPtr<GLArrayBuffer> m_outer;

		Stack* m_stack;

	};
}



#endif /* _BLENDINT_GUI_COLORSELECTOR_HPP_ */
