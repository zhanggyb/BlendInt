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

#ifndef _BLENDINT_GUI_MULTIPLEFRAME_HPP_
#define _BLENDINT_GUI_MULTIPLEFRAME_HPP_

#include <BlendInt/Gui/Frame.hpp>

namespace BlendInt {

	class MultipleFrame: public Frame
	{

	public:

		MultipleFrame();

		virtual ~MultipleFrame ();

		virtual Widget* GetFocusedWidget () const;

		virtual Widget* GetHoveredWidget () const;

	protected:

		virtual void DispatchHoverEvent (const MouseEvent& event);

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PreDraw (Profile& profile);

		virtual ResponseType Draw (Profile& profile);

		virtual void PostDraw (Profile& profile);

		virtual void MouseHoverOutEvent (const MouseEvent& event);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	private:

		void DispatchMouseHoverEventInSubs (const MouseEvent& event);

		void OnFocusedWidgetDestroyed (Widget* widget);

		void OnHoverWidgetDestroyed (Widget* widget);

		void ClearHoverWidgets ();

		Widget* focused_widget_;

		Widget* hovered_widget_;

		glm::mat4 projection_matrix_;

		glm::mat4 model_matrix_;

	};

}


#endif /* _BLENDINT_GUI_MULTIPLEFRAME_HPP_ */
