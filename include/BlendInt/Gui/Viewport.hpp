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

#ifndef _BLENDINT_GUI_VIEWPORT_HPP_
#define _BLENDINT_GUI_VIEWPORT_HPP_

#include <BlendInt/Gui/AbstractViewport.hpp>
#include <BlendInt/OpenGL/GLBuffer.hpp>

#include <BlendInt/Gui/Widget.hpp>

namespace BlendInt {

	class Viewport: public AbstractViewport
	{
	public:

		Viewport ();

		virtual ~Viewport ();

		void AddWidget (Widget* widget);

		void SetFocused (AbstractWidget* widget);

		AbstractWidget* focused() const
		{
			return focused_;
		}

		AbstractWidget* top_hovered () const
		{
			return top_hovered_;
		}

		virtual Size GetPreferredSize () const;

	protected:

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (Profile& profile);

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

		void UpdateHoverWidgetSubs (const MouseEvent& event);

		void OnFocusedWidgetDestroyed (AbstractWidget* widget);

		void OnHoverWidgetDestroyed (AbstractWidget* widget);

		void ClearHoverWidgets ();

		AbstractWidget* focused_;

		AbstractWidget* top_hovered_;

		bool custom_focused_widget_;

		Point cursor_;

		glm::mat4 projection_matrix_;

		glm::mat4 model_matrix_;

		GLuint vao_;

		GLBuffer<ARRAY_BUFFER> buffer_;
	};

}

 #endif	// _BLENDINT_GUI_VIEWPORT_HPP_
