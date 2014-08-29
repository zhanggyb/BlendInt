/*
 * GLFWContext.cpp
 */

#include "GLFWDemoContext.hpp"

#include <BlendInt/Core/Image.hpp>

GLFWDemoContext::GLFWDemoContext(GLFWwindow* window)
: BI::Context(),
  arrow_(0),
  cross_(0),
  window_(window)
{
	InitializeGLFWCursors();

	Initialize ();
}

GLFWDemoContext::~GLFWDemoContext ()
{
	ReleaseGLFWCursors();
}

void GLFWDemoContext::SetCursor (int cursor_type)
{
	using namespace BlendInt;

	if(cursor_type < 0 || cursor_type >= CursorShapeLast) {
		return;
	}

	switch (cursor_type) {

		case ArrowCursor: {
			glfwSetCursor(window_, arrow_);
			break;
		}

		case CrossCursor: {
			glfwSetCursor(window_, cross_);
			break;
		}

		default: {
			glfwSetCursor(window_, arrow_);
			break;
		}
	}
}

void GLFWDemoContext::InitializeGLFWCursors()
{
	using namespace BI;

	GLFWimage cursor;
	Image img;

	if(img.Read("0.png")) {
		cursor.width = img.width();
		cursor.height = img.height();
		cursor.pixels = const_cast<unsigned char*>(img.pixels());

		arrow_ = glfwCreateCursor(&cursor, 3, 3);
		assert(arrow_ != 0);
	}

	if(img.Read("2.png")) {
		cursor.width = img.width();
		cursor.height = img.height();
		cursor.pixels = const_cast<unsigned char*>(img.pixels());

		cross_ = glfwCreateCursor(&cursor, 12, 12);
		assert(cross_ != 0);
	}
}

void GLFWDemoContext::Initialize ()
{
	using namespace BI;

	Workspace* ws = Manage(new Workspace);
	ws->SetPosition(20, 20);
	ws->Resize(960, 720);

	PushBack(ws);

	StaticPanel* panel = Manage(new StaticPanel);
	DBG_SET_NAME(panel, "Static Panel");
	panel->SetPosition(1020, 200);
	panel->SetSpace(0);

	Decoration* dec = Manage(new Decoration);
	DBG_SET_NAME(dec, "Decoration");
	dec->SetRoundType(RoundTopLeft | RoundTopRight);

	ColorSelector* cs = Manage(new ColorSelector);
	DBG_SET_NAME(cs, "ColorSelector");
	cs->SetRoundType(RoundBottomLeft | RoundBottomRight);

	panel->SetDecoration(dec);
	panel->SetContent(cs);

	panel->Resize(panel->GetPreferredSize());

	PushBack(panel);

}

void GLFWDemoContext::ReleaseGLFWCursors()
{
	glfwDestroyCursor(arrow_);
	glfwDestroyCursor(cross_);
}
