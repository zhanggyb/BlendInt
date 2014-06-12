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

#ifndef _BLENDINT_GUI_TABHEADER_HPP_
#define _BLENDINT_GUI_TABHEADER_HPP_

#include <BlendInt/Gui/AbstractDequeContainer.hpp>
#include <BlendInt/Gui/ButtonGroup.hpp>
#include <BlendInt/Gui/TabButton.hpp>

namespace BlendInt {

	class TabHeader: public AbstractDequeContainer
	{
		DISALLOW_COPY_AND_ASSIGN(TabHeader);

	public:

		TabHeader ();

		virtual ~TabHeader ();

		void PushBack (TabButton* button);

		virtual bool IsExpandX () const;

		virtual Size GetPreferredSize () const;

		Cpp::EventRef<int> button_clicked ()
		{
			return m_button_index_clicked;
		}

		Cpp::EventRef<int, bool> button_index_toggled ()
		{
			return m_button_index_toggled;
		}

	protected:

		virtual void UpdateContainer (const WidgetUpdateRequest& request);

		virtual void UpdateGeometry (const WidgetUpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	private:

		void OnButtonIndexClicked (int index);

		void OnButtonIndexToggled (int index, bool toggled);

		int GetLastPosition () const;

		RefPtr<GLArrayBuffer> m_buffer;

		GLuint m_vao;

		ButtonGroup m_group;

		Cpp::Event<int> m_button_index_clicked;

		Cpp::Event<int, bool> m_button_index_toggled;
	};

}

#endif /* _BLENDINT_GUI_TABHEADER_HPP_ */
