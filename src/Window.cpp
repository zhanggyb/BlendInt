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

#include <BIL/Window.hpp>
#include <BIL/Drawable.hpp>
#include <BIL/KeyEvent.hpp>
#include <BIL/MouseEvent.hpp>
#include <BIL/ContextMenuEvent.hpp>

using namespace std;

namespace BIL {

	std::map<GLFWwindow*, Window*> Window::windowMap;

	Window::Window (Traceable *parent)
		: Traceable(parent), window_(NULL),
		  cursor_pos_x_(0.0), cursor_pos_y_(0.0),
		  size_(640, 480)
	{
		title_ = "Default";

		window_ = glfwCreateWindow(size_.width(),
								   size_.height(),
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
		  size_(width, height)
	{
		title_ = title;
		window_ = glfwCreateWindow(size_.width(),
								   size_.height(),
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

#ifdef DEBUG
	void Window::drawGrid (int width, int height)
	{
		// Draw grid for debug
		const int small_step = 20;
		const int big_step = 100;

		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glColor4f (1.0f, 1.0f, 1.0f, 0.1f);
		glLineStipple (1, 0xAAAA);
		for (int num = 1; num < width; num++)
		{
			int step = num * small_step;
			glBegin (GL_LINES);
				glVertex2i(0, step);
				glVertex2i(width, step);
			glEnd();

		}
		for (int num = 1; num < height; num++)
		{
			int step = num * small_step;
			glBegin (GL_LINES);
				glVertex2i(step, 0);
				glVertex2i(step, height);
			glEnd();
		}

		glColor4f (1.0f, 1.0f, 1.0f, 0.25f);
		glLineStipple (1, 0xAAAA);
		for (int num = 1; num < width; num++)
		{
			int step = num * big_step;
			glBegin (GL_LINES);
			glVertex2i(0, step);
			glVertex2i(width, step);
			glEnd();
		}

		for (int num = 1; num < height; num++)
		{
			int step = num * big_step;
			glBegin (GL_LINES);
				glVertex2i(step, 0);
				glVertex2i(step, height);
			glEnd();
		}

		glDisable(GL_LINE_STIPPLE);

	}
#endif

	void Window::render ()
	{
		int width = size_.width();
		int height = size_.height();
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

#ifdef DEBUG
		drawGrid (width, height);
#endif

		list<Traceable*>::const_iterator it;
		Drawable *item = NULL;
		for (it = children_.begin(); it != children_.end(); it++) {
			item = dynamic_cast<Drawable*>(*it);
			if (item) render(item);
		}

		list<Traceable*>::const_iterator j;
		for (j = Traceable::getList()->begin(); j != Traceable::getList()->end(); j++)
		{
			item = dynamic_cast<Drawable*>(*j);
			if (item)	render(item);
		}

		glDisable(GL_BLEND);
		// if (_mainLayout != NULL) {
		//_mainLayout->Refresh();
		//}
	}

	void Window::render (Drawable* obj)
	{
		list<Traceable*>::const_iterator it;
		Drawable *item = NULL;
		for (it = obj->children().begin(); it != obj->children().end(); it++) {
			item = dynamic_cast<Drawable*>(*it);
			if (item) {
				render (item);
			}
		}

		obj->render();
	}

	void Window::resizeEvent (int width, int height)
	{
		// TODO: resize all widgets/layouts in children
		size_.set_width(width);
		size_.set_height(height);
	}

	void Window::keyEvent (int key, int scancode, int action, int mods)
	{
		if (key == Key_Menu) {
			ContextMenuEvent event(ContextMenuEvent::Keyboard,
								   mods);

			list<Traceable*>::const_reverse_iterator it;
			Drawable *item = NULL;
			for (it = children_.rbegin(); it != children_.rend(); it++) {
				item = dynamic_cast<Drawable*>(*it);
				if (item == NULL) continue;

				// TODO: only the focused widget can dispose key event
				switch (action) {
				case KeyPress:
					item->contextMenuPressEvent (&event);
					break;
				case KeyRelease:
					item->contextMenuReleaseEvent (&event);
					break;
				default:
					break;
				}
				if(event.IsAccepted()) break;
			}

		} else {

			KeyEvent event(key, scancode, action, mods);

			list<Traceable*>::const_reverse_iterator it;
			Drawable *item = NULL;
			for (it = children_.rbegin(); it != children_.rend(); it++) {
				item = dynamic_cast<Drawable*>(*it);
				if (item == NULL) continue;

				// TODO: only the focused widget can dispose key event
				switch (action) {
				case KeyPress:
					disposeKeyPressEvent(item, &event);
					break;
				case KeyRelease:
					// item->KeyReleaseEvent(dynamic_cast<BIL::KeyEvent*>(event));
					break;
				case KeyRepeat:
					// item->KeyRepeatEvent(&event);
					break;
				default:
					break;
				}
				if(event.IsAccepted()) break;
			}

		}
	}

	void Window::inputMethodEvent (unsigned int character)
	{
		list<Traceable*>::const_reverse_iterator it;
		Drawable *item = NULL;
		for (it = children_.rbegin(); it != children_.rend(); it++) {
			item = dynamic_cast<Drawable*>(*it);
			if (item != NULL) {
				// item->inputMethodEvent(character);
			}
		}
	}

	void Window::mouseButtonEvent (int button, int action, int mods)
	{
		MouseButton mouseclick = MouseButtonNone;
		switch (button) {
		case GLFW_MOUSE_BUTTON_1:
			mouseclick = MouseButtonLeft;
			break;
		case GLFW_MOUSE_BUTTON_2:
			mouseclick = MouseButtonRight;
			break;
		case GLFW_MOUSE_BUTTON_3:
			mouseclick = MouseButtonMiddle;
			break;
		case GLFW_MOUSE_BUTTON_4:
			mouseclick = MouseButtonScrollUp;
			break;
		case GLFW_MOUSE_BUTTON_5:
			mouseclick = MouseButtonScrollDown;
			break;
		default:
			break;
		}

	    MouseAction mouse_action = MouseNone;
		switch (action) {
		case GLFW_PRESS:
			mouse_action = MousePress;
			break;
		case GLFW_RELEASE:
			mouse_action = MouseRelease;
			break;
		default:
			break;
		}

		MouseEvent event (mouse_action, mouseclick);
		event.set_window_pos(cursor_pos_x_, cursor_pos_y_);

		list<Traceable*>::const_reverse_iterator it;
		Drawable *item = NULL;
		float local_x;
		float local_y;
		for (it = children_.rbegin(); it != children_.rend(); it++) {
			item = dynamic_cast<Drawable*>(*it);
			if (item == NULL) continue;

			local_x = cursor_pos_x_ - (item->pos().x());
			local_y = cursor_pos_y_ - (item->pos().y());
			if ((local_x - 0.000001 > 0.0) &&
				(local_y - 0.000001 > 0.0) &&
				(local_x - item->size().width()) < 0.0 &&
				(local_y - item->size().height()) < 0.0)
			{
				event.set_pos (local_x, local_y);
				switch (action) {
				case GLFW_PRESS:
					disposeMousePressEvent(item, &event);
					break;
				case GLFW_RELEASE:
					item->mouseReleaseEvent(&event);
					break;
				default:
					break;
				}
			}
			if(event.IsAccepted()) break;
		}
	}

	void Window::cursorPosEvent (double xpos, double ypos)
	{
		cursor_pos_x_ = xpos;
		cursor_pos_y_ = size_.height() - ypos;

		MouseEvent event (MouseNone, MouseButtonNone);
		event.set_window_pos(cursor_pos_x_, cursor_pos_y_);

		list<Traceable*>::const_reverse_iterator it;
		Drawable *item = NULL;
		float local_x;
		float local_y;

		for (it = children_.rbegin(); it != children_.rend(); it++) {
			item = dynamic_cast<Drawable*>(*it);
			if (item != NULL) {
				local_x = cursor_pos_x_ - (item->pos().x());
				local_y = cursor_pos_y_ - (item->pos().y());
				event.set_pos (local_x, local_y);
				disposeMouseMoveEvent(item, &event);
				if(event.IsAccepted()) break;
			}
		}
	}

	void Window::cursorEnterEvent (int entered)
	{
		if (entered == GL_TRUE) {
			//cout << "Cursor entered." << endl;
		}
	}

	void Window::disposeKeyPressEvent(Drawable* obj, KeyEvent* event)
	{
		obj->keyPressEvent(event);

		if (event->IsAccepted()) {
			return;
		} else {
			list<Traceable*>::const_reverse_iterator it;
			Drawable *item = NULL;
			for (it = obj->children().rbegin(); it != obj->children().rend(); it++) {
				item = dynamic_cast<Drawable*>(*it);
				if(item) disposeKeyPressEvent(item, event);
				if(event->IsAccepted()) return;
			}
		}
	}

	void Window::disposeMousePressEvent(Drawable* obj, MouseEvent* event)
	{
		obj->mousePressEvent(event);

		if (event->IsAccepted()) {
			return;
		} else {
			list<Traceable*>::const_reverse_iterator it;
			Drawable *item = NULL;
			for (it = obj->children().rbegin(); it != obj->children().rend(); it++) {
				item = dynamic_cast<Drawable*>(*it);
				if(item) disposeMousePressEvent(item, event);
				if(event->IsAccepted()) return;
			}
		}
	}

	void Window::disposeMouseMoveEvent(Drawable* obj, MouseEvent* event)
	{
		obj->mouseMoveEvent(event);

		if (event->IsAccepted()) {
			return;
		} else {
			list<Traceable*>::const_reverse_iterator it;
			Drawable *item = NULL;
			for (it = obj->children().rbegin(); it != obj->children().rend(); it++) {
				item = dynamic_cast<Drawable*>(*it);
				if(item) disposeMouseMoveEvent(item, event);
				if(event->IsAccepted()) return;
			}
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
			winObj->inputMethodEvent(character);
		}
	}

	void Window::cbWindowSize (GLFWwindow* window, int w, int h)
	{
		map<GLFWwindow*, BIL::Window*>::iterator it;
		it = windowMap.find(window);

		if(it != windowMap.end()) {
			BIL::Window* winObj = windowMap[window];
			winObj->resizeEvent(w, h);
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
