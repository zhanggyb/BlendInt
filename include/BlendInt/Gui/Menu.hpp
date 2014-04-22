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

#ifndef _BLENDINT_MENUITEMBIN_HPP_
#define _BLENDINT_MENUITEMBIN_HPP_

#include <boost/smart_ptr.hpp>

#include <BlendInt/Core/String.hpp>
#include <BlendInt/OpenGL/GLArrayBuffer.hpp>

#include <BlendInt/Gui/Font.hpp>
#include <BlendInt/Gui/Shadow.hpp>
#include <BlendInt/Gui/RoundWidget.hpp>
#include <BlendInt/Gui/ActionItem.hpp>

#include <Cpp/Events.hpp>

namespace BlendInt {

	/**
	 * @brief A widget contains and handles a menu
	 */
	class Menu: public RoundWidget
	{
		DISALLOW_COPY_AND_ASSIGN(Menu);

	public:

		Menu ();

		virtual ~Menu();

		void SetTitle (const String& title);

		void AddActionItem (const String& text);

		void AddActionItem (const String& text, const String& shortcut);

		void AddActionItem (const RefPtr<Icon>& icon, const String& text);

		void AddActionItem (const RefPtr<Icon>& icon, const String& text, const String& shortcut);

		void AddActionItem (const RefPtr<ActionItem>& item);

		void RemoveActionItem (size_t index);

		void RemoveActionItem (const RefPtr<ActionItem>& item);

		const String& title () const
		{
			return m_title;
		}

		Cpp::EventRef<ActionItem*> triggered() {return m_triggered;}

	protected:

		virtual bool Update (const UpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType FocusEvent (bool focus);

		virtual ResponseType MouseMoveEvent(const MouseEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

	private:

		void InitOnce ();

		void ResetHighlightBuffer (unsigned int width);

		unsigned int GetHighlightNo (int y);

		GLuint m_vao;

		String m_title;

		/**
		 * @brief The highlight item in Menu
		 *
		 * 	- 0: no highlight
		 * 	- n: the n'th item in the Menu
		 */
		unsigned int m_highlight;	// the highlight item index

		Font m_font;

		RefPtr<GLArrayBuffer> m_inner_buffer;
		RefPtr<GLArrayBuffer> m_outer_buffer;
		RefPtr<GLArrayBuffer> m_highlight_buffer;

		RefPtr<Shadow> m_shadow;

		std::deque<RefPtr<ActionItem> > m_list;

		Cpp::Event<ActionItem*> m_hovered;

		Cpp::Event<ActionItem*> m_triggered;

		//Cpp::Event<ActionItem*> m_triggered;
		static int DefaultMenuItemHeight;

		static int DefaultIconSpace;

		static int DefaultShortcutSpace;

	};

}

#endif /* _BLENDINT_MENUBIN_HPP_ */

