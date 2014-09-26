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

#include <stack>
#include <set>

#include <BlendInt/OpenGL/GLTexture2D.hpp>

#include <BlendInt/Gui/AbstractContainer.hpp>
#include <BlendInt/Gui/Section.hpp>
#include <BlendInt/Gui/AbstractScreen.hpp>

namespace BlendInt {

	/**
	 * @brief Container to hold and manage all widgets in a OpenGL window
	 *
	 * Context is a special container which holds and manage all widgets in a OpenGL window.
	 * There should be at least one Context object to work with Interface to show and dispatch events.
	 */
	class Context: public AbstractContainer
	{
		DISALLOW_COPY_AND_ASSIGN(Context);

	public:

		static bool Initialize ();

		static void Release ();

		Context ();

		virtual ~Context ();

		void AddScreen (AbstractScreen* vp);

		void Draw ();

		void DispatchKeyEvent (const KeyEvent& event);

		void DispatchMouseEvent (const MouseEvent& event);

		void SetFocusedViewport (AbstractScreen* viewport);

		/**
		 * @brief Always return true
		 */
		virtual bool Contain (const Point& point) const;

		/**
		 * @brief Set the cursor
		 */
		virtual void SetCursor (int cursor_type);

		int current_cursor () const
		{
			return current_cursor_;
		}

		void PushCursor (int cursor_type);

		int PopCursor ();

		Cpp::EventRef<const Size&> resized ()
		{
			return resized_;
		}

		static Context* GetContext (AbstractWidget* widget);

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

		virtual ResponseType Draw (Profile& profile);

		virtual void PostDraw (Profile& profile);

		virtual ResponseType FocusEvent (bool focus);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

	private:

		static void GetGLVersion (int *major, int *minor);

		static void GetGLSLVersion (int *major, int *minor);

		void InitializeContext ();

		void OnHoverViewportDestroyed (AbstractWidget* widget);

		Profile profile_;

		int current_cursor_;

		AbstractScreen* hover_;

		std::stack<int> cursor_stack_;

		Cpp::Event<const Size&> resized_;

		static std::set<Context*> context_set;

	};

}


#endif /* _BLENDINT_GUI_CONTEXT_HPP_ */
