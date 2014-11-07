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

#ifndef _BLENDINT_GUI_TOOLBOX_HPP_
#define _BLENDINT_GUI_TOOLBOX_HPP_

#include <BlendInt/Gui/Layout.hpp>
#include <BlendInt/OpenGL/GLBuffer.hpp>
#include <BlendInt/Gui/Frame.hpp>

#include <BlendInt/OpenGL/GLTexture2D.hpp>

namespace BlendInt {

	class ToolBox: public Frame
	{
	public:

		ToolBox (Orientation orientation = Horizontal);

		ToolBox (int width, int height, Orientation orientation = Horizontal);

		virtual ~ToolBox ();

		void AddWidget (Widget* widget, bool append = true);

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

		Widget* focused_widget () const
		{
			return focused_widget_;
		}

		Widget* hovered_widget () const
		{
			return hovered_widget_;
		}

	protected:

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual bool PreDraw (Profile& profile);

		virtual ResponseType Draw (Profile& profile);

		virtual void PostDraw (Profile& profile);

		virtual void FocusEvent (bool focus);

		virtual void MouseHoverInEvent (const MouseEvent& event);

		virtual void MouseHoverOutEvent (const MouseEvent& event);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		virtual ResponseType DispatchHoverEvent (const MouseEvent& event);

	private:

		void InitializeToolBoxOnce ();

		void FillSubWidgets ();

		void FillSubWidgetsHorizontally ();

		void FillSubWidgetsVertically ();

		int GetLastPosition () const;

		void SetFocusedWidget (Widget* widget);

		void OnFocusedWidgetDestroyed (Widget* widget);

		void OnHoverWidgetDestroyed (Widget* widget);

		void RenderToBuffer (Profile& profile);

		glm::mat4 projection_matrix_;

		glm::mat4 model_matrix_;

		Widget* focused_widget_;

		Widget* hovered_widget_;

		int space_;

		// 0 - for inner
		// 1 - for texture buffer
		GLuint vao_[2];
		GLBuffer<ARRAY_BUFFER, 2> inner_;

		Margin margin_;

		// Layout orientation
		Orientation orientation_;

        GLTexture2D texture_buffer_;

	};

}

#endif /* _BLENDINT_GUI_TOOLBOX_HPP_ */
