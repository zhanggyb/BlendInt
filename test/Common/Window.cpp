#include "Window.hpp"

#include <iostream>

//using BlendInt::Interface;

namespace BlendInt {

static void CbError (int error, const char* description)
{
	std::cerr << "Error: " << description
		<< " (error code: " << error << ")"
		<< std::endl;
}

static void CbWindowSize(GLFWwindow* window, int w, int h) {
	Interface::instance()->resizeEvent(w, h);
}

static void CbKey(GLFWwindow* window, int key, int scancode, int action,
		int mods) {
	Interface::instance()->keyEvent(key, scancode, action, mods);
}

static void CbMouseButton(GLFWwindow* window, int button, int action,
		int mods) {
	Interface::instance()->mouseButtonEvent(button, action, mods);
}

static void CbCursorPos(GLFWwindow* window, double xpos, double ypos) {
	Interface::instance()->cursorPosEvent(xpos, ypos);
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

}
