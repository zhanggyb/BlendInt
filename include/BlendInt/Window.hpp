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

#ifndef _BLENDINT_WINDOW_HPP_
#define _BLENDINT_WINDOW_HPP_

#include <GLFW/glfw3.h>

#include <BlendInt/Interface.hpp>

namespace BlendInt {

	/**
	 * @brief The window class to contain the opengl context
	 */
	class Window
	{
	public:

		static void Initialize ();

		static Window* Create (const char* name, int width, int height);

		static void Run ();

		static void Release ();

		static void Refresh ();

#ifdef DEBUG
		static bool CheckAllocatedObjects ();
#endif

	private:

		static void ErrorCallback (int error, const char* description);

		static void ResizeCallback (GLFWwindow* window, int width, int height);

		static void KeyboardCallback (GLFWwindow* window, int key, int scancode, int action, int mods);

		static void CharacterInputCallback(GLFWwindow* window, unsigned int character);

		static void MouseButtonCallback (GLFWwindow* window, int button, int action, int mods);

		static void MouseMoveCallback (GLFWwindow* window, double xpos, double ypos);

		Window ();

		~Window();

		static Window* m_window;

		GLFWwindow* m_glfw_window;

		KeyEvent* m_key_event;

		MouseEvent* m_mouse_event;
	};

}

#endif	// _BLENDINT_WINDOW_HPP_

