#include "Window.hpp"

#include <iostream>

namespace BlendInt {

	static KeyAction kKeyAction = KeyNone;
	static int kKey = 0;
	static int kModifiers = 0;
	static int kScancode = 0;
	static String kString;

	static MouseAction kMouseAction = MouseNone;
	static MouseButton kMouseButton = MouseButtonLeft;

	static Context* main_context = 0;
	static int cursor_pos_x = 0;
	static int cursor_pos_y = 0;

	static void CbError (int error, const char* description)
	{
		std::cerr << "Error: " << description
				  << " (error code: " << error << ")"
				  << std::endl;
	}

	static void CbWindowSize(GLFWwindow* window, int w, int h)
	{
		main_context->Resize(w, h);
	}

	static void CbKey(GLFWwindow* window, int key, int scancode, int action,
					  int mods)
	{
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
		kString.clear();

		main_context->DispatchKeyEvent(kKeyAction, kKey, kModifiers, kScancode, kString);
	}

	static void CbChar(GLFWwindow* window, unsigned int character)
	{

#ifdef __APPLE__
		// glfw3 in Mac OS will call this function if press some unprintalbe keys such as Left, Right, Up, Down
		if(character > 255) {
			DBG_PRINT_MSG("unprintable character in Mac: %u", character);
			return;
		}
#endif

		kString.clear();
		kString.push_back(character);

		main_context->DispatchKeyEvent(kKeyAction, kKey, kModifiers, kScancode, kString);
	}

	static void CbMouseButton(GLFWwindow* window, int button, int action,
							  int mods)
	{
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

		main_context->DispatchMouseEvent(cursor_pos_x, cursor_pos_y, kMouseAction, kMouseButton, kModifiers);
	}

	static void CbCursorPos(GLFWwindow* window, double xpos, double ypos)
	{
		cursor_pos_x = (int)xpos;
		cursor_pos_y = (int)ypos;

		kMouseAction = MouseMove;
		kMouseButton = MouseButtonNone;

		main_context->DispatchMouseEvent(cursor_pos_x, cursor_pos_y, kMouseAction, kMouseButton, kModifiers);
	}

#ifdef __APPLE__

	// MUST set this callback to render the context when resizing in OSX
	static void CbWindowRefresh (GLFWwindow* window)
	{
		main_context->Draw();
		glfwSwapBuffers(window);
	}

#endif

	void Init ()
	{
		/* Initialize the library */
		if (!glfwInit())
			exit(-1);

		glfwSetErrorCallback(&CbError);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        //glfwWindowHint(GLFW_REFRESH_RATE, 1);

	}

	GLFWwindow* CreateWindow (const char* name, int width, int height)
	{
		GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
		if (!window) {
			glfwTerminate();
			exit(-1);
		}

		glfwSetWindowSizeCallback(window, &CbWindowSize);
		glfwSetKeyCallback(window, &CbKey);
		glfwSetCharCallback(window, &CbChar);
		glfwSetMouseButtonCallback(window, &CbMouseButton);
		glfwSetCursorPosCallback(window, &CbCursorPos);
#ifdef __APPLE__
		glfwSetWindowRefreshCallback(window, &CbWindowRefresh);
#endif

		/* Make the window's context current */
		glfwMakeContextCurrent(window);
	
		/* initialize BlendInt after OpenGL content is created */
		if (!Context::Initialize()) {
			glfwTerminate();
			exit(-1);
		}

		return window;
	}

	void RunLoop (GLFWwindow* window, RenderCallback callback, void* param)
	{
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window)) {

			if(main_context->refresh()) {
    			/* Render here */
    			main_context->Draw();
                if(callback) {
                    (*callback)(param);
                }
    			/* Swap front and back buffers */
    			glfwSwapBuffers(window);
            }

			/* Wait for and process events */
            glfwWaitEvents();
		}
	
	}

	void Terminate ()
	{
		/* release BlendInt */
		Context::Release();
		glfwTerminate();
		main_context = 0;
	}

	void SetContext (Context* context)
	{
		if(main_context) {
			delete main_context;
			main_context = 0;
		}

		main_context = context;
	}

}
