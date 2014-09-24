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

#ifndef _BLENDINT_GUI_SCREENSPLITTER_HPP_
#define _BLENDINT_GUI_SCREENSPLITTER_HPP_

#include <BlendInt/Gui/AbstractScreen.hpp>
#include <BlendInt/OpenGL/GLBuffer.hpp>

namespace BlendInt {

	class Widget;
	class ScreenSplitter;

	class ScreenSplitterHandle: public AbstractScreen
	{
	public:

		ScreenSplitterHandle (Orientation orientation = Horizontal);

		virtual ~ScreenSplitterHandle ();

		void SetHandleWidget (Widget* widget);

		virtual Size GetPreferredSize () const;

	protected:

		friend class ScreenSplitter;

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

		glm::mat4 projection_matrix_;

		glm::mat4 model_matrix_;

		Orientation orientation_;

		GLuint vao_;

		GLBuffer<ARRAY_BUFFER> buffer_;

		AbstractScreen* previous_viewport_;

		AbstractScreen* next_viewport_;
	};

	// -------------------------------

	class ScreenSplitter: public AbstractScreen
	{
	public:

		ScreenSplitter (Orientation orientation = Horizontal);

		virtual ~ScreenSplitter ();

		void PrependViewport (AbstractScreen* viewport);

		void AppendViewport (AbstractScreen* viewport);

		void Insert (int index, AbstractScreen* viewport);

		virtual Size GetPreferredSize () const;

	protected:

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

		friend class ScreenSplitterHandle;

		void AlignSubViewports (Orientation orientation, const Size& size);

		int GetAverageRoom (Orientation orientation, const Size& size);

		void OnHoverViewportDestroyed (AbstractWidget* widget);

		Orientation orientation_;

		AbstractScreen* hover_;
	};
}

#endif /* _BLENDINT_GUI_SCREENSPLITTER_HPP_ */
