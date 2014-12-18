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

#ifndef _BLENDINT_GUI_DIALOG_HPP_
#define _BLENDINT_GUI_DIALOG_HPP_

#include <BlendInt/OpenGL/GLBuffer.hpp>
#include <BlendInt/OpenGL/GLTexture2D.hpp>

#include <BlendInt/Gui/AbstractFloatingFrame.hpp>
#include <BlendInt/Gui/AbstractLayout.hpp>
#include <BlendInt/Gui/FrameShadow.hpp>

namespace BlendInt {

	class Dialog: public AbstractFloatingFrame
	{

	public:

		Dialog ();

		virtual ~Dialog();

		void SetLayout (AbstractLayout* layout);

		void AddWidget (AbstractWidget* widget);

		void InsertWidget (int index, AbstractWidget* widget);

	protected:

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual bool PreDraw (Profile& profile);

		virtual ResponseType Draw (Profile& profile);

		virtual void PostDraw (Profile& profile);

		virtual void FocusEvent (bool focus);

		virtual void MouseHoverInEvent (const MouseEvent& event);

		virtual void MouseHoverOutEvent (const MouseEvent& event);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		virtual ResponseType DispatchHoverEvent (const MouseEvent& event);

	private:

		void SetFocusedWidget (AbstractWidget* widget);

		void OnFocusedWidgetDestroyed (AbstractWidget* widget);

		void OnHoverWidgetDestroyed (AbstractWidget* widget);

		void OnLayoutDestroyed (AbstractWidget* layout);

		void RenderToBuffer (Profile& profile);

		glm::mat4 projection_matrix_;

		glm::mat4 model_matrix_;

		Point last_;
		Point cursor_;

		RefPtr<FrameShadow> shadow_;

		AbstractWidget* focused_widget_;

		AbstractWidget* hovered_widget_;

		AbstractLayout* layout_;

		GLuint vao_[2];

		GLBuffer<ARRAY_BUFFER, 2> buffer_;

        GLTexture2D texture_buffer_;

	};

}

#endif /* _BLENDINT_GUI_DIALOG_HPP_ */
