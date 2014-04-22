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

#ifndef _BLENDINT_GUI_TAB_HPP_
#define _BLENDINT_GUI_TAB_HPP_

#include <BlendInt/Core/String.hpp>

#include <BlendInt/Gui/AbstractDequeContainer.hpp>
#include <BlendInt/Gui/AbstractButton.hpp>
#include <BlendInt/Gui/Stack.hpp>
#include <BlendInt/Gui/HLayout.hpp>
#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/ButtonGroup.hpp>

namespace BlendInt {

	/**
	 * @brief A special stack used in Tab and share theme with TabButton
	 */
	class TabStack: public Stack
	{
		DISALLOW_COPY_AND_ASSIGN(TabStack);

	public:

		TabStack ();

		virtual ~TabStack ();

	protected:

		virtual void Update (const UpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

	private:

		void InitOnce ();

		GLuint m_vao;

		RefPtr<GLArrayBuffer> m_inner;
		RefPtr<GLArrayBuffer> m_outer;
	};

	class Tab: public AbstractDequeContainer
	{
		DISALLOW_COPY_AND_ASSIGN(Tab);

	public:

		Tab ();

		virtual ~Tab ();

		void Add (const String& title, AbstractWidget* widget);

	protected:

		virtual void Update (const UpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	private:

		void OnButtonToggled (int index, bool toggled);

		void AdjustGeometries ();

		unsigned int m_title_height;

		ButtonGroup m_group;

		TabStack* m_stack;
	};

}

#endif /* _BLENDINT_GUI_TAB_HPP_ */
