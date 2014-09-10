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

#include <BlendInt/Gui/AbstractContainer.hpp>

#include <BlendInt/Gui/Splitter.hpp>
#include <BlendInt/Gui/Button.hpp>

namespace BlendInt {

	/**
	 * @brief A special button used in Workspace to switch on/off side bars
	 */
	class SideButton: public AbstractButton
	{
		DISALLOW_COPY_AND_ASSIGN(SideButton);

	public:

		SideButton (int round_type);

		virtual ~SideButton ();

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (Profile& profile);

	private:

		GLuint vao_[2];

		RefPtr<GLArrayBuffer> inner_;

		RefPtr<GLArrayBuffer> outer_;

	};

	/**
	 * @brief A special container which works as a space in Blender
	 *
	 * A workspace may contain:
	 * 	- A left side bar, e.g, for toolbox
	 * 	- A right side bar, e.g, a property box
	 * 	- A header which on top or bottom
	 * 	- A viewport
	 */
	class Workspace: public AbstractContainer
	{
		DISALLOW_COPY_AND_ASSIGN(Workspace);

	public:

		Workspace ();

		virtual ~Workspace ();

		void SetViewport (AbstractWidget* viewport);

		void SetLeftSideBar (AbstractWidget* widget);

		void SetRightSideBar (AbstractWidget* widget);

		void SetHeader (AbstractWidget* widget);

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request);

		virtual void PerformRoundRadiusUpdate (const RoundRadiusUpdateRequest& request);

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

		void InitializeWorkspace ();

		void AdjustGeometries (const Point& out_pos, const Size& out_size, const Margin& margin);

		void AdjustGeometries (int x, int y, int w, int h);

		AbstractWidget* left_sidebar_;

		AbstractWidget* right_sidebar_;

		AbstractWidget* header_;

		AbstractWidget* viewport_;

		Splitter* splitter_;

		SideButton* left_button_;

		SideButton* right_button_;

		GLuint vao_;

		RefPtr<GLArrayBuffer> inner_;

	};

}


#endif /* _BLENDINT_GUI_WORKSPACE_HPP_ */
