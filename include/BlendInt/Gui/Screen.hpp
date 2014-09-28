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

#include <BlendInt/Gui/AbstractScreen.hpp>

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Gui/Layout.hpp>

namespace BlendInt {

	class Screen: public AbstractScreen
	{
	public:

		Screen ();

		virtual ~Screen ();

		void Setup (Widget* widget);

		void Setup (Layout* container);

		void SetFocused (AbstractWidget* widget);

		AbstractWidget* focused_widget() const
		{
			return focused_widget_;
		}

		AbstractWidget* top_hovered () const
		{
			return top_hovered_widget_;
		}

		virtual Size GetPreferredSize () const;

	protected:

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PreDraw (Profile& profile);

		virtual ResponseType Draw (Profile& profile);

		virtual void PostDraw (Profile& profile);

		virtual ResponseType FocusEvent (bool focus);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	private:

		bool CheckAndUpdateHoverWidget (const MouseEvent& event);

		void UpdateHoverWidgetSubs (Point& cursor);

		void OnFocusedWidgetDestroyed (AbstractWidget* widget);

		void OnHoverWidgetDestroyed (AbstractWidget* widget);

		void ClearHoverWidgets ();

		AbstractWidget* focused_widget_;

		AbstractWidget* top_hovered_widget_;

		bool custom_focused_widget_;

		Point cursor_;

		glm::mat4 projection_matrix_;

		glm::mat4 model_matrix_;
	};

}

 #endif	// _BLENDINT_GUI_SCREEN_HPP_
