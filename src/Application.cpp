/*
 * Application.cpp
 *
 *  Created on: 2013年7月3日
 *      Author: zhanggyb
 */

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>

#include <iostream>

#include <BIL/Application.h>

// Include GLM
#include <glm/glm.hpp>

namespace BIL {

	Application::Application ()
			: _mainWindow(NULL)
	{
		// TODO Auto-generated constructor stub

	}

	int Application::init (void)
	{
		int ret = glfwInit();

		if (ret == GL_TRUE)
			glfwSetErrorCallback(&Application::cbError);

		return ret;
	}

	Version Application::getVersion (void)
	{
		Version version;

		glfwGetVersion(&version.major, &version.minor, &version.rev);

		return version;
	}

	std::string Application::getVersionString (void)
	{
		std::string version;

		version = glfwGetVersionString();

		return version;
	}

	void Application::run (void)
	{
		if (_mainWindow == NULL) {
			std::cerr << "No Main Window" << std::endl;
			return;
		}

		_mainWindow->MakeContextCurrent();

		// Initialize GLEW
		glewExperimental = true; // Needed in core profile
		if (glewInit() != GLEW_OK) {
			std::cerr << "Failed to initilize GLEW" << std::endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		while (!glfwWindowShouldClose(_mainWindow->getWindow())) {

			_mainWindow->refresh();

			glfwSwapBuffers(_mainWindow->getWindow());
			glfwPollEvents();
		}

		return;
	}

	Application::~Application ()
	{
		// TODO Auto-generated destructor stub
		glfwTerminate();
	}

	void Application::cbError (int error, const char* description)
	{
		std::cerr << error << ' ' << description << std::endl;

		return;
	}

} /* namespace BIL */
