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

#ifndef _BLENDINT_GUI_VIRTUALWINDOW_HPP_
#define _BLENDINT_GUI_VIRTUALWINDOW_HPP_

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>
#include <BlendInt/Gui/AbstractVectorContainer.hpp>

#include <BlendInt/OpenGL/ScissorStatus.hpp>

namespace BlendInt {

	/**
	 * @brief A special container works as a window in desktop
	 *
	 * The margin of a VirtualWindow is always zero.
	 */
	class VirtualWindow: public AbstractVectorContainer
	{
	public:

		VirtualWindow ();

		virtual ~VirtualWindow ();

		void Setup (AbstractWidget* widget);

	protected:

		virtual void UpdateContainer (const ContainerUpdateRequest& request);

		virtual void ProcessPositionUpdate (const PositionUpdateRequest& request);

		virtual void ProcessSizeUpdate (const SizeUpdateRequest& request);

		virtual void ProcessRoundTypeUpdate (const RoundTypeUpdateRequest& request);

		virtual void ProcessRoundRadiusUpdate (const RoundRadiusUpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType FocusEvent (bool focus);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		void FillSubWidgets (const Point& out_pos, const Size& size);

		void FillSubWidgets (int x, int y, int w, int h);

	private:

		enum SubWidgetIndex {
			DecorationIndex,
			ContentIndex
		};

		void InitializeVirtualWindow ();

		GLuint m_vao[1];

		RefPtr<GLArrayBuffer> m_inner;

	};

}

#endif /* _BLENDINT_GUI_VIRTUALWINDOW_HPP_ */
