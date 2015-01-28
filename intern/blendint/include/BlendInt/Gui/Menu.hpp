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

#include <BlendInt/OpenGL/GLBuffer.hpp>

#include <BlendInt/Gui/AbstractFloatingFrame.hpp>
#include <BlendInt/Gui/FrameShadow.hpp>

#include <BlendInt/Gui/MenuItem.hpp>
#include <BlendInt/Gui/Button.hpp>

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

		bool AddMenuItem (MenuItem* item);

		bool InsertMenuItem (int index, MenuItem* item);

		bool AddButton (Button* btn);

		const String& title () const
		{
			return title_;
		}

		Cpp::EventRef<Action*> triggered() {return m_triggered;}

		const Font& font () const
		{
			return font_;
		}

	protected:

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (int round_type);

		virtual void PerformRoundRadiusUpdate (float radius);

		virtual bool PreDraw (const AbstractWindow* context);

		virtual ResponseType Draw (const AbstractWindow* context);

		virtual void PostDraw (const AbstractWindow* context);

		virtual void PerformFocusOn (const AbstractWindow* context);

		virtual void PerformFocusOff (const AbstractWindow* context);

		virtual void PerformHoverIn (const AbstractWindow* context);

		virtual void PerformHoverOut (const AbstractWindow* context);

		virtual ResponseType PerformKeyPress (const AbstractWindow* context);

		virtual ResponseType PerformContextMenuPress (const AbstractWindow* context);

		virtual ResponseType PerformContextMenuRelease (const AbstractWindow* context);

		virtual ResponseType PerformMousePress (const AbstractWindow* context);

		virtual ResponseType PerformMouseRelease (const AbstractWindow* context);

		virtual ResponseType PerformMouseMove (const AbstractWindow* context);

	private:

		virtual ResponseType DispatchHoverEvent (const AbstractWindow* context);

		void InitializeMenu ();

		void ResetHighlightBuffer (int width);

		void OnFocusedWidgetDestroyed (AbstractWidget* widget);

		void OnHoverWidgetDestroyed (AbstractWidget* widget);

		void SetFocusedWidget (AbstractWidget* widget, const AbstractWindow* context);

		String title_;

		Font font_;

		GLuint vao_[3];

		GLBuffer<ARRAY_BUFFER, 3> buffer_;

		Cpp::Event<Action*> m_hovered;

		Cpp::Event<Action*> m_triggered;

		glm::mat4 projection_matrix_;

		glm::mat3 model_matrix_;

		AbstractWidget* focused_widget_;

		AbstractWidget* hovered_widget_;

		RefPtr<FrameShadow> shadow_;

        GLTexture2D texture_buffer_;

		int cursor_range_;

		static int kDefaultIconSpace;

		static int kDefaultShortcutSpace;

	};

}

#endif /* _BLENDINT_MENUBIN_HPP_ */

