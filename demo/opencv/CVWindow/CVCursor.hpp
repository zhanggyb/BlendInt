/**
 * CVCursor.hpp
 */

#ifndef _CV_CURSOR_HPP_
#define _CV_CURSOR_HPP_

#include <GLFW/glfw3.h>

#include <BlendInt/Gui/CursorType.hpp>

namespace BI=BlendInt;

class CVCursor: public BI::CursorType
{
public:

	CVCursor (GLFWwindow* window)
	: BI::CursorType(),
	window_(window),
	arrow_(nullptr),
	cross_(nullptr),
	split_v_(nullptr),
	split_h_(nullptr)
	{
		Initialize();
	}

	virtual ~CVCursor ()
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

#endif	// _CV_CURSOR_HPP_
