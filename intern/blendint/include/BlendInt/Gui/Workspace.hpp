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
 * License along with BlendInt.	 If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_GUI_WORKSPACE_HPP_
#define _BLENDINT_GUI_WORKSPACE_HPP_

#include <BlendInt/Gui/VLayout.hpp>

#include <BlendInt/Gui/Splitter.hpp>
#include <BlendInt/Gui/Button.hpp>

#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/FrameSplitter.hpp>
#include <BlendInt/Gui/ToolBox.hpp>

namespace BlendInt {

	/**
	 * @brief A special button used in Workspace to switch on/off side bars
	 */
	class EdgeButton: public AbstractButton
	{
		DISALLOW_COPY_AND_ASSIGN(EdgeButton);

	public:

		EdgeButton (int round_type);

		virtual ~EdgeButton ();

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (Profile& profile);

	private:

		GLuint vao_[2];

		/**
		 * 0 - inner
		 * 1 - outer
		 */
		GLBuffer<ARRAY_BUFFER, 2> buffer_;

	};

	/**
	 * @brief A special container used in Workspace
	 */
	class EdgeButtonLayer: public Widget
	{
		DISALLOW_COPY_AND_ASSIGN(EdgeButtonLayer);

	public:

		EdgeButtonLayer();

		virtual ~EdgeButtonLayer ();

		virtual bool Contain (const Point& point) const;

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

	private:

		void InitializeSideButtonLayer ();

		void AlighButtons (const Point& out_pos, const Size& out_size);

		void AlignButtons (int x, int y, int w, int h);

	};

	// ------------------------

	/**
	 * @brief A special container which works as a space in Blender
	 *
	 * A workspace may contain:
	 * 	- A left side bar, e.g, for toolbox
	 * 	- A right side bar, e.g, a property box
	 * 	- A header which on top or bottom
	 * 	- A viewport
	 */
	class Workspace: public Frame
	{
		DISALLOW_COPY_AND_ASSIGN(Workspace);

	public:

		Workspace ();

		virtual ~Workspace ();

		void SetViewport (Frame* viewport);

		void SetLeftSideBar (ToolBox* leftbar);

		void SetRightSideBar (ToolBox* sidebar);

		void SetHeader (ToolBox* header);

		void SwitchHeaderPosition ();

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

		virtual AbstractView* GetFocusedView () const;

	protected:

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

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

		void InitializeWorkspace ();

		void SetFocusedFrame (AbstractFrame* frame);

		void OnHoverFrameDestroyed (AbstractFrame* frame);

		void OnFocusedFrameDestroyed (AbstractFrame* frame);

		ToolBox* left_sidebar_;

		ToolBox* right_sidebar_;

		ToolBox* header_;

		Frame* viewport_;

		FrameSplitter* splitter_;

		AbstractFrame* hover_frame_;

		AbstractFrame* focused_frame_;

	};

}


#endif /* _BLENDINT_GUI_WORKSPACE_HPP_ */
