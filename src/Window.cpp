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
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>
#include <GL/gl.h>

#include <BIL/Window.h>
#include <BIL/Drawable.h>
#include <BIL/KeyEvent.h>
#include <BIL/MouseEvent.h>

using namespace std;

namespace BIL {

	std::map<GLFWwindow*, Window*> Window::windowMap;

	Window::Window (Traceable *parent)
		: Traceable(parent), _window(NULL), _cursorPosX(0.0), _cursorPosY(0.0)
	{
		_title = "Default";

		_window = glfwCreateWindow(640, 480, _title.data(), NULL, NULL);

		if (_window == NULL)
			throw;

		registerCallbacks();

		// createDefaultLayout ();
	}

	Window::Window (int width, int height, const char* title,
	        GLFWmonitor* monitor, GLFWwindow* share, Traceable* parent)
		: Traceable(parent), _window(NULL), _cursorPosX(0.0), _cursorPosY(0.0)
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

	Vec2i Window::getSize (void)
	{
		int w, h;

		glfwGetWindowSize(_window, &w, &h);

		return Vec2i(w, h);
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

		glfwSetWindowPosCallback(_window, &BIL::Window::cbWindowPosition);
		glfwSetWindowSizeCallback(_window, &BIL::Window::cbWindowSize);

		glfwSetKeyCallback(_window, &BIL::Window::cbKey);
		glfwSetCharCallback(_window, &BIL::Window::cbChar);
		glfwSetMouseButtonCallback(_window, &BIL::Window::cbMouseButton);
		glfwSetCursorPosCallback (_window, &BIL::Window::cbCursorPos);
		glfwSetCursorEnterCallback(_window, &BIL::Window::cbCursorEnter);

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
		int width = getSize().vec.x;
		int height = getSize().vec.y;
		// float ratio = width / (float) height;

		// float bg = 114.0 / 255;	// the default blender background color
		glClearColor(0.447, 0.447, 0.447, 1.00);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glColor4f(1.00, 1.00, 1.00, 1.00);

		// enable anti-alias
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		//glEnable (GL_POINT_SMOOTH);
		//glEnable (GL_LINE_SMOOTH);
		//glEnable (GL_POLYGON_SMOOTH);

		glViewport(0, 0, width, height);
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.f, (float) width, 0.f, (float) height, 100.f, -100.f);

		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity();

		ChildrenList<Traceable*>::const_iterator it;
		Drawable *item = NULL;
		for (it = _children.begin(); it != _children.end(); it++) {
			item = dynamic_cast<Drawable*>(*it);
			if (item != NULL) {
				item->render();
			}
		}

		list<Traceable*>::const_iterator j;
		for (j = Traceable::getList()->begin(); j != Traceable::getList()->end(); j++)
		{
			item = dynamic_cast<Drawable*>(*j);
			if (item != NULL) {
				item->render();
			}
		}

		glDisable(GL_BLEND);
		// if (_mainLayout != NULL) {
		//_mainLayout->Refresh();
		//}
	}

	void Window::keyEvent (int key, int scancode, int action, int mods)
	{
		KeyEvent event((Key)key,
					   scancode,
					   (KeyButtonAction)action,
					   (KeyModifier)mods);

		ChildrenList<Traceable*>::const_reverse_iterator it;
		Drawable *item = NULL;
		for (it = _children.rbegin(); it != _children.rend(); it++) {
			item = dynamic_cast<Drawable*>(*it);
			if (item != NULL) {
				// TODO: only the focused widget can dispose key event
				item->keyEvent(&event);
				if(event.isAccepted()) break;
			}
		}
	}

	void Window::charEvent (unsigned int character)
	{
		ChildrenList<Traceable*>::const_reverse_iterator it;
		Drawable *item = NULL;
		for (it = _children.rbegin(); it != _children.rend(); it++) {
			item = dynamic_cast<Drawable*>(*it);
			if (item != NULL) {
				item->charEvent(character);
			}
		}
	}

	void Window::mouseButtonEvent (int button, int action, int mods)
	{
		MouseButton mouseclick = ButtonLeft;
		switch (button) {
		case GLFW_MOUSE_BUTTON_1:
			mouseclick = ButtonLeft;
			break;
		case GLFW_MOUSE_BUTTON_2:
			mouseclick = ButtonRight;
			break;
		case GLFW_MOUSE_BUTTON_3:
			mouseclick = ButtonMiddle;
			break;
		case GLFW_MOUSE_BUTTON_4:
			mouseclick = ButtonScrollUp;
			break;
		case GLFW_MOUSE_BUTTON_5:
			mouseclick = ButtonScrollDown;
			break;
		default:
			break;
		}

		Event::Type type = Event::None;
		switch (action) {
		case GLFW_PRESS:
			type = Event::MousePress;
			break;
		case GLFW_RELEASE:
			type = Event::MouseRelease;
			break;
		default:
			break;
		}

		MouseEvent event (type, mouseclick);
		event.setWindowPos(_cursorPosX, _cursorPosY);

		ChildrenList<Traceable*>::const_reverse_iterator it;
		Drawable *item = NULL;
		for (it = _children.rbegin(); it != _children.rend(); it++) {
			item = dynamic_cast<Drawable*>(*it);
			if (item != NULL) {
				float x = _cursorPosX - (item->getPos().coord.x);
				float y = _cursorPosY - (item->getPos().coord.y);
				if (x < item->getSize().vec.x &&
					y < item->getSize().vec.y)
				{
					event.setLocalPos (x, y);
					item->mouseButtonEvent(button, action, mods);
				}
				if(event.isAccepted()) break;
			}
		}
	}

	void Window::cursorPosEvent (double xpos, double ypos)
	{
		_cursorPosX = xpos;
		_cursorPosY = ypos;

		ChildrenList<Traceable*>::const_reverse_iterator it;
		Drawable *item = NULL;
		for (it = _children.rbegin(); it != _children.rend(); it++) {
			item = dynamic_cast<Drawable*>(*it);
			if (item != NULL) {
				item->cursorPosEvent(xpos, ypos);
			}
		}
	}

	void Window::cursorEnterEvent (int entered)
	{
		if (entered == GL_TRUE) {
			cout << "Cursor entered." << endl;
		}
	}

	void Window::cbKey (GLFWwindow* window, int key, int scancode, int action,
	        int mods)
	{
		map<GLFWwindow*, BIL::Window*>::iterator it;
		it = windowMap.find(window);

		if(it != windowMap.end()) {
			BIL::Window* winObj = windowMap[window];
			winObj->keyEvent(key, scancode, action, mods);
		}
	}

	void Window::cbChar (GLFWwindow* window, unsigned int character)
	{
		map<GLFWwindow*, BIL::Window*>::iterator it;
		it = windowMap.find(window);

		if(it != windowMap.end()) {
			BIL::Window* winObj = windowMap[window];
			winObj->charEvent(character);
		}
	}

	void Window::cbWindowSize (GLFWwindow* window, int w, int h)
	{
		map<GLFWwindow*, BIL::Window*>::iterator it;
		it = windowMap.find(window);

		if(it != windowMap.end()) {
			// TODO: resize all widgets/layouts in children
		}
	}

	void Window::cbWindowPosition (GLFWwindow* window, int xpos, int ypos)
	{
	}

	void Window::cbMouseButton (GLFWwindow* window, int button, int action,
	        int mods)
	{
		map<GLFWwindow*, BIL::Window*>::iterator it;
		it = windowMap.find(window);

		if(it != windowMap.end()) {
			BIL::Window* winObj = windowMap[window];
			winObj->mouseButtonEvent(button, action, mods);
		}
	}

	void Window::cbCursorPos (GLFWwindow* window, double xpos, double ypos)
	{
		map<GLFWwindow*, BIL::Window*>::iterator it;
		it = windowMap.find(window);

		if(it != windowMap.end()) {
			BIL::Window* winObj = windowMap[window];
			winObj->cursorPosEvent(xpos, ypos);
		}
	}

	void Window::cbCursorEnter (GLFWwindow* window, int entered)
	{
		map<GLFWwindow*, BIL::Window*>::iterator it;
		it = windowMap.find(window);

		if(it != windowMap.end()) {
			BIL::Window* winObj = windowMap[window];
			winObj->cursorEnterEvent(entered);
		}
	}

} /* namespace BIL */
