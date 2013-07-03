/*
 * Window.h
 *
 *  Created on: 2013年7月3日
 *      Author: zhanggyb
 */

#ifndef _BIL_WINDOW_H_
#define _BIL_WINDOW_H_

#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <map>

#include <BIL/BasicObject.h>
#include <BIL/Size.h>

namespace BIL {

	class Window: public BIL::BasicObject
	{
	public:

		Window (BasicObject * parent = NULL);

		Window (int width, int height, const char *title, GLFWmonitor *monitor,
		        GLFWwindow *share, BasicObject* parent = NULL);

		void MakeContextCurrent (void);

		virtual ~Window ();

		GLFWwindow* getWindow (void) const
		{
			return _window;
		}

		Size getSize (void);

		bool resize (const Size& size);

		bool resize (int w, int h)
		{
			return (resize(Size(w, h)));
		}

		void setTitle (const std::string& title);
		void setTitle (const char *title);

		std::string getTitle (void)
		{
			return _title;
		}

		inline void refresh (void)
		{
			render();
		}

	protected:

		virtual void render (void);

	private:

		Window (const Window& orig);
		Window& operator = (const Window& orig);

		bool registerCallbacks (void * pointer);

		bool unregisterCallbacks (void * pointer);

		GLFWwindow *_window;

		std::string _title;

		static void CbKey (GLFWwindow* window, int key, int scancode,
		        int action, int mods);

		static void CbWindowSize (GLFWwindow* window, int w, int h);

		static void CbWindowPosition (GLFWwindow* window, int xpos, int ypos);

		static void CbMouseButton (GLFWwindow* window, int button, int action,
		        int mods);

		static void CbCursorPosition (GLFWwindow* window, double xpos,
		        double ypos);

		static void CbCursorEnter (GLFWwindow* window, int entered);

		/**
		 * A std::map container to record GLFWwindow and Window
		 */
		static std::map<GLFWwindow*, BIL::Window*> windowMap;
	};

} /* namespace BIL */
#endif /* _BIL_WINDOW_H_ */
