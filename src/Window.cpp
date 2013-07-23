/*
 * Window.cpp
 *
 *  Created on: 2013年7月3日
 *      Author: zhanggyb
 */

#include <BIL/Window.h>

#include <BIL/Drawable.h>

namespace BIL {

    std::map<GLFWwindow*, Window*> Window::windowMap;

    Window::Window (BasicObject *parent)
        : BasicObject(parent), _window(NULL)
    {
        _title = "Default";

        _window = glfwCreateWindow(640, 480, _title.data(), NULL, NULL);

        if (_window == NULL)
            throw;

        registerCallbacks();

        // createDefaultLayout ();
    }

    Window::Window (int width, int height, const char* title,
                    GLFWmonitor* monitor, GLFWwindow* share, BasicObject* parent)
        : BasicObject(parent), _window(NULL)
    {
        _title = title;
        _window = glfwCreateWindow(width, height, title, monitor, share);

        if (_window == NULL)
            throw;

        registerCallbacks();

        // createDefaultLayout();
    }

    Window::~Window ()
    {
        unregisterCallbacks();

        deleteChildren();

        glfwDestroyWindow(_window);
    }

    Size2Di Window::getSize (void)
    {
        int w, h;

        glfwGetWindowSize(_window, &w, &h);

        return Size2Di(w, h);
    }

    bool Window::resize (const Size2Di& size)
    {
        return true;
    }

    void Window::setTitle (const std::string& title)
    {
        _title = title;
        glfwSetWindowTitle(_window, title.data());

        return;
    }

    void Window::setTitle (const char* title)
    {
        _title = title;
        glfwSetWindowTitle(_window, title);

        return;
    }

    bool Window::registerCallbacks (void)
    {
        windowMap[_window] = this;

        glfwSetKeyCallback(_window, &BIL::Window::cbKey);
        glfwSetWindowPosCallback(_window, &BIL::Window::cbWindowPosition);
        glfwSetWindowSizeCallback(_window, &BIL::Window::cbWindowSize);

        // _scope.connect(this->keyEvent(), this, &Window::message);

        return true;
    }

    bool Window::unregisterCallbacks (void)
    {
        windowMap.erase(_window);

        return true;
    }

    void Window::render (void)
    {
        int width = getSize().getWidth();
        int height = getSize().getHeight();
        // float ratio = width / (float) height;

        glClearColor(0.40,0.40,0.45,1.00);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor4f(1.00,1.00,1.00,1.00);

        // enable anti-alias
        /*
          glEnable(GL_BLEND);
          glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          glEnable (GL_POINT_SMOOTH);
          glEnable (GL_LINE_SMOOTH);
          glEnable (GL_POLYGON_SMOOTH);
        */

        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.f, (float) width, 0.f, (float) height, 100.f, -100.f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        ChildrenList<BasicObject*>::iterator it;
        Drawable *item = NULL;
        for (it = _children.begin(); it != _children.end(); it++) {
            item = dynamic_cast<Drawable*>(*it);
            if (item != NULL) {
                item->refresh();
            }
        }
        // if (_mainLayout != NULL) {
        //_mainLayout->Refresh();
        //}
    }

    void Window::cbKey (GLFWwindow* window, int key, int scancode, int action,
                        int mods)
    {
    }

    void Window::cbWindowSize (GLFWwindow* window, int w, int h)
    {
    }

    void Window::cbWindowPosition (GLFWwindow* window, int xpos, int ypos)
    {
    }

    void Window::cbMouseButton (GLFWwindow* window, int button, int action,
                                int mods)
    {
    }

    void Window::cbCursorPosition (GLFWwindow* window, double xpos, double ypos)
    {
    }

    void Window::cbCursorEnter (GLFWwindow* window, int entered)
    {
    }

} /* namespace BIL */
