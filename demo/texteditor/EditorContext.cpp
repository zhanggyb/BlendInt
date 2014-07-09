/*
 * GLFWContext.cpp
 */

#include "EditorContext.hpp"

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

	PushBack(m_vshader_file);
	PushBack(m_fshader_file);
	PushBack(m_ok);
	//Add(m_label);
	PushBack(m_fontview);
}
