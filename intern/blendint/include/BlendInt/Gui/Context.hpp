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

#include <set>

#include <boost/smart_ptr.hpp>

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/HID/Input.hpp>

#include <BlendInt/Core/String.hpp>
#include <BlendInt/Gui/AbstractView.hpp>
#include <BlendInt/Gui/AbstractFrame.hpp>

namespace BlendInt {

	/**
	 * @brief Layout to hold and manage all widgets in a OpenGL window
	 *
	 * Context is a special container which holds and manage all widgets in a OpenGL window.
	 * There should be at least one Context object to work with Interface to show and dispatch events.
	 */
	class Context: public AbstractView
	{
		DISALLOW_COPY_AND_ASSIGN(Context);

	public:

		static bool Initialize ();

		static void Release ();

		Context ();

		virtual ~Context ();

		bool AddFrame (AbstractFrame* frame);

		bool InsertFrame (int index, AbstractFrame* frame);

		void MoveFrameToTop (AbstractFrame* frame);

		void Draw ();

		void DispatchKeyEvent(
				KeyAction action,
				int key,
				int modifier,
				int scancode,
				String text);

		void DispatchMouseEvent (
				int x,
				int y,
				MouseAction action,
				MouseButton button,
				int modifier);

		/**
		 * @brief Always return true
		 */
		virtual bool Contain (const Point& point) const;

		virtual void SynchronizeWindow ();

		virtual void MakeGLContextCurrent ();

		AbstractFrame* leaf_frame () const
		{
#ifdef DEBUG
			assert(leaf_frame_ != nullptr);
#endif

			return leaf_frame_;
		}

		const Point& cursor_position () const
		{
			return cursor_position_;
		}

		int key() const {return key_;}

		int scancode () const {return scancode_;}

		MouseAction mouse_action() const {return mouse_action_;}

		KeyAction key_action () const {return key_action_;}

		int modifiers () const {return modifiers_;}

		MouseButton mouse_button() const {return mouse_button_;}

		const String& text () const {return text_;}

		Cpp::EventRef<const Size&> resized ()
		{
			return resized_;
		}

		static Context* GetContext (AbstractView* widget);

		static glm::mat4 default_view_matrix;

	protected:

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual bool PreDraw (Profile& profile);

		virtual ResponseType Draw (Profile& profile);

		virtual void PostDraw (Profile& profile);

		virtual void FocusEvent (bool focus);

		virtual void MouseHoverInEvent (const Context* context);

		virtual void MouseHoverOutEvent (const Context* context);

		virtual ResponseType KeyPressEvent (const Context* context);

		virtual ResponseType ContextMenuPressEvent (const Context* context);

		virtual ResponseType ContextMenuReleaseEvent (const Context* context);

		virtual ResponseType MousePressEvent (const Context* context);

		virtual ResponseType MouseReleaseEvent (const Context* context);

		virtual ResponseType MouseMoveEvent (const Context* context);

		virtual bool RemoveSubView (AbstractView* view);

		Cpp::ConnectionScope* events() const {return events_.get();}

	private:

		friend class AbstractFrame;

		static void GetGLVersion (int *major, int *minor);

		static void GetGLSLVersion (int *major, int *minor);

		void InitializeContext ();

		void DispatchHoverEvent ();

		boost::scoped_ptr<Cpp::ConnectionScope> events_;

		//void RenderToBuffer (Profile& profile);

        //GLuint vao_;
        
		Profile profile_;

        //GLTexture2D texture_buffer_;

		//GLBuffer<> vertex_buffer_;

		Point cursor_position_;

		AbstractFrame* leaf_frame_;

		// ------- input

		int modifiers_;

		KeyAction key_action_;

		int key_;

		int scancode_;

		MouseAction mouse_action_;

		MouseButton mouse_button_;

		String text_;

		// ------

		Cpp::Event<const Size&> resized_;

		static std::set<Context*> context_set;

	};

}


#endif /* _BLENDINT_GUI_CONTEXT_HPP_ */
