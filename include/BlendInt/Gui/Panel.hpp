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
#include <BlendInt/Gui/AbstractContainer.hpp>
#include <BlendInt/OpenGL/TextureBuffer.hpp>

namespace BlendInt {

	/**
	 * @brief A special container works as a window in desktop
	 *
	 * The margin of a VirtualWindow is always zero.
	 */
	class Panel: public AbstractContainer
	{
	public:

		Panel ();

		virtual ~Panel ();

		void Setup (AbstractWidget* widget);

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

		void FillSubWidgets (const Point& out_pos, const Size& size, const Margin& margin);

		void FillSubWidgets (int x, int y, int w, int h);

	private:

		enum SubWidgetIndex {
			DecorationIndex,
			ContentIndex
		};

		void InitializeVirtualWindow ();

		int space_;

		GLuint vao_[2];

		RefPtr<GLArrayBuffer> inner_;

		RefPtr<GLArrayBuffer> outer_;
	};

	class StaticPanel: public Panel
	{
	public:

		StaticPanel ();

		virtual ~StaticPanel ();

	protected:

		virtual void PerformRefresh (const RefreshRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (Profile& profile);

	private:

		void RenderToBuffer (Profile& profile);

		TextureBuffer tex_buffer_;

		bool refresh_;

	};

}

#endif /* _BLENDINT_GUI_VIRTUALWINDOW_HPP_ */
