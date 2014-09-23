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

#ifndef _BLENDINT_GUI_TOOLBAR_HPP_
#define _BLENDINT_GUI_TOOLBAR_HPP_

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>
#include <BlendInt/OpenGL/GLSLProgram.hpp>

#include <BlendInt/Gui/Container.hpp>
#include <BlendInt/Gui/ToolButton.hpp>

namespace BlendInt {

	class ToolBar: public Container
	{
		DISALLOW_COPY_AND_ASSIGN(ToolBar);

	public:

		ToolBar ();

		virtual ~ToolBar ();

		void Append (AbstractWidget* widget);

		void Append (const RefPtr<Action>& action);

		virtual bool IsExpandX () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual void PerformMarginUpdate (const Margin& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (Profile& profile);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	private:

		void InitializeToolBar ();

		void FillSubWidgets (const Size& out_size, const Margin& margin, int space);

		void FillSubWidgets (int x, int y, int width, int height, int space);

		void RealignSubWidgets (const Size& size, const Margin& margin, int space);

		int GetLastPosition () const;

		GLuint vao_;

		/**
		 * space between tool buttons
		 */
		int space_;

		/** Used for middle mouse move */
		bool move_status_;
		int last_x_;
		int start_x_;

		RefPtr<GLArrayBuffer> inner_;
	};

}

#endif /* _BLENDINT_GUI_TOOLBAR_HPP_ */
