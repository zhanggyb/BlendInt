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
#include <BlendInt/Gui/Frame.hpp>

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

	Frame* screen1 = Manage(new Frame);
	AddFrame(screen1);

	Frame* screen2 = Manage(new Frame);
	AddFrame(screen2);

	Frame* screen3 = Manage(new Frame);
	AddFrame(screen3);

	Frame* screen4 = Manage(new Frame);
	AddFrame(screen4);

	Frame* screen5 = Manage(new Frame);
	AddFrame(screen5);

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

	Viewport3D* view1 = Manage(new Viewport3D);
	Viewport3D* view2 = Manage(new Viewport3D);

	m_panel1 = Manage(new Panel);
	m_panel2 = Manage(new StaticPanel);
	//m_panel->SetMargin(10, 10, 10, 10);
	m_panel1->Setup(view1);
	m_panel2->Setup(view2);

	m_panel1->Resize(720, 600);

	m_panel2->Resize(720, 600);

	screen1->AddWidget(label1);
	screen1->SetPosition(50, 700);

	screen2->AddWidget(label2);
	screen2->SetPosition(830, 700);

	screen3->AddWidget(m_panel1);
	screen3->SetPosition(50, 100);

	screen4->AddWidget(m_panel2);
	screen4->SetPosition(830, 100);

	screen5->AddWidget(m_button);
	screen5->SetPosition(750, 20);

	events()->connect(m_button->clicked(), this, &TexBufContext::OnTakeScreenShot);
	//events()->connect(resized(), screen, static_cast<void (AbstractWidget::*)(const Size&) >(&Screen::Resize));

}

void TexBufContext::OnTakeScreenShot ()
{
	DBG_PRINT_MSG("%s", "Take a screen shot of panel");

	//m_panel2->RenderToFile("Panel2.png");
}
