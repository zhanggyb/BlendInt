/*
 * GLFWContext.cpp
 */

#include "GLFWDemoContext.hpp"

#include <BlendInt/Core/Image.hpp>

GLFWDemoContext::GLFWDemoContext(GLFWwindow* window)
: BI::Context(),
  m_view3d(0),
  m_browser(0),
  m_open_button(0),
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

	m_view3d = Manage(new Viewport3D);
	m_view3d->SetPosition(20, 100);
	m_view3d->Resize(800, 600);

	PushBack(m_view3d);

	m_browser = Manage(new FileBrowser);
	m_open_button = Manage(new Button("Load File"));

	PushBack(m_browser);
	m_browser->SetPosition(850, 300);
	PushBack(m_open_button);
	m_open_button->SetPosition(900, 200);

	events()->connect(m_open_button->clicked(), this, &GLFWDemoContext::OnOpenObjFile);
}

void GLFWDemoContext::ReleaseGLFWCursors()
{
	glfwDestroyCursor(arrow_);
	glfwDestroyCursor(cross_);
}

void GLFWDemoContext::OnOpenObjFile()
{
	using namespace BI;

	if(m_browser->file_selected().empty()) return;
	std::string filename = ConvertFromString(m_browser->file_selected());

	DBG_PRINT_MSG("Load file: %s", filename.c_str());

	RefPtr<Mesh> monkey(new Mesh);

	if(monkey->Load(filename.c_str())) {
		m_view3d->PushBack(monkey);
	}
}
