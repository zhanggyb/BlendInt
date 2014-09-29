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

#include <BlendInt/Gui/AbstractFrame.hpp>
#include <BlendInt/OpenGL/GLBuffer.hpp>

namespace BlendInt {

	class Widget;
	class FrameSplitter;

	class FrameSplitterHandle: public AbstractFrame
	{
	public:

		FrameSplitterHandle (Orientation orientation = Horizontal);

		virtual ~FrameSplitterHandle ();

		void SetHandleWidget (Widget* widget);

		virtual Size GetPreferredSize () const;

	protected:

		friend class FrameSplitter;

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

		AbstractFrame* previous_viewport_;

		AbstractFrame* next_viewport_;
	};

	// -------------------------------

	class FrameSplitter: public AbstractFrame
	{
	public:

		FrameSplitter (Orientation orientation = Horizontal);

		virtual ~FrameSplitter ();

		void PrependViewport (AbstractFrame* viewport);

		void AppendViewport (AbstractFrame* viewport);

		void Insert (int index, AbstractFrame* viewport);

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

		friend class FrameSplitterHandle;

		void AlignSubViewports (Orientation orientation, const Size& size);

		int GetAverageRoom (Orientation orientation, const Size& size);

		void OnHoverViewportDestroyed (AbstractWidget* widget);

		Orientation orientation_;

		AbstractFrame* hover_;
	};
}

#endif /* _BLENDINT_GUI_SCREENSPLITTER_HPP_ */
