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

	Window::Window (int width, int height, const char* title)
	: AbstractWindow(width, height),
	  window_(0)
	{
		window_ = glfwCreateWindow(width, height, title, NULL, NULL);

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

		if(!InitializeGLContext()) {
			DBG_PRINT_MSG("Critical: %s", "Cannot initialize GL Context");
			exit(EXIT_FAILURE);
		}

		glm::mat4 projection = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		shaders->SetFrameProjectionMatrix(projection);
		shaders->SetFrameViewMatrix(default_view_matrix);
		shaders->SetFrameModelMatrix(glm::mat3(1.f));

		shaders->SetWidgetViewMatrix(default_view_matrix);
		shaders->SetWidgetModelMatrix(glm::mat3(1.f));

		kWindowMap[window_] = this;
	}

	Window::Window (int width, int height, const char* title,
	        const Window& share)
	: AbstractWindow(width, height),
	  window_(0)
	{
		window_ = glfwCreateWindow(width, height, title, NULL, share.window_);

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
		ReleaseGLContext();

		kWindowMap.erase(window_);
		window_ = 0;
	}

	void Window::MakeCurrent ()
	{
		glfwMakeContextCurrent(window_);
	}

	void Window::Synchronize ()
	{
		glfwPostEmptyEvent();
	}

	void Window::Exec ()
	{
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window_)) {

			if(refresh()) {

				glClearColor(0.208f, 0.208f, 0.208f, 1.f);
				//glClearColor(1.f, 1.f, 1.f, 1.f);
				glClearStencil(0);
				glClearDepth(1.0);

				glClear(GL_COLOR_BUFFER_BIT |
						GL_DEPTH_BUFFER_BIT |
						GL_STENCIL_BUFFER_BIT);

				// Here cannot enable depth test -- glEnable(GL_DEPTH_TEST);

				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);

				set_viewport_origin(0, 0);
				if(stencil_count() != 0) {
					DBG_PRINT_MSG("Warning: %s, stencil_count_: %u", "stencil used but not released", stencil_count());
				}
				set_stencil_count(0);

				glViewport(0, 0, size().width(), size().height());

				set_refresh(false);
				if(PreDraw(this)) {
					Draw(this);
					PostDraw(this);
				}

    			glfwSwapBuffers(window_);
			}

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

	void Window::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			set_size(*request.size());

			glm::mat4 projection = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
			shaders->SetFrameProjectionMatrix(projection);

			set_refresh(true);
		}
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
		Window* win = kWindowMap[window];

		switch (action) {
			case GLFW_PRESS:
				kKeyAction = KeyPress;
				break;
			case GLFW_RELEASE:
				kKeyAction = KeyRelease;
				break;
			case GLFW_REPEAT:
				kKeyAction = KeyRepeat;
				break;
			default:
				kKeyAction = KeyNone;
				break;
		}

		kKey = key;
		kModifiers = mods;
		kScancode = scancode;
		kText.clear();

		win->register_active_frame(0);

		switch (kKeyAction) {

			case KeyPress: {
				win->PerformKeyPress(win);
				break;
			}

			case KeyRelease: {
				break;
			}

			case KeyRepeat: {
				break;
			}

			default:
				break;
		}
	}

	void Window::CbChar (GLFWwindow* window, unsigned int character)
	{
		Window* win = kWindowMap[window];

#ifdef __APPLE__
		// glfw3 in Mac OS will call this function if press some unprintalbe keys such as Left, Right, Up, Down
		if(character > 255) {
			DBG_PRINT_MSG("unprintable character in Mac: %u", character);
			return;
		}
#endif

		kText.clear();
		kText.push_back(character);

		win->register_active_frame(0);

		switch (kKeyAction) {

			case KeyPress: {
				win->PerformKeyPress(win);
				break;
			}

			case KeyRelease: {
				break;
			}

			case KeyRepeat: {
				break;
			}

			default:
				break;
		}

	}

	void Window::CbMouseButton (GLFWwindow* window, int button, int action,
	        int mods)
	{
		Window* win = kWindowMap[window];

		switch (action) {
			case GLFW_RELEASE:
				kMouseAction = MouseRelease;
				break;
			case GLFW_PRESS:
				kMouseAction = MousePress;
				break;
			case GLFW_REPEAT:
				kMouseAction = MouseNone;
				break;
			default:
				break;
		}

		kMouseButton = MouseButtonNone;

		switch(button) {
			case GLFW_MOUSE_BUTTON_1:
				kMouseButton = MouseButtonLeft;
				break;
			case GLFW_MOUSE_BUTTON_2:
				kMouseButton = MouseButtonRight;
				break;
			case GLFW_MOUSE_BUTTON_3:
				kMouseButton = MouseButtonMiddle;
				break;
			default:
				break;
		}

		kModifiers = mods;

		win->register_active_frame(0);

		switch (kMouseAction) {

			case MouseMove: {
				win->DispatchHoverEvent();
				win->PerformMouseMove(win);
				break;
			}

			case MousePress: {
				win->DispatchHoverEvent();
				win->PerformMousePress(win);
				break;
			}

			case MouseRelease: {
				win->PerformMouseRelease(win);
				win->DispatchHoverEvent();
				break;
			}

			default:
				break;
		}

	}

	void Window::CbCursorPos (GLFWwindow* window, double xpos, double ypos)
	{
		Window* win = kWindowMap[window];

		kCursor.reset((int)xpos, win->size().height() - (int)ypos);

		kMouseAction = MouseMove;
		kMouseButton = MouseButtonNone;

		win->register_active_frame(0);

		switch (kMouseAction) {

			case MouseMove: {
				win->DispatchHoverEvent();
				win->PerformMouseMove(win);
				break;
			}

			case MousePress: {
				win->DispatchHoverEvent();
				win->PerformMousePress(win);
				break;
			}

			case MouseRelease: {
				win->PerformMouseRelease(win);
				win->DispatchHoverEvent();
				break;
			}

			default:
				break;
		}

	}

#ifdef __APPLE__

	// MUST set this callback to render the context when resizing in OSX
	static void Window::CbWindowRefresh (GLFWwindow* window)
	{
		Window* win = kWindowMap[window];

		glfwSwapBuffers(win->window_);
	}

#endif

}
