#include "Window.hpp"

// Load GLEW first
#ifdef __UNIX__
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <iostream>
#include <BlendInt/Interface.hpp>

//using BlendInt::Interface;
#include <BlendInt/AbstractWidget.hpp>

namespace BlendInt {

	static void CbError (int error, const char* description)
	{
		std::cerr << "Error: " << description
				  << " (error code: " << error << ")"
				  << std::endl;
	}

	static void CbWindowSize(GLFWwindow* window, int w, int h)
	{
		Interface::Instance()->Resize(w, h);
	}

	static void CbKey(GLFWwindow* window, int key, int scancode, int action,
					  int mods)
	{
		std::cout << "Get key number from GLFW: " << key << std::endl;
		std::cout << "Get scancode from GLFW: " << scancode << std::endl;

		Interface::Instance()->GLFWKeyEvent(key, scancode, action, mods);
	}

	static void CbMouseButton(GLFWwindow* window, int button, int action,
							  int mods) {
		Interface::Instance()->GLFWMouseButtonEvent(button, action, mods);
	}

	static void CbCursorPos(GLFWwindow* window, double xpos, double ypos) {
		Interface::Instance()->GLFWCursorPosEvent(xpos, ypos);
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
		if (!Interface::Initialize()) {
			glfwTerminate();
			exit(-1);
		}

		Interface::Instance()->Resize(1200, 800);
	
		return window;
	}

	void RunLoop (GLFWwindow* window)
	{
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window)) {
			/* Render here */
			Interface::Instance()->Draw();
		
			/* Swap front and back buffers */
			glfwSwapBuffers(window);
		
			/* Poll for and process events */
			glfwPollEvents();
		}
	
	}

	void Terminate ()
	{
		/* release BlendInt */
		Interface::Release();
	
		glfwTerminate();
	}

	bool CheckAllocatedObjects ()
	{
#ifdef DEBUG
		int mapsize = Object::GetMapSize();

		if(mapsize > 0) {
			map<uint64_t, Object*>::const_iterator it;
			for (it = Object::GetMap().begin(); it != Object::GetMap().end(); it++)
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
