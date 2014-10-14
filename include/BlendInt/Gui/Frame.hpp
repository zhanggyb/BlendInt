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

#ifndef _BLENDINT_GUI_SCREEN_HPP_
#define _BLENDINT_GUI_SCREEN_HPP_

#include <BlendInt/Gui/AbstractFrame.hpp>

namespace BlendInt {

	class Frame: public AbstractFrame
	{
	public:

		Frame ();

		virtual ~Frame ();

		void SetFocused (Widget* widget);

		virtual void SetCursorFollowedWidget (Widget* widget);

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

		virtual Widget* GetFocusedWidget () const;

		virtual Widget* GetHoveredWidget () const;

	protected:

		virtual void DispatchHoverEvent (const MouseEvent& event);

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

		virtual void PreDraw (Profile& profile);

		virtual ResponseType Draw (Profile& profile);

		virtual void PostDraw (Profile& profile);

		virtual void FocusEvent (bool focus);

		virtual void MouseHoverInEvent (const MouseEvent& event);

		virtual void MouseHoverOutEvent (const MouseEvent& event);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	private:

		//void OnCursorFollowedWidgetDestroyed (Widget* widget);

		//Widget* cursor_followed_widget_;

		//bool custom_focused_widget_;

	};

}

 #endif	// _BLENDINT_GUI_SCREEN_HPP_
