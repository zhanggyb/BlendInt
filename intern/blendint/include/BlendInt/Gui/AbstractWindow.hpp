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

		virtual bool Contain (const Point& point) const;

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

		static glm::mat4 default_view_matrix;

	private:

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

		void InitializeMatrices ();

		boost::scoped_ptr<Cpp::ConnectionScope> events_;

		AbstractFrame* active_frame_;

		// the viewport offset
		Point viewport_origin_;

		GLuint stencil_count_;

	};

}
