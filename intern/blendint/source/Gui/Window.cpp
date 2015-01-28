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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/Window.hpp>

namespace BlendInt {

	std::map<GLFWwindow*, Window*> Window::kWindowMap;

	KeyAction Window::kKeyAction = KeyNone;

	int Window::kKey = 0;

	int Window::kModifiers = 0;

	int Window::kScancode = 0;

	String Window::kText;

	MouseAction Window::kMouseAction = MouseNone;

	MouseButton Window::kMouseButton = MouseButtonLeft;

	Point Window::kCursor;

	Window::Window (int width, int height, const char* name)
	: AbstractWindow(width, height),
	  window_(0)
	{
		window_ = glfwCreateWindow(width, height, name, NULL, NULL);

		if(!window_) {
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		glfwSetWindowSizeCallback(window_, &CbWindowSize);
//		glfwSetKeyCallback(window, &CbKey);
//		glfwSetCharCallback(window, &CbChar);
//		glfwSetMouseButtonCallback(window, &CbMouseButton);
//		glfwSetCursorPosCallback(window, &CbCursorPos);
//#ifdef __APPLE__
//		glfwSetWindowRefreshCallback(window, &CbWindowRefresh);
//#endif

		/* Make the window's context current */
		glfwMakeContextCurrent(window_);

		kWindowMap[window_] = this;
	}

	Window::~Window ()
	{
		kWindowMap.erase(window_);
		window_ = 0;
	}

	bool Window::AddFrame (AbstractFrame* frame, bool focus)
	{
		return false;
	}

	bool Window::InsertFrame (int index, AbstractFrame* frame, bool focus)
	{
		return false;
	}

	void Window::MoveFrameToTop (AbstractFrame* frame, bool focus)
	{
	}

	void Window::Synchronize ()
	{

	}

	void Window::Exec ()
	{
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window_)) {

			glClearColor(0.208f, 0.208f, 0.208f, 1.f);
			//glClearColor(1.f, 1.f, 1.f, 1.f);
			glClearStencil(0);
			glClearDepth(1.0);

			glClear(GL_COLOR_BUFFER_BIT |
					GL_DEPTH_BUFFER_BIT |
					GL_STENCIL_BUFFER_BIT);

//			if(main_context->refresh()) {
//    			/* Render here */
//    			main_context->Draw();
//                if(callback) {
//                    (*callback)(param);
//                }
//    			/* Swap front and back buffers */
    			glfwSwapBuffers(window_);
//            }

			/* Poll for and process events */
#ifdef __APPLE__
            glfwWaitEvents();
#else
            glfwWaitEvents();
#endif  // __APPLE__
		}
	}

	int Window::GetKeyInput () const
	{
		return kKey;
	}

	int Window::GetScancode () const
	{
		return kScancode;
	}

	MouseAction Window::GetMouseAction () const
	{
		return kMouseAction;
	}

	KeyAction Window::GetKeyAction () const
	{
		return kKeyAction;
	}

	int Window::GetModifiers () const
	{
		return kModifiers;
	}

	MouseButton Window::GetMouseButton () const
	{
		return kMouseButton;
	}

	const String& Window::GetTextInput () const
	{
		return kText;
	}

	const Point& Window::GetCursorPosition () const
	{
		return kCursor;
	}

	bool Window::Initialize ()
	{
		/* Initialize the library */
		if (!glfwInit()) {
			DBG_PRINT_MSG("Critical: %s", "Cannot initialize GLFW");
			return false;
		}

		glfwSetErrorCallback(&CbError);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		return true;
	}

	void Window::Terminate ()
	{
		glfwTerminate();
	}

	void Window::CbError (int error, const char* description)
	{
		DBG_PRINT_MSG("Error: %s (error code: %d)", description, error);
	}

	void Window::CbWindowSize(GLFWwindow* window, int w, int h)
	{
		Window* win = kWindowMap[window];

		win->Resize(w, h);
	}

	void Window::CbKey (GLFWwindow* window, int key, int scancode, int action,
	        int mods)
	{
		//Window* win = kWindowMap[window];

		//win->PerformKeyPress();
	}

}
