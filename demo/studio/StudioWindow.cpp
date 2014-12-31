#include "StudioWindow.hpp"

#include <iostream>

#include <BlendInt/HID/KeyEvent.hpp>
#include <BlendInt/HID/MouseEvent.hpp>

namespace BlendInt {

	static KeyEvent key_event;
	static MouseEvent mouse_event;
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
                key_event.set_action(KeyPress);
				break;
			case GLFW_RELEASE:
                key_event.set_action(KeyRelease);
				break;
			case GLFW_REPEAT:
                key_event.set_action(KeyRepeat);
				break;
			default:
                key_event.set_action(KeyNone);
				break;
		}

		key_event.set_key(key);
		key_event.set_scancode(scancode);
		key_event.set_modifiers(mods);
        key_event.clear_text();

		main_context->DispatchKeyEvent(key_event.action(), key_event.key(), key_event.modifiers(), key_event.scancode(), key_event.text());
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

		key_event.set_text(character);

		main_context->DispatchKeyEvent(key_event.action(), key_event.key(), key_event.modifiers(), key_event.scancode(), key_event.text());
	}

	static void CbMouseButton(GLFWwindow* window, int button, int action,
							  int mods)
	{
		MouseAction mouse_action = MouseNone;

		switch (action) {
			case GLFW_RELEASE:
				mouse_action = MouseRelease;
				break;
			case GLFW_PRESS:
				mouse_action = MousePress;
				break;
			case GLFW_REPEAT:
				mouse_action = MouseNone;
				break;
			default:
				break;
		}

		MouseButton mouse_button = MouseButtonNone;

		switch(button) {
			case GLFW_MOUSE_BUTTON_1:
				mouse_button = MouseButtonLeft;
				break;
			case GLFW_MOUSE_BUTTON_2:
				mouse_button = MouseButtonRight;
				break;
			case GLFW_MOUSE_BUTTON_3:
				mouse_button = MouseButtonMiddle;
				break;
			default:
				break;
		}

		mouse_event.set_button(mouse_button);
		mouse_event.set_action(mouse_action);
		mouse_event.set_modifiers(mods);

		main_context->DispatchMouseEvent(cursor_pos_x, cursor_pos_y, mouse_event.action(), mouse_event.button(), mouse_event.modifiers());
	}

	static void CbCursorPos(GLFWwindow* window, double xpos, double ypos)
	{
		cursor_pos_x = (int)xpos;
		cursor_pos_y = (int)ypos;

        mouse_event.set_action(MouseMove);
        mouse_event.set_button(MouseButtonNone);

		main_context->DispatchMouseEvent(cursor_pos_x, cursor_pos_y, mouse_event.action(), mouse_event.button(), mouse_event.modifiers());
	}

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

			/* Poll for and process events */
#ifdef __APPLE__
            glfwWaitEvents();
#else
            glfwWaitEvents();
#endif  // __APPLE__
		}
	
	}

	void Terminate ()
	{
		/* release BlendInt */
		Context::Release();
		glfwTerminate();
		main_context = 0;
	}

	bool CheckAllocatedObjects ()
	{
		return true;
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
