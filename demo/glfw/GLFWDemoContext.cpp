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

	StaticPanel* panel1 = Manage(new StaticPanel);

	VBlockLayout* block = Manage(new VBlockLayout);

	Button* btn1 = Manage(new Button("Button"));
	Button* btn2 = Manage(new Button("Button"));
	Button* btn3 = Manage(new Button("Button"));
	Button* btn4 = Manage(new Button("Button"));

	block->PushBack(btn1);
	block->PushBack(btn2);
	block->PushBack(btn3);
	block->PushBack(btn4);

	panel1->Setup(block);
	panel1->Resize(panel1->GetPreferredSize());

	StaticPanel* panel2 = Manage(new StaticPanel);

	ColorSelector* cs = Manage(new ColorSelector);

	panel2->Setup(cs);
	panel2->Resize(panel2->GetPreferredSize());

	Section* section = PushBack(panel1);
	section->PushBack(panel2);
}

void GLFWDemoContext::ReleaseGLFWCursors()
{
	glfwDestroyCursor(arrow_);
	glfwDestroyCursor(cross_);
}
