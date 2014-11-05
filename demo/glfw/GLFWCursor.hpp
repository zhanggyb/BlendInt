/**
 * GLFWCursor.hpp
 */

#ifndef _GLFW_CURSOR_HPP_
#define _GLFW_CURSOR_HPP_

#include <GLFW/glfw3.h>

#include <BlendInt/Gui/CursorType.hpp>

namespace BI=BlendInt;

class GLFWCursor: public BI::CursorType
{
public:

	GLFWCursor (GLFWwindow* window)
	: BI::CursorType(),
	window_(window),
	arrow_(0),
	cross_(0)
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
};

#endif	// _GLFW_CURSOR_HPP_