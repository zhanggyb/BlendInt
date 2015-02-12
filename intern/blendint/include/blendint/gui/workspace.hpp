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

#include <gui/abstract-button.hpp>

#include <gui/frame.hpp>
#include <gui/toolbox.hpp>
#include <gui/frame-splitter.hpp>

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

		virtual Response Draw (AbstractWindow* context);

	private:

		GLuint vao_[2];

		/**
		 * 0 - inner
		 * 1 - outer
		 */
		GLBuffer<ARRAY_BUFFER, 2> buffer_;

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

		void SetHeader (ToolBox* header, bool append = true);

		void SwitchHeaderPosition ();

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

		virtual AbstractView* GetFocusedView () const;

	protected:

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual bool PreDraw (AbstractWindow* context);

		virtual Response Draw (AbstractWindow* context);

		virtual void PostDraw (AbstractWindow* context);

		virtual void PerformFocusOn (AbstractWindow* context);

		virtual void PerformFocusOff (AbstractWindow* context);

		virtual void PerformHoverIn (AbstractWindow* context);

		virtual void PerformHoverOut (AbstractWindow* context);

		virtual Response PerformKeyPress (AbstractWindow* context);

		virtual Response PerformMousePress (AbstractWindow* context);

		virtual Response PerformMouseRelease (AbstractWindow* context);

		virtual Response PerformMouseMove (AbstractWindow* context);

		virtual Response DispatchHoverEvent (AbstractWindow* context);

		virtual bool RemoveSubView (AbstractView* view);

	private:

		void InitializeWorkspace ();

		void SetFocusedFrame (AbstractFrame* frame, AbstractWindow* context);

		void SetHoveredFrame (AbstractWindow* context);

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
