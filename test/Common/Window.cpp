#include "Window.hpp"

#include <iostream>

//using BlendInt::Interface;
#include <BlendInt/AbstractWidget.hpp>

namespace BlendInt {

	static void CbError (int error, const char* description)
	{
		std::cerr << "Error: " << description
				  << " (error code: " << error << ")"
				  << std::endl;
	}

	static void CbWindowSize(GLFWwindow* window, int w, int h) {
		Interface::instance()->Resize(w, h);
	}

	static void CbKey(GLFWwindow* window, int key, int scancode, int action,
					  int mods) {
		Interface::instance()->GLFWKeyEvent(key, scancode, action, mods);
	}

	static void CbMouseButton(GLFWwindow* window, int button, int action,
							  int mods) {
		Interface::instance()->GLFWMouseButtonEvent(button, action, mods);
	}

	static void CbCursorPos(GLFWwindow* window, double xpos, double ypos) {
		Interface::instance()->GLFWCursorPosEvent(xpos, ypos);
	}

	void Init ()
	{
		/* Initialize the library */
		if (!glfwInit())
			exit(-1);

		glfwSetErrorCallback(&CbError);
	}

	GLFWwindow* CreateWindow (const char* name)
	{
		GLFWwindow* window = glfwCreateWindow(1200, 800, name, NULL, NULL);
		if (!window) {
			glfwTerminate();
			exit(-1);
		}

		glfwSetWindowSizeCallback(window, &CbWindowSize);
		glfwSetKeyCallback(window, &CbKey);
		glfwSetMouseButtonCallback(window, &CbMouseButton);
		glfwSetCursorPosCallback(window, &CbCursorPos);

		/* Make the window's context current */
		glfwMakeContextCurrent(window);
	
		/* initialize BlendInt after OpenGL content is created */
		if (!Interface::initialize()) {
			glfwTerminate();
			exit(-1);
		}

		Interface::instance()->Resize(1200, 800);
	
		return window;
	}

	void RunLoop (GLFWwindow* window)
	{
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window)) {
			/* Render here */
			Interface::instance()->Render();
		
			/* Swap front and back buffers */
			glfwSwapBuffers(window);
		
			/* Poll for and process events */
			glfwPollEvents();
		}
	
	}

	void Terminate ()
	{
		/* release BlendInt */
		Interface::release();
	
		glfwTerminate();
	}

	bool CheckAllocatedObjects ()
	{
#ifdef DEBUG
		int mapsize = AbstractWidget::map_size();

		if(mapsize > 0) {
			map<uint64_t, AbstractWidget*>::const_iterator it;
			for (it = AbstractWidget::get_map().begin(); it != AbstractWidget::get_map().end(); it++)
			{
				cout << "id: " << it->first << " was not deleted!" << endl;
			}
		}

		return (mapsize == 0);
#else
		return true;
#endif
	}

}
