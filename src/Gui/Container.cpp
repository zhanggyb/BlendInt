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

#include <BlendInt/Gui/Container.hpp>

namespace BlendInt {

	Container::Container ()
	{
	}

	Container::~Container ()
	{
	}

	void Container::SetMargin (const Margin& margin)
	{
		if (margin_.equal(margin))
			return;

		PerformMarginUpdate(margin);
		margin_ = margin;
	}

	void Container::SetMargin (int left, int right, int top, int bottom)
	{
		if (margin_.equal(left, right, top, bottom))
			return;

		Margin new_margin(left, right, top, bottom);
		PerformMarginUpdate(new_margin);
		margin_ = new_margin;
	}

	void Container::PerformMarginUpdate(const Margin& margin)
	{
	}

	ResponseType Container::Draw (Profile& profile)
	{
		return Ignore;
	}

	ResponseType Container::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Container::KeyPressEvent (
	        const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Container::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Container::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Container::MousePressEvent (
	        const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Container::MouseReleaseEvent (
	        const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Container::MouseMoveEvent (
	        const MouseEvent& event)
	{
		return Ignore;
	}

}
