/*
 * TexBufContext.cpp
 */

#include "TexBufContext.hpp"
#include <BlendInt/Stock/Icons.hpp>

#include <BlendInt/Gui/Viewport3D.hpp>
#include <BlendInt/Gui/Label.hpp>
#include <BlendInt/Gui/ToggleButton.hpp>
#include <BlendInt/Gui/NumericalSlider.hpp>
#include <BlendInt/Gui/ColorSelector.hpp>
#include <BlendInt/Gui/FileSelector.hpp>
#include <BlendInt/Gui/NodeView.hpp>
#include <BlendInt/Gui/Splitter.hpp>
#include <BlendInt/Gui/HLayout.hpp>
#include <BlendInt/Gui/ListView.hpp>

TexBufContext::TexBufContext()
: BI::Context(),
	m_button(0),
	m_panel1(0),
	m_panel2(0)
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
	using Stock::Icons;

	m_button = Manage(new Button("Take Screenshot"));

	//FileSelector* view = Manage(new FileSelector);
	
	Font my_font ("Droid Sans", 24);
	
	Label* label1 = Manage(new Label("Realtime Render"));
	label1->SetFont(my_font);
	label1->SetForegroundColor(Color(0xF0B0E0EF));
	label1->SetBackgroundColor(Color(0x101010A0));
	label1->Resize(label1->GetPreferredSize());
	
	Label* label2 = Manage(new Label("Off-screen Render Once"));
	label2->SetFont(my_font);
	label2->SetForegroundColor(Color(0xF0B0E0EF));
	label2->SetBackgroundColor(Color(0x101010A0));
	label2->Resize(label2->GetPreferredSize());

	FileSelector* view1 = Manage(new FileSelector);
	FileSelector* view2 = Manage(new FileSelector);

	m_panel1 = Manage(new Frame);
	m_panel2 = Manage(new FramePanel);
	//m_panel->SetMargin(10, 10, 10, 10);
	m_panel1->Setup(view1);
	m_panel2->Setup(view2);

	label1->SetPosition(50, 700);
	m_panel1->SetPosition(50, 100);
	m_panel1->Resize(720, 600);

	label2->SetPosition(830, 700);
	m_panel2->SetPosition(830, 100);
	m_panel2->Resize(720, 600);

	m_button->SetPosition(750, 20);

	Section* section = PushBack(label1);
	section->PushBack(label2);
	PushBack(m_panel1);
	PushBack(m_panel2);
	PushBack(m_button);
	events()->connect(m_button->clicked(), this, &TexBufContext::OnTakeScreenShot);

}

void TexBufContext::OnTakeScreenShot ()
{
	DBG_PRINT_MSG("%s", "Take a screen shot of panel");

	//m_panel->ExportTextureToFile("Panel.png");
}
