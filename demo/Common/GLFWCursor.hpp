/**
 * GLFWCursor.hpp
 */

#ifndef _GLFW_CURSOR_HPP_
#define _GLFW_CURSOR_HPP_

#include <GLFW/glfw3.h>

#include <BlendInt/Gui/CursorType.hpp>

namespace BlendInt {

class GLFWCursor: public CursorType
{
public:

	GLFWCursor (GLFWwindow* window)
	: CursorType(),
	window_(window),
	arrow_(nullptr),
	cross_(nullptr),
	split_v_(nullptr),
	split_h_(nullptr),
	top_left_corner_(nullptr),
	top_right_corner_(nullptr)
	{
		Initialize();
	}

	virtual ~GLFWCursor ()
	{
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

#endif	// _GLFW_CURSOR_HPP_
