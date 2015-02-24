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

#pragma once

#include <GLFW/glfw3.h>
#include <boost/smart_ptr.hpp>

#include <core/input.hpp>
#include <gui/abstract-view.hpp>
#include <gui/abstract-frame.hpp>

#include <stock/icons.hpp>
#include <stock/theme.hpp>
#include <stock/shaders.hpp>

namespace BlendInt {

	/**
	 * @brief Abstract class for window
	 *
	 * The AbstractWindow class represents a window in the desktop.
	 *
	 * A sub class should override the virual functions and make sure
	 * an OpenGL context is created.
	 *
	 * @see Window
	 */
	class AbstractWindow: public AbstractView
	{

	public:

		/**
		 * @brief Default constructor
		 *
		 * Create a 640, 480 window
		 */
		AbstractWindow();

		/**
		 * @brief Constructor with window size
		 */
		AbstractWindow(int width, int height);

		/**
		 * @brief Destructor
		 */
		virtual ~AbstractWindow ();

		/**
		* @brief Add a frame in this window
		*/
		bool AddFrame (AbstractFrame* frame);

		bool SetFocusedFrame (AbstractFrame* frame);

		virtual bool Contain (const Point& point) const;

		/**
		 * @brief Make this window as the current OpenGL Context
		 */
		virtual void MakeCurrent () = 0;

		/**
		 * @brief Synchronize this window to redraw contents
		 */
		virtual void Synchronize () = 0;

		virtual void Exec () = 0;

		virtual void SetCursor (CursorShape cursor_type);

		void PushCursor ();

		void PopCursor ();

		void BeginPushStencil ();

		void EndPushStencil ();

		void BeginPopStencil ();

		void EndPopStencil ();

		virtual int GetKeyInput () const = 0;

		virtual int GetScancode () const = 0;

		virtual MouseAction GetMouseAction () const = 0;

		virtual KeyAction GetKeyAction () const = 0;

		virtual int GetModifiers () const = 0;

		virtual MouseButton GetMouseButton () const = 0;

		virtual const String& GetTextInput () const = 0;

		virtual const Point& GetCursorPosition () const = 0;

		Point GetAbsolutePosition (const AbstractWidget* widget);

		Point GetRelativePosition (const AbstractWidget* widget);

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

		const Point& viewport_origin () const
		{
			return viewport_origin_;
		}

		/**
		 * @brief Get the current cursor shape
		 */
		inline CursorShape current_cursor_shape () const
		{
			return current_cursor_shape_;
		}

		Cpp::ConnectionScope* events() const {return events_.get();}

		static AbstractWindow* GetWindow (AbstractView* widget);

		static bool InitializeGLContext ();

		static void ReleaseGLContext ();

		static Theme* theme;

		static Icons* icons;

		static Shaders* shaders;

	protected:

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

		virtual bool PreDraw (AbstractWindow* context);

		virtual Response Draw (AbstractWindow* context);

		virtual void PostDraw (AbstractWindow* context);

		virtual void PerformFocusOn (AbstractWindow* context);

		virtual void PerformFocusOff (AbstractWindow* context);

		virtual void PerformHoverIn (AbstractWindow* context);

		virtual void PerformHoverOut (AbstractWindow* context);

		virtual Response PerformKeyPress (AbstractWindow* context);

		virtual Response PerformContextMenuPress (AbstractWindow* context);

		virtual Response PerformContextMenuRelease (AbstractWindow* context);

		virtual Response PerformMousePress (AbstractWindow* context);

		virtual Response PerformMouseRelease (AbstractWindow* context);

		virtual Response PerformMouseMove (AbstractWindow* context);

		virtual bool RemoveSubView (AbstractView* view);

		void PerformMouseHover ();

		inline void set_viewport_origin (int x, int y)
		{
			viewport_origin_.reset(x, y);
		}

		inline void set_viewport_origin (const Point& point)
		{
			viewport_origin_ = point;
		}

		inline GLuint stencil_count () const
		{
			return stencil_count_;
		}

		inline void set_stencil_count (GLuint count)
		{
			stencil_count_ = count;
		}

		static glm::mat4 default_view_matrix;

	private:

		friend class AbstractFrame;
		friend class AbstractWidget;

		static bool InitializeTheme ();

		static bool InitializeIcons ();

		static bool InitializeShaders ();

		static bool InitializeFont ();

		static void ReleaseTheme ();

		static void ReleaseIcons ();

		static void ReleaseShaders ();

		static void ReleaseFont ();

		static void GetGLVersion (int *major, int *minor);

		static void GetGLSLVersion (int *major, int *minor);

		boost::scoped_ptr<Cpp::ConnectionScope> events_;

		AbstractFrame* active_frame_;

		AbstractFrame* focused_frame_;

		// the viewport offset
		Point viewport_origin_;

		GLuint stencil_count_;

		CursorShape current_cursor_shape_;

		std::stack<CursorShape> cursor_stack_;

		int always_on_top_frame_count_;

		bool pressed_;
	};

	inline int pixel_size (int a)
	{
		return a * AbstractWindow::theme->pixel();
	}

	inline int pixel_size (unsigned int a)
	{
		return a * AbstractWindow::theme->pixel();
	}

}
