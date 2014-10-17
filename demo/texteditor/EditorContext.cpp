/*
 * GLFWContext.cpp
 */

#include "EditorContext.hpp"
#include <BlendInt/Gui/Frame.hpp>

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

	Frame* screen1 = Manage(new Frame);
	screen1->Resize(m_vshader_file->size());
	screen1->AddWidget(m_vshader_file);
	screen1->SetPosition(20, 440);

	Frame* screen2 = Manage(new Frame);
	screen2->Resize(m_fshader_file->size());
	screen2->AddWidget(m_fshader_file);
	screen2->SetPosition(20, 400);

	Frame* screen3 = Manage(new Frame);
	screen3->Resize(m_ok->size());
	screen3->AddWidget(m_ok);
	screen3->SetPosition(530, 360);

	Frame* screen4 = Manage(new Frame);
	screen4->Resize(m_fontview->size());
	screen4->AddWidget(m_fontview);
	screen4->SetPosition(100, 100);

	AddFrame(screen1);
	AddFrame(screen2);
	AddFrame(screen3);
	AddFrame(screen4);
}
