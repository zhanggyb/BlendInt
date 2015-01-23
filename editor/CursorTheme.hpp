/**
 * CursorTheme.hpp
 */

#pragma once

#include <GLFW/glfw3.h>

#include <BlendInt/Gui/AbstractCursorTheme.hpp>

namespace BlendInt {

	class CursorTheme: public AbstractCursorTheme {
	public:

		CursorTheme (GLFWwindow* window) :
				AbstractCursorTheme(), window_(window), arrow_(nullptr), cross_(nullptr), split_v_(
				        nullptr), split_h_(nullptr), top_left_corner_(nullptr), top_right_corner_(
				        nullptr) {
			Initialize();
		}

		virtual ~CursorTheme () {
			Release();
		}

		virtual void SetCursor (int cursor_type);

	private:

		void Initialize ();

		void Release ();

		GLFWwindow* window_;

		GLFWcursor* arrow_;

		GLFWcursor* cross_;

		GLFWcursor* split_v_;

		GLFWcursor* split_h_;

		GLFWcursor* top_left_corner_;

		GLFWcursor* top_right_corner_;
	};

}
