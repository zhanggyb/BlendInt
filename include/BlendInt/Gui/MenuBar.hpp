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

#ifndef _BLENDINT_GUI_MENUBAR_HPP_
#define _BLENDINT_GUI_MENUBAR_HPP_

#include <BlendInt/Gui/AbstractContainer.hpp>

namespace BlendInt {

	class MenuBar: public AbstractContainer
	{
		DISALLOW_COPY_AND_ASSIGN(MenuBar);

	public:

		MenuBar ();

		virtual ~MenuBar ();

		int space () const
		{
			return m_space;
		}

	protected:

		virtual bool Update (const UpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual void CursorEnterEvent (bool entered);

		virtual void KeyPressEvent (KeyEvent* event);

		virtual void ContextMenuPressEvent (ContextMenuEvent* event);

		virtual void ContextMenuReleaseEvent (ContextMenuEvent* event);

		virtual void MousePressEvent (MouseEvent* event);

		virtual void MouseReleaseEvent (MouseEvent* event);

		virtual void MouseMoveEvent (MouseEvent* event);

	private:

		void InitOnce ();

		GLuint m_vao;

		int m_space;

		RefPtr<GLArrayBuffer> m_buffer;

	};

}

#endif /* _BLENDINT_GUI_MENUBAR_HPP_ */
