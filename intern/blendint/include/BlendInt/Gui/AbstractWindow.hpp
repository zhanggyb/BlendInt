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

#include <BlendInt/Core/Input.hpp>
#include <BlendInt/Gui/AbstractView.hpp>
#include <BlendInt/Gui/AbstractFrame.hpp>

#include <BlendInt/Stock/Icons.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Cursor.hpp>

namespace BlendInt {

	class AbstractWindow: public AbstractView
	{

	public:

		AbstractWindow();

		AbstractWindow(int width, int height);

		virtual ~AbstractWindow ();

		bool AddFrame (AbstractFrame* frame, bool focus = true);

		bool InsertFrame (int index, AbstractFrame* frame, bool focus = true);

		void MoveFrameToTop (AbstractFrame* frame, bool focus = true);

		virtual bool Contain (const Point& point) const;

		virtual void MakeCurrent () = 0;

		virtual void Synchronize () = 0;

		virtual void Exec () = 0;

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

		static AbstractWindow* GetContext (AbstractView* widget);

		static bool InitializeGLContext ();

		static void ReleaseGLContext ();

		static Theme* theme;

		static Icons* icons;

		static Shaders* shaders;

		static Cursor* cursor;

	protected:

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

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

		virtual bool RemoveSubView (AbstractView* view);

		void DispatchHoverEvent ();

		Cpp::ConnectionScope* events() const {return events_.get();}

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

		static bool InitializeCursor ();

		static bool InitializeFont ();

		static void ReleaseTheme ();

		static void ReleaseIcons ();

		static void ReleaseShaders ();

		static void ReleaseCursor ();

		static void ReleaseFont ();

		static void GetGLVersion (int *major, int *minor);

		static void GetGLSLVersion (int *major, int *minor);

		boost::scoped_ptr<Cpp::ConnectionScope> events_;

		AbstractFrame* active_frame_;

		// the viewport offset
		Point viewport_origin_;

		GLuint stencil_count_;

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
