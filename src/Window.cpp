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
#include <BIL/ContextMenuEvent.h>

using namespace std;

namespace BIL {

	std::map<GLFWwindow*, Window*> Window::windowMap;

	Window::Window (Traceable *parent)
		: Traceable(parent), window_(NULL),
		  cursor_pos_x_(0.0), cursor_pos_y_(0.0),
		  size_(Vec2i(640, 480))
	{
		title_ = "Default";

		window_ = glfwCreateWindow(size_.vec.x,
								   size_.vec.y,
								   title_.data(),
								   NULL,
								   NULL);

		if (window_ == NULL)
			throw;

		registerCallbacks();

		// createDefaultLayout ();
	}

	Window::Window (int width, int height, const char* title,
	        GLFWmonitor* monitor, GLFWwindow* share, Traceable* parent)
		: Traceable(parent), window_(NULL),
		  cursor_pos_x_(0.0), cursor_pos_y_(0.0),
		  size_(Vec2i(width, height))
	{
		title_ = title;
		window_ = glfwCreateWindow(size_.vec.x,
								   size_.vec.y,
								   title,
								   monitor,
								   share);

		if (window_ == NULL)
			throw;

		registerCallbacks();

		// createDefaultLayout();
	}

	Window::~Window ()
	{
		unregisterCallbacks();

		deleteChildren();

		glfwDestroyWindow(window_);
	}

	bool Window::registerCallbacks (void)
	{
		windowMap[window_] = this;

		glfwSetWindowPosCallback(window_, &BIL::Window::cbWindowPosition);
		glfwSetWindowSizeCallback(window_, &BIL::Window::cbWindowSize);

		glfwSetKeyCallback(window_, &BIL::Window::cbKey);
		glfwSetCharCallback(window_, &BIL::Window::cbChar);
		glfwSetMouseButtonCallback(window_, &BIL::Window::cbMouseButton);
		glfwSetCursorPosCallback (window_, &BIL::Window::cbCursorPos);
		glfwSetCursorEnterCallback(window_, &BIL::Window::cbCursorEnter);

		// _scope.connect(this->keyEvent(), this, &Window::message);

		return true;
	}

	bool Window::unregisterCallbacks (void)
	{
		windowMap.erase(window_);

		return true;
	}

	void Window::render (void)
	{
		int width = size().vec.x;
		int height = size().vec.y;
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

	void Window::ResizeEvent (int width, int height)
	{
		// TODO: resize all widgets/layouts in children
		glfwGetWindowSize(window_, &size_.vec.x, &size_.vec.y);
	}

	void Window::KeyEvent (int key, int scancode, int action, int mods)
	{
		Event* event = NULL;

		if (key == Key_Menu) {
			event = new ContextMenuEvent(ContextMenuEvent::Keyboard,
										 static_cast<KeyModifier>(mods));
		} else {
			event = new BIL::KeyEvent(static_cast<Key>(key),
									  scancode,
									  static_cast<KeyButtonAction>(action),
									  static_cast<KeyModifier>(mods));
		}

		ChildrenList<Traceable*>::const_reverse_iterator it;
		Drawable *item = NULL;
		for (it = _children.rbegin(); it != _children.rend(); it++) {
			item = dynamic_cast<Drawable*>(*it);
			if (item == NULL) continue;

			// TODO: only the focused widget can dispose key event
			switch (action) {
			case Press:
				//item->KeyPressEvent(event);
				break;
			case Release:
				// item->KeyReleaseEvent(&event);
				break;
			case Repeat:
				// item->KeyRepeatEvent(&event);
				break;
			default:
				//item->KeyPressEvent(event);
				break;
			}
			if(event->IsAccepted()) break;
		}

		delete event;
	}

	void Window::InputMethodEvent (unsigned int character)
	{
		ChildrenList<Traceable*>::const_reverse_iterator it;
		Drawable *item = NULL;
		for (it = _children.rbegin(); it != _children.rend(); it++) {
			item = dynamic_cast<Drawable*>(*it);
			if (item != NULL) {
				item->InputMethodEvent(character);
			}
		}
	}

	void Window::MouseButtonEvent (int button, int action, int mods)
	{
		MouseButton mouseclick = ButtonNone;
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
		event.set_window_pos(cursor_pos_x_, cursor_pos_y_);

		ChildrenList<Traceable*>::const_reverse_iterator it;
		Drawable *item = NULL;
		float local_x;
		float local_y;
		for (it = _children.rbegin(); it != _children.rend(); it++) {
			item = dynamic_cast<Drawable*>(*it);
			if (item == NULL) continue;

			local_x = cursor_pos_x_ - (item->getPos().coord.x);
			local_y = cursor_pos_y_ - (item->getPos().coord.y);
			if ((local_x - 0.000001 > 0.0) &&
				(local_y - 0.000001 > 0.0) &&
				(local_x - item->getSize().vec.x) < 0.0 &&
				(local_y - item->getSize().vec.y) < 0.0)
			{
				event.set_pos (local_x, local_y);
				switch (action) {
				case GLFW_PRESS:
					item->MousePressEvent(&event);
					break;
				case GLFW_RELEASE:
					item->MouseReleaseEvent(&event);
				default:
					break;
				}
			}
			if(event.IsAccepted()) break;
		}
	}

	void Window::CursorPosEvent (double xpos, double ypos)
	{
		cursor_pos_x_ = xpos;
		cursor_pos_y_ = size_.vec.y - ypos;

		MouseButton mouseclick = ButtonNone;
		Event::Type type = Event::None;

		MouseEvent event (type, mouseclick);
		event.set_window_pos(cursor_pos_x_, cursor_pos_y_);

		ChildrenList<Traceable*>::const_reverse_iterator it;
		Drawable *item = NULL;
		float local_x;
		float local_y;

		for (it = _children.rbegin(); it != _children.rend(); it++) {
			item = dynamic_cast<Drawable*>(*it);
			if (item != NULL) {
				local_x = cursor_pos_x_ - (item->getPos().coord.x);
				local_y = cursor_pos_y_ - (item->getPos().coord.y);
				if ((local_x - 0.000001 > 0.0) &&
					(local_y - 0.000001 > 0.0) &&
					(local_x - item->getSize().vec.x) < 0.0 &&
					(local_y - item->getSize().vec.y) < 0.0)
				{
					event.set_pos (local_x, local_y);
					item->MouseMoveEvent(&event);
				}
				if(event.IsAccepted()) break;
			}
		}
	}

	void Window::CursorEnterEvent (int entered)
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
			winObj->KeyEvent(key, scancode, action, mods);
		}
	}

	void Window::cbChar (GLFWwindow* window, unsigned int character)
	{
		map<GLFWwindow*, BIL::Window*>::iterator it;
		it = windowMap.find(window);

		if(it != windowMap.end()) {
			BIL::Window* winObj = windowMap[window];
			winObj->InputMethodEvent(character);
		}
	}

	void Window::cbWindowSize (GLFWwindow* window, int w, int h)
	{
		map<GLFWwindow*, BIL::Window*>::iterator it;
		it = windowMap.find(window);

		if(it != windowMap.end()) {
			BIL::Window* winObj = windowMap[window];
			winObj->ResizeEvent(w, h);
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
			winObj->MouseButtonEvent(button, action, mods);
		}
	}

	void Window::cbCursorPos (GLFWwindow* window, double xpos, double ypos)
	{
		map<GLFWwindow*, BIL::Window*>::iterator it;
		it = windowMap.find(window);

		if(it != windowMap.end()) {
			BIL::Window* winObj = windowMap[window];
			winObj->CursorPosEvent(xpos, ypos);
		}
	}

	void Window::cbCursorEnter (GLFWwindow* window, int entered)
	{
		map<GLFWwindow*, BIL::Window*>::iterator it;
		it = windowMap.find(window);

		if(it != windowMap.end()) {
			BIL::Window* winObj = windowMap[window];
			winObj->CursorEnterEvent(entered);
		}
	}

} /* namespace BIL */
