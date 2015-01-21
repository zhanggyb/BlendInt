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
#include <BlendInt/Core/Input.hpp>

#include <BlendInt/Core/String.hpp>
#include <BlendInt/Gui/AbstractView.hpp>
#include <BlendInt/Gui/AbstractFrame.hpp>

#include <BlendInt/Stock/Icons.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Cursor.hpp>

namespace BlendInt {

	/**
	 * @brief The root view to interact with window system and manage
	 * the interface
	 *
	 * Context is a special view in BlendInt, it provide public
	 * entries for the windows system to control the display and input
	 * events.
	 *
	 * There should be at least one Context object to work with
	 * Interface to show and dispatch events.
	 */
	class Context: public AbstractView
	{
		DISALLOW_COPY_AND_ASSIGN(Context);

	public:

		static bool Initialize ();

		static void Release ();

		Context ();

		virtual ~Context ();

		bool AddFrame (AbstractFrame* frame, bool focus = true);

		bool InsertFrame (int index, AbstractFrame* frame, bool focus = true);

		void MoveFrameToTop (AbstractFrame* frame, bool focus = true);

		void Draw ();

		void DispatchKeyEvent (KeyAction action,
							   int key,
							   int modifier,
							   int scancode,
							   String text);

		void DispatchMouseEvent (int x,
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

		void register_active_frame (AbstractFrame* frame)
		{
			active_frame_ = frame;
		}

		AbstractFrame* active_frame () const
		{
#ifdef DEBUG
			assert(active_frame_ != nullptr);
#endif
			return active_frame_;
		}

		const Point& cursor_position () const
		{
			return cursor_position_;
		}

		const Point& viewport_origin () const
		{
			return viewport_origin_;
		}

		void BeginPushStencil ();

		void EndPushStencil ();

		void BeginPopStencil ();

		void EndPopStencil ();

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

		static Theme* theme;

		static Icons* icons;

		static Shaders* shaders;

		static Cursor* cursor;

	protected:

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual bool PreDraw (const Context* context);

		virtual ResponseType Draw (const Context* context);

		virtual void PostDraw (const Context* context);

		virtual void PerformFocusOn (const Context* context);

		virtual void PerformFocusOff (const Context* context);

		virtual void PerformHoverIn (const Context* context);

		virtual void PerformHoverOut (const Context* context);

		virtual ResponseType PerformKeyPress (const Context* context);

		virtual ResponseType PerformContextMenuPress (const Context* context);

		virtual ResponseType PerformContextMenuRelease (const Context* context);

		virtual ResponseType PerformMousePress (const Context* context);

		virtual ResponseType PerformMouseRelease (const Context* context);

		virtual ResponseType PerformMouseMove (const Context* context);

		virtual bool RemoveSubView (AbstractView* view);

		Cpp::ConnectionScope* events() const {return events_.get();}

	private:

		friend class AbstractFrame;
		friend class AbstractWidget;

		static bool InitializeTheme ();

		static bool InitializeIcons ();

		static bool InitializeShaders ();

		static bool InitializeCursor ();

		static bool InitializeFont ();

		static void ReleaseTheme ();

		static void ReleaseIcons ();

		static void ReleaseShaders ();

		static void ReleaseCursor ();

		static void ReleaseFont ();

		static void GetGLVersion (int *major, int *minor);

		static void GetGLSLVersion (int *major, int *minor);

		void InitializeContext ();

		void DispatchHoverEvent ();

		boost::scoped_ptr<Cpp::ConnectionScope> events_;

		Point cursor_position_;

		AbstractFrame* active_frame_;

		// ------- input

		int modifiers_;

		KeyAction key_action_;

		int key_;

		int scancode_;

		MouseAction mouse_action_;

		MouseButton mouse_button_;

		String text_;

		// the following 2 variables are used when rendering

		// the viewport offset
		Point viewport_origin_;

		GLuint stencil_count_;

		// ------

		Cpp::Event<const Size&> resized_;

		static std::set<Context*> context_set;

		static glm::mat4 default_view_matrix;
	};

	inline float pixel_size (int a)
	{
		return a * Context::theme->pixel();
	}

	inline float pixel_size (unsigned int a)
	{
		return a * Context::theme->pixel();
	}

}

#endif /* _BLENDINT_GUI_CONTEXT_HPP_ */
