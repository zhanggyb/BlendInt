/*
 * Application.cpp
 *
 *  Created on: 2013年7月3日
 *      Author: zhanggyb
 */

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>

#include <iostream>
#include <string>
#include <locale.h>
#include <stdlib.h>

#include <BIL/Application.h>
#include <BIL/FontManager.h>

using namespace std;

namespace BIL {

    string Application::glStrVersion;
    GLfloat Application::glVersion = 1.0;

    Application::Application ()
        : _mainWindow(NULL)
    {
        // TODO Auto-generated constructor stub

    }

    int Application::initialize (bool nls)
    {
        int ret = glfwInit();

        if (ret == GL_TRUE) glfwSetErrorCallback(&Application::cbError);

        FontManager::gFontService = FontManager::instance();
        bool fontinit = FontManager::gFontService->initialize();
        if(!fontinit) {
            cerr << "Cannot initialize font service" << endl;
            exit(1);
        }
        fontinit = FontManager::gFontService->loadFont();
        if(!fontinit) {
            cerr << "Cannot load default font into memory" << endl;
            exit(1);
        }

        // set locale
        setlocale(LC_ALL, "");

        return ret;
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

    // FIXME: run in bummblebee cause crash if the window is closed
    void Application::run (void)
    {
        if (_mainWindow == NULL) {
            cerr << "No Main Window" << endl;
            return;
        }

        _mainWindow->MakeContextCurrent();

        // check OpenGL version
        glStrVersion = (const char*) glGetString(GL_VERSION);
        glVersion = std::atof(glStrVersion.c_str());    // C++ 98
        // glVersion = std::stof (glStrVersion);        // C++ 11

        // Initialize GLEW
        glewExperimental = true; // Needed in core profile
        if (glewInit() != GLEW_OK) {
            cerr << "Failed to initilize GLEW" << endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
#ifdef DEBUG
        cerr << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;
#endif

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

        // TODO: the following lines will cause crash
        /*
          if (_mainWindow != NULL) {
          delete _mainWindow;
          _mainWindow = NULL;
          }
        */

        delete FontManager::gFontService;
        FontManager::gFontService = NULL;

        glfwTerminate();
    }

    void Application::cbError (int error, const char* description)
    {
        std::cerr << error << ' ' << description << std::endl;

        return;
    }

} /* namespace BIL */
