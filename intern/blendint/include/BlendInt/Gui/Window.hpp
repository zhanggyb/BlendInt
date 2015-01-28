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

#include <map>

#include <GLFW/glfw3.h>

#include <BlendInt/Core/String.hpp>
#include <BlendInt/Gui/AbstractWindow.hpp>

namespace BlendInt {

	class Window: public AbstractWindow
	{
	public:

		Window (int width, int height, const char* title);

		virtual ~Window ();

		bool AddFrame (AbstractFrame* frame, bool focus = true);

		bool InsertFrame (int index, AbstractFrame* frame, bool focus = true);

		void MoveFrameToTop (AbstractFrame* frame, bool focus = true);

		virtual void Synchronize ();

		virtual void Exec ();

		virtual int GetKeyInput () const;

		virtual int GetScancode () const;

		virtual MouseAction GetMouseAction () const;

		virtual KeyAction GetKeyAction () const;

		virtual int GetModifiers () const;

		virtual MouseButton GetMouseButton () const;

		virtual const String& GetTextInput () const;

		virtual const Point& GetCursorPosition () const;

		static bool Initialize ();

		static void Terminate ();

	private:

		GLFWwindow* window_;

		static std::map<GLFWwindow*, Window*> kWindowMap;

		static KeyAction kKeyAction;

		static int kKey;

		static int kModifiers;

		static int kScancode;

		static String kText;

		static MouseAction kMouseAction;

		static MouseButton kMouseButton;

		static Point kCursor;

		static void CbError (int error, const char* description);

		static void CbWindowSize(GLFWwindow* window, int w, int h);

		static void CbKey(GLFWwindow* window, int key, int scancode, int action,
							  int mods);
	};

}
