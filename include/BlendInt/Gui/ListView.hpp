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

#ifndef _BLENDINT_GUI_LISTVIEW_HPP_
#define _BLENDINT_GUI_LISTVIEW_HPP_

#include <BlendInt/Gui/ListModel.hpp>
#include <BlendInt/Gui/AbstractScrollable.hpp>
#include <BlendInt/Gui/AbstractItemView.hpp>

namespace BlendInt {

	/**
	 * @brief ListView provides a list or icon view onto a model
	 *
	 * The ListView provides a list or icon view onto a model.
	 *
	 * A ListView presents items stored in a model, either as a simple
	 * non-hierarchical list, or as a collection of icons.
	 *
	 * ListView is a subclass of AbstractScrollable, so it contains 2
	 * native ScrollBar object and will be displayed automatically
	 * when the widget cannot show all contents of the list.
	 */
	class ListView: public AbstractItemView
	{
		DISALLOW_COPY_AND_ASSIGN(ListView);

	public:

		ListView ();

		virtual ~ListView();

	protected:

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType FocusEvent (bool focus);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	private:

	};

}

#endif /* _BLENDINT_GUI_LISTVIEW_HPP_ */
