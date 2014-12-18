/**
 * StudioCursor.hpp
 */

#ifndef _GLFW_CURSOR_HPP_
#define _GLFW_CURSOR_HPP_

#include <GLFW/glfw3.h>

#include <BlendInt/Gui/CursorType.hpp>

namespace BI=BlendInt;

class StudioCursor: public BI::CursorType
{
public:

	StudioCursor (GLFWwindow* window)
	: BI::CursorType(),
	window_(window),
	arrow_(nullptr),
	cross_(nullptr),
	split_v_(nullptr),
	split_h_(nullptr)
	{
		Initialize();
	}

	virtual ~StudioCursor ()
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
};

#endif	// _GLFW_CURSOR_HPP_
