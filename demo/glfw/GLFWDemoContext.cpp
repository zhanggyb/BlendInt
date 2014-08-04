/*
 * GLFWContext.cpp
 */

#include "GLFWDemoContext.hpp"

GLFWDemoContext::GLFWDemoContext()
: BI::Context(),
  m_view3d(0),
  m_browser(0),
  m_open_button(0)
{
	Initialize ();
}

GLFWDemoContext::~GLFWDemoContext ()
{

}

void GLFWDemoContext::Initialize ()
{
	using namespace BI;

	/*
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
	*/

	TextEntry* text = Manage(new TextEntry);
	PushBack(text);
	text->SetPosition(200, 100);
	text->SetRoundCornerType(RoundAll);
	String str;
	text->SetText(str);
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
