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

#ifndef _BLENDINT_GUI_CONTEXT_HPP_
#define _BLENDINT_GUI_CONTEXT_HPP_

#include <map>

#include <BlendInt/Core/Point.hpp>
#include <BlendInt/Core/Size.hpp>

#include <BlendInt/OpenGL/ScissorStatus.hpp>

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/OpenGL/GLArrayBuffer.hpp>

#include <BlendInt/Gui/AbstractContainer.hpp>
#include <BlendInt/Gui/Section.hpp>

namespace BlendInt {

	class Interface;

	/**
	 * @brief Container to hold and manage all widgets in a OpenGL window
	 *
	 * Context is a special container which holds and manage all widgets in a OpenGL window.
	 * There should be at least one Context object to work with Interface to show and dispatch events.
	 */
	class Context: public AbstractContainer
	{
		DISALLOW_COPY_AND_ASSIGN(Context);

		friend class Interface;

	public:

		Context ();

		virtual ~Context ();

		/**
		 * @brief Add a widget in a new section
		 *
		 * @note The section will be deleted if the last sub widget is removed or destroyed.
		 *
		 * The widget could also be a Section object
		 *
		 */
		Section* PushBack (AbstractWidget* widget);

		/**
		 * @brief Remove widget from this context
		 *
		 * @warning: this function will remove widget from any container, if it's in a section hold by this container, the section will be destroyed by itself if managed.
		 */
		Section* Remove (AbstractWidget* widget);

		void MoveToTop (const Section* section);

		void MoveToBottom (const Section* section);

		void MoveUp (const Section* seciton);

		void MoveDown (const Section* section);

		void SetFocusedWidget (AbstractWidget* widget);

		/**
		 * @brief Always return true
		 */
		virtual bool Contain (const Point& point) const;

		AbstractWidget* focused_widget () const
		{
			//return m_focused_widget;
			return 0;
		}

		/**
		 * @brief Set the cursor
		 */
		virtual void SetCursor (int cursor_type);

		virtual int GetCursor () const;

		virtual void PushCursor (int cursor_type);

		virtual int PopCursor ();

		Cpp::EventRef<const Size&> resized ()
		{
			return m_resized;
		}

		static Context* GetContext (AbstractWidget* widget);

		static void RenderToTexture (AbstractWidget* widget, GLTexture2D* texture);

		static void RenderToFile (AbstractWidget* widget, const char* filename);

		static glm::mat4 default_view_matrix;

	protected:

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

		virtual bool RoundTypeUpdateTest (const RoundTypeUpdateRequest& request);

		virtual bool RoundRadiusUpdateTest (const RoundRadiusUpdateRequest& request);

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request);

		virtual void PerformRoundRadiusUpdate (const RoundRadiusUpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType FocusEvent (bool focus);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	private:

		void InitializeContext ();

		AbstractWidget* GetWidgetUnderCursor (const MouseEvent& event, AbstractWidget* parent);

		void OnFocusedWidgetDestroyed (AbstractWidget* widget);

		RedrawEvent m_redraw_event;

		bool m_refresh;

		AbstractWidget* m_focused_widget;

		Cpp::Event<const Size&> m_resized;

		static std::set<Context*> context_set;
	};

}


#endif /* _BLENDINT_GUI_CONTEXT_HPP_ */
