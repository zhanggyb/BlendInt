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

#include <BlendInt/Gui/TreeView.hpp>

namespace BlendInt {

	TreeView::TreeView()
	: AbstractItemView()
	{

	}

	TreeView::~TreeView ()
	{
	}

	ModelIndex TreeView::GetIndexAt (const Point& point) const
	{
		return ModelIndex();
	}

	ResponseType TreeView::Draw (const RedrawEvent& event)
	{
		return Ignore;
	}

	ResponseType TreeView::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType TreeView::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType TreeView::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType TreeView::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType TreeView::ContextMenuReleaseEvent (
			const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType TreeView::MousePressEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType TreeView::MouseReleaseEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType TreeView::MouseMoveEvent(const MouseEvent& event)
	{
		return Ignore;
	}

}
