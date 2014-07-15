/*
 * TexBufContext.cpp
 */

#include "TexBufContext.hpp"


TexBufContext::TexBufContext()
: BI::Context(), m_button(0), m_panel(0)
{
	activate_events();

	CreateWidgets();
}

TexBufContext::~TexBufContext ()
{

}

void TexBufContext::CreateWidgets ()
{
	using namespace BlendInt;

	m_button = Manage(new Button("Take Screenshot"));

	Button* btn = Manage(new Button("Button in Frame"));

	m_panel = Manage(new FramePanel);
	m_panel->Setup(btn);

	m_button->SetPosition(600, 200);

	m_panel->SetPosition(100, 100);

	PushBack(m_panel);
	PushBack(m_button);

	events()->connect(m_button->clicked(), this, &TexBufContext::OnTakeScreenShot);
}

void TexBufContext::OnTakeScreenShot ()
{
	DBG_PRINT_MSG("%s", "Take a screen shot of panel");

	BI::Section::RenderToFile(m_panel, "Panel.png");
}
