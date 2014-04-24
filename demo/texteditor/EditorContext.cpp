/*
 * GLFWContext.cpp
 */

#include "EditorContext.hpp"

EditorContext::EditorContext()
: BI::Context(), m_label(0)
{
	InitContext ();
}

EditorContext::~EditorContext ()
{

}

void EditorContext::InitContext()
{
	m_ok = Manage(new BI::Button("OK"));
	m_ok->SetPosition(530, 360);

	m_label = Manage(new BI::Label("Aa"));
	m_label->SetPosition(20, 20);
	m_label->Resize(320, 320);
	m_label->SetFont(BI::Font("Sans", 180));
	m_label->set_background_color(BI::Color(0x00102040));

	Add(m_ok);
	Add(m_label);
}
