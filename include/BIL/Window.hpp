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

#ifndef _BIL_WINDOW_H_
#define _BIL_WINDOW_H_

#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <map>

#include <BIL/Traceable.hpp>
#include <BIL/Drawable.hpp>
#include <BIL/Tuple.hpp>
#include <BIL/Coord.hpp>
#include <BIL/Size.hpp>

using namespace std;

namespace BIL {

	class Window: public BIL::Traceable
	{
	public:

		enum InputMode {
			CURSOR = GLFW_CURSOR,
			STICKY_KEYS = GLFW_STICKY_KEYS,
			STICKY_MOUSE_BUTTONS = GLFW_STICKY_MOUSE_BUTTONS
		};

		/**
		 * @brief Default Constructor
		 * @param parent parent object
		 *
		 * @warning: any OpenGL code to initialize something  will fail before
		 * makeContextCurrent in constructor
		 */
		Window (Traceable * parent = NULL);

		Window (int width, int height, const char *title, GLFWmonitor *monitor,
		        GLFWwindow *share, Traceable* parent = NULL);

		void makeContextCurrent ()
		{
			glfwMakeContextCurrent(window_);
		}

		virtual ~Window ();

		GLFWwindow* window () const
		{
			return window_;
		}

		const Size& size () const
		{
			return size_;
		}

		void set_title (const std::string& title)
		{
			title_ = title;
			glfwSetWindowTitle(window_, title.data());

			return;
		}

		void set_title (const char* title)
		{
			title_ = title;
			glfwSetWindowTitle(window_, title);

			return;
		}

		void resize (const Size& size)
		{
			resize (size.width(), size.height());
		}

		void resize (int w, int h)
		{
			glfwSetWindowSize (window_, w, h);

			int width, height;
			glfwGetWindowSize(window_, &width, &height);
			size_.set_width(static_cast<unsigned int>(width));
			size_.set_height(static_cast<unsigned int>(height));
		}

		void setTitle (const std::string& title);
		void setTitle (const char *title);

		std::string getTitle (void)
		{
			return title_;
		}

		/**
		 * @brief Returns the value of an input option for the window
		 * @param[in] mode
		 * @return
		 */
		int getInputMode (InputMode mode)
		{
			return glfwGetInputMode (window_, mode);
		}

		/**
		 * @brief Sets an input option for the window
		 * @param mode
		 * @param value
		 */
		void setInputMode (InputMode mode, int value)
		{
			glfwSetInputMode (window_, mode, value);
		}

		int getKey (int key)
		{
			return glfwGetKey(window_, key);
		}

		int getMouseButton (int button)
		{
			return glfwGetMouseButton(window_, button);
		}

		Coord2d getCursorPos ()
		{
			Coord2d ret;
			glfwGetCursorPos (window_, &(ret.coord.x), &(ret.coord.y));

			return ret;
		}

		void setCursorPos (double xpos, double ypos)
		{
			glfwSetCursorPos(window_, xpos, ypos);
		}

		virtual void render ();

	protected:

		// TODO: remove virtual
		virtual void resizeEvent (int width, int height);

		virtual void keyEvent (int key, int scancode, int action, int mods);

		virtual void inputMethodEvent (unsigned int character);

		virtual void mouseButtonEvent (int button, int action, int mods);

		virtual void cursorPosEvent (double xpos, double ypos);

		virtual void cursorEnterEvent (int entered);

	private:					/* member functions */

		bool registerCallbacks (void);

		bool unregisterCallbacks (void);

		void render (Drawable* obj);

		void disposeKeyPressEvent (Drawable* obj, KeyEvent* event);

		void disposeMousePressEvent (Drawable* obj, MouseEvent* event);

		void disposeMouseReleaseEvent (Drawable* obj, MouseEvent* event);

		void disposeMouseMoveEvent (Drawable* obj, MouseEvent* event);

#ifdef DEBUG
		void drawGrid (int width, int height);
#endif

	private:					/* member variables */
		
		GLFWwindow *window_;

		double cursor_pos_x_;				/* cursor x position */
		double cursor_pos_y_;				/* cursor y position */

		Size size_;			/* window size */

		std::string title_;

	private:
		// static member functions

		static void cbKey (GLFWwindow* window, int key, int scancode,
		        int action, int mods);

		static void cbChar (GLFWwindow* window, unsigned int character);

		static void cbWindowSize (GLFWwindow* window, int w, int h);

		static void cbWindowPosition (GLFWwindow* window, int xpos, int ypos);

		static void cbMouseButton (GLFWwindow* window, int button, int action,
		        int mods);

		static void cbCursorPos (GLFWwindow* window, double xpos,
		        double ypos);

		static void cbCursorEnter (GLFWwindow* window, int entered);

		/**
		 * A std::map container to record GLFWwindow and Window
		 */
		static std::map<GLFWwindow*, BIL::Window*> windowMap;

	private:					/* member functions disabled */

		DISALLOW_COPY_AND_ASSIGN(Window);
	};

} /* namespace BIL */
#endif /* _BIL_WINDOW_H_ */
