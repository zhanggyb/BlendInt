/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.	If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>

#include <iostream>
#include <string>
#include <cstdlib>

#include <locale.h>

#include <Cpp/Events.hpp>

#include <BIL/Application.hpp>
#include <BIL/FontConfig.hpp>
#include <BIL/FontCache.hpp>
#include <BIL/Theme.hpp>
#include <BIL/Window.hpp>

using namespace std;

namespace BIL {

	string Application::glStrVersion;
	GLfloat Application::glVersion = 1.0;

	static void error_callback (int error, const char* description)
	{
		cerr << "Error: " << description << " (error code: " << error << ")" << endl;
	}

	void initialize (bool nls)
	{
		int ret = glfwInit();

		if (ret == GL_TRUE) {
			glfwSetErrorCallback(&error_callback);
		} else {
			cerr << "Cannot initialize GLFW" << endl;
			exit(EXIT_FAILURE);
		}

		if (nls) {
			setlocale(LC_ALL, "");
		}
	}

	void run (Window* window)
	{
		GLFWwindow* glfw_window = window->window();
		while (!glfwWindowShouldClose(glfw_window)) {

			window->render();

			glfwSwapBuffers(glfw_window);
			glfwPollEvents();
		}
	}

	void terminate ()
	{
		std::map<GLFWwindow*, BIL::Window*>::iterator it;
		for (it = Window::getWindowMap().begin(); it != Window::getWindowMap().end(); it++)
		{
			glfwDestroyWindow (it->first);
		}

		Window::getWindowMap().clear ();

		glfwTerminate();

		//FontConfig::release();
		//FontCache::releaseAll();
		//Theme::release();

		// Traceable::clearSoloList();
	}

	Application::Application ()
		: _mainWindow(NULL)
	{
		int ret = glfwInit();

		if (ret == GL_TRUE) {
			glfwSetErrorCallback(&Application::cbError);
		} else {
			cerr << "Cannot initialize GLFW" << endl;
			exit(EXIT_FAILURE);
		}
	}

	bool Application::initialize (bool nls)
	{
		if (!FontConfig::instance())
			return false;

		if (!FontConfig::initialize()) {
			std::cerr << "Cannot initialize FontConfig" << std::endl;
			return false;
		} else {
			FontConfig* ftconfig = FontConfig::instance();
			if (!ftconfig->loadDefaultFontToMem()) {
				cerr << "Cannot load default font into memory" << endl;
				return false;
			}
		}

		Theme* theme = Theme::instance();
		if (theme)
			theme->initialize();

		if (nls) {
			setlocale(LC_ALL, "");
		}

		// Initialize GLEW
		glewExperimental = true; // Needed in core profile
		if (glewInit() != GLEW_OK) {
			cerr << "Failed to initilize GLEW" << endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		if (_mainWindow) {
			_mainWindow->makeContextCurrent();
		}

		return true;
	}

	GLFWVersion Application::getVersion (void)
	{
		GLFWVersion version;

		glfwGetVersion(&version.major, &version.minor, &version.rev);

		return version;
	}

	string Application::getVersionString (void)
	{
		string version;

		version = glfwGetVersionString();

		return version;
	}

	void Application::setMainWindow (Window* window)
	{
		_mainWindow = window;
		_mainWindow->makeContextCurrent();
	}

	// FIXME: run in bummblebee cause crash if the window is closed
	void Application::run (void)
	{
		// check OpenGL version
		glStrVersion = (const char*) glGetString(GL_VERSION);
		glVersion = std::atof(glStrVersion.c_str());	// C++ 98
		// glVersion = std::stof (glStrVersion);		// C++ 11

		GLFWwindow* window = _mainWindow->window();
		while (!glfwWindowShouldClose(window)) {

			_mainWindow->render();

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		return;
	}

	Application::~Application ()
	{
		// TODO: the following lines will cause crash
		/*
		  if (_mainWindow != NULL) {
		  delete _mainWindow;
		  _mainWindow = NULL;
		  }
		*/

		Traceable::clearSoloList();

		FontConfig::release();
		FontCache::releaseAll();
		Theme::release();

		glfwTerminate();
	}

	void Application::cbError (int error, const char* description)
	{
		std::cerr << error << ' ' << description << std::endl;

		return;
	}

} /* namespace BIL */
