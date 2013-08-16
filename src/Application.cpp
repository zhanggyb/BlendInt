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

#include <GL/gl.h>

#include <iostream>
#include <string>
#include <cstdlib>

#include <locale.h>

#include <BIL/Application.hpp>
#include <BIL/FontConfig.hpp>
#include <BIL/Window.hpp>

using namespace std;

namespace BIL {

	string Application::glStrVersion;
	GLfloat Application::glVersion = 1.0;

	Application::Application ()
		: _mainWindow(NULL)
	{
		// TODO Auto-generated constructor stub
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

		FontConfig* ftconfig = FontConfig::getService();
		bool fontinit = ftconfig->initialize();
		if (!fontinit) {
			cerr << "Cannot initialize font service" << endl;
			return false;
		}

		fontinit = ftconfig->loadDefaultFontToMem();
		if (!fontinit) {
			cerr << "Cannot load default font into memory" << endl;
			return false;
		}

		if (nls) {
			// set locale
			setlocale(LC_ALL, "");
		}

		if (_mainWindow != NULL) {
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
		// Initialize GLEW
		glewExperimental = true; // Needed in core profile
		if (glewInit() != GLEW_OK) {
			cerr << "Failed to initilize GLEW" << endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

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

		FontConfig::release();

		glfwTerminate();
	}

	void Application::cbError (int error, const char* description)
	{
		std::cerr << error << ' ' << description << std::endl;

		return;
	}

} /* namespace BIL */
