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

#include <iostream>

#include <BlendInt/Window.hpp>
#include <BlendInt/AbstractWidget.hpp>
#include <BlendInt/KeyEvent.hpp>
#include <BlendInt/MouseEvent.hpp>

namespace BlendInt {

	Window* Window::m_window = 0;

	void Window::Initialize ()
	{
		/* Initialize the library */
		if (!glfwInit())
			exit(-1);

		glfwSetErrorCallback(&ErrorCallback);
	}

	Window* Window::Create (const char* name, int width, int height)
	{
		if(!m_window) {
			m_window = new Window;

			m_window->m_glfw_window = glfwCreateWindow(width, height, name, NULL, NULL);
			if (!m_window->m_glfw_window) {
				delete m_window;
				m_window = 0;
				glfwTerminate();
				exit(-1);
			}

			glfwSetWindowSizeCallback(m_window->m_glfw_window, &ResizeCallback);
			glfwSetKeyCallback(m_window->m_glfw_window, &KeyboardCallback);
			glfwSetCharCallback(m_window->m_glfw_window, &CharacterInputCallback);
			glfwSetMouseButtonCallback(m_window->m_glfw_window, &MouseButtonCallback);
			glfwSetCursorPosCallback(m_window->m_glfw_window, &MouseMoveCallback);
			glfwSetWindowFocusCallback(m_window->m_glfw_window, &WindowFocusCallback);

			/* Make the window's context current */
			glfwMakeContextCurrent(m_window->m_glfw_window);
		}

		return m_window;
	}

	void Window::Run ()
	{
		if(!m_window) {
			std::cerr << "Window is not created" << std::endl;
			return;
		}

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(m_window->m_glfw_window)) {

			/* Render here */
			Interface::Instance()->Draw();

			/* Swap front and back buffers */
			glfwSwapBuffers(m_window->m_glfw_window);

			/* Poll for and process events */
			//glfwPollEvents();
			glfwWaitEvents();
		}
	}

	void Window::Release ()
	{
		if(m_window) {
            glfwDestroyWindow(m_window->m_glfw_window);
            glfwTerminate();
			delete m_window;
			m_window = 0;
		}
	}

	void Window::Refresh ()
	{
		if(m_window) {

			//glfwSwapBuffers(m_window->m_glfw_window);
			// glfwPollEvents();

			//TODO: refresh the opengl context
		}
	}

	void Window::Resize(int width, int height)
	{
		glfwSetWindowSize (m_glfw_window, width, height);
	}

	Window::Window ()
	: m_glfw_window(0),
	  m_key_event(0),
	  m_mouse_event(0)
	{
		m_key_event = new KeyEvent;
		m_mouse_event = new MouseEvent;
	}

	Window::~Window ()
	{
		delete m_key_event;
		delete m_mouse_event;
	}

	void Window::ErrorCallback (int error, const char* description)
	{
		std::cerr << "Error: " << description
				  << " (error code: " << error << ")"
				  << std::endl;
	}

	void Window::ResizeCallback(GLFWwindow* window, int w, int h)
	{
		Interface::Instance()->Resize(w, h);
	}

	void Window::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action,
					  int mods)
	{
		switch (action) {
			case GLFW_PRESS:
                m_window->m_key_event->set_action(KeyPress);
				break;
			case GLFW_RELEASE:
                m_window->m_key_event->set_action(KeyRelease);
				break;
			case GLFW_REPEAT:
                m_window->m_key_event->set_action(KeyRepeat);
				break;
			default:
                m_window->m_key_event->set_action(KeyNone);
				break;
		}

		m_window->m_key_event->set_key(key);
		m_window->m_key_event->set_scancode(scancode);
		m_window->m_key_event->set_modifiers(mods);
		m_window->m_key_event->clear_text();

		Interface::Instance()->DispatchKeyEvent(m_window->m_key_event);
	}

	void Window::CharacterInputCallback(GLFWwindow* window, unsigned int character)
	{
		m_window->m_key_event->set_text(character);

		Interface::Instance()->DispatchKeyEvent(m_window->m_key_event);
	}

	void Window::MouseButtonCallback(GLFWwindow* window, int button, int action,
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

		m_window->m_mouse_event->set_button(mouse_button);
		m_window->m_mouse_event->set_action(mouse_action);
		m_window->m_mouse_event->set_modifiers(mods);

		Interface::Instance()->DispatchMouseEvent(m_window->m_mouse_event);
	}

	void Window::MouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
	{
		m_window->m_mouse_event->set_action(MouseMove);
		m_window->m_mouse_event->set_button(MouseButtonNone);
		m_window->m_mouse_event->set_position(static_cast<int>(xpos), Interface::Instance()->size().height() - static_cast<int>(ypos));

		Interface::Instance()->DispatchMouseEvent(m_window->m_mouse_event);
	}

	void Window::WindowFocusCallback (GLFWwindow* window, int focused)
	{
		if(focused == GL_TRUE) {
			glfwMakeContextCurrent(window);
		}
	}

}
