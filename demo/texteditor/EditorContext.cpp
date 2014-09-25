/*
 * GLFWContext.cpp
 */

#include "EditorContext.hpp"
#include <BlendInt/Gui/Screen.hpp>

EditorContext::EditorContext()
: BI::Context(),
  m_vshader_file(0),
  m_fshader_file(0),
  m_ok(0),
  //m_label(0),
  m_fontview(0)
{
	InitContext ();
}

EditorContext::~EditorContext ()
{

}

void EditorContext::InitContext()
{
	using namespace BI;

	m_vshader_file = Manage(new BI::TextEntry);
	m_vshader_file->SetText("vert.glsl");
	m_vshader_file->SetPosition(20, 440);

	m_fshader_file = Manage(new BI::TextEntry);
	m_fshader_file->SetText("frag.glsl");
	m_fshader_file->SetPosition(20, 400);

	m_ok = Manage(new BI::Button("OK"));
	m_ok->SetPosition(530, 360);

	//m_label = Manage(new BI::Label("Aa"));
	//m_label->SetPosition(20, 20);
	//m_label->Resize(320, 320);
	//m_label->SetFont(BI::Font("Sans", 180));
	//m_label->set_background_color(BI::Color(0x00102040));

	m_fontview = Manage(new FontView);
	m_fontview->SetPosition(100, 100);
	m_fontview->LoadCharacter();

	Screen* screen1 = Manage(new Screen);
	screen1->Setup(m_vshader_file);
	screen1->SetPosition(20, 440);

	Screen* screen2 = Manage(new Screen);
	screen2->Setup(m_fshader_file);
	screen2->SetPosition(20, 400);

	Screen* screen3 = Manage(new Screen);
	screen3->Setup(m_ok);
	screen3->SetPosition(530, 360);

	Screen* screen4 = Manage(new Screen);
	screen4->Setup(m_fontview);
	screen4->SetPosition(100, 100);

	AddScreen(screen1);
	AddScreen(screen2);
	AddScreen(screen3);
	AddScreen(screen4);
}
