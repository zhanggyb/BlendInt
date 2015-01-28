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

#pragma once

#include <BlendInt/OpenGL/GLBuffer.hpp>
#include <BlendInt/OpenGL/GLTexture2D.hpp>

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Gui/Frame.hpp>

namespace BlendInt {

	class ToolBox: public Frame
	{
	public:

		ToolBox (Orientation orientation = Horizontal);

		ToolBox (int width, int height, Orientation orientation = Horizontal);

		virtual ~ToolBox ();

		void AddWidget (AbstractWidget* widget);

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

		virtual AbstractView* GetFocusedView () const;

	protected:

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual bool PreDraw (const AbstractWindow* context);

		virtual ResponseType Draw (const AbstractWindow* context);

		virtual void PostDraw (const AbstractWindow* context);

		virtual void PerformFocusOn (const AbstractWindow* context);

		virtual void PerformFocusOff (const AbstractWindow* context);

		virtual void PerformHoverIn (const AbstractWindow* context);

		virtual void PerformHoverOut (const AbstractWindow* context);

		virtual ResponseType PerformKeyPress (const AbstractWindow* context);

		virtual ResponseType PerformMousePress (const AbstractWindow* context);

		virtual ResponseType PerformMouseRelease (const AbstractWindow* context);

		virtual ResponseType PerformMouseMove (const AbstractWindow* context);

	private:

		virtual ResponseType DispatchHoverEvent (const AbstractWindow* context);

		void InitializeToolBoxOnce ();

		void FillSubWidgets ();

		void FillSubWidgetsHorizontally ();

		void FillSubWidgetsVertically ();

		int GetLastPosition () const;

		void SetFocusedWidget (AbstractWidget* widget, const AbstractWindow* context);

		void OnFocusedWidgetDestroyed (AbstractWidget* widget);

		void OnHoverWidgetDestroyed (AbstractWidget* widget);

		glm::mat4 projection_matrix_;

		glm::mat3 model_matrix_;

		AbstractWidget* focused_widget_;

		AbstractWidget* hovered_widget_;

		int space_;

		// 0 - for inner
		// 1 - for outer
		// 2 - for texture buffer
		GLuint vao_[3];
		GLBuffer<ARRAY_BUFFER, 2> buffer_;

		// for texture buffer
		GLBuffer<ARRAY_BUFFER, 1> vbo_;

		Margin margin_;

		// Layout orientation
		Orientation orientation_;

        GLTexture2D texture_buffer_;

        int cursor_position_;
	};

}
