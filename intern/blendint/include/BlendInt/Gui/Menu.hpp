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

#ifndef _BLENDINT_GUI_MENU_HPP_
#define _BLENDINT_GUI_MENU_HPP_

#include <BlendInt/Core/String.hpp>
#include <BlendInt/OpenGL/GLBuffer.hpp>

#include <BlendInt/Gui/Font.hpp>
#include <BlendInt/Gui/Action.hpp>
#include <BlendInt/Gui/AbstractFloatingFrame.hpp>
#include <BlendInt/Gui/FrameShadow.hpp>

#include <Cpp/Events.hpp>

namespace BlendInt {

	/**
	 * @brief A widget contains and handles a menu
	 */
	class Menu: public AbstractFloatingFrame
	{
		DISALLOW_COPY_AND_ASSIGN(Menu);

	public:

		Menu ();

		virtual ~Menu();

		void SetTitle (const String& title);

		void AddAction (const String& text);

		void AddAction (const String& text, const String& shortcut);

		void AddAction (const RefPtr<AbstractIcon>& icon, const String& text);

		void AddAction (const RefPtr<AbstractIcon>& icon, const String& text, const String& shortcut);

		void AddAction (const RefPtr<Action>& item);

		void RemoveAction (size_t index);

		void RemoveAction (const RefPtr<Action>& item);

		const String& title () const
		{
			return m_title;
		}

		Cpp::EventRef<Action*> triggered() {return m_triggered;}

	protected:

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (int round_type);

		virtual void PerformRoundRadiusUpdate (float radius);

		virtual bool PreDraw (Profile& profile);

		virtual ResponseType Draw (Profile& profile);

		virtual void PostDraw (Profile& profile);

		virtual void FocusEvent (bool focus);

		virtual void MouseHoverInEvent (const MouseEvent& event);

		virtual void MouseHoverOutEvent (const MouseEvent& event);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		virtual ResponseType DispatchHoverEvent (const MouseEvent& event);

	private:

		void InitializeMenu ();

		void ResetHighlightBuffer (int width);

		unsigned int GetHighlightNo (int y);

		GLuint vao_[3];

		/**
		 * @brief The highlight item in Menu
		 *
		 * 	- 0: no highlight
		 * 	- n: the n'th item in the Menu
		 */
		unsigned int m_highlight;	// the highlight item index

		FrameShadow* shadow_;

		String m_title;

		Font m_font;

		RefPtr<GLArrayBuffer> inner_;
		RefPtr<GLArrayBuffer> outer_;

		RefPtr<GLArrayBuffer> m_highlight_buffer;

		std::deque<RefPtr<Action> > m_list;

		Cpp::Event<Action*> m_hovered;

		Cpp::Event<Action*> m_triggered;

		glm::mat4 projection_matrix_;

		glm::mat4 model_matrix_;

		//Cpp::Event<ActionItem*> m_triggered;
		static int DefaultMenuItemHeight;

		static int DefaultIconSpace;

		static int DefaultShortcutSpace;

	};

}

#endif /* _BLENDINT_MENUBIN_HPP_ */

