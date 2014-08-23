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

	NodeView* view1 = Manage(new NodeView);
	NodeView* view2 = Manage(new NodeView);

	m_panel1 = Manage(new Frame);
	m_panel2 = Manage(new FramePanel);
	//m_panel->SetMargin(10, 10, 10, 10);
	m_panel1->Setup(view1);
	m_panel2->Setup(view2);

	m_panel1->SetPosition(50, 100);
	m_panel1->Resize(580, 600);

	m_panel2->SetPosition(650, 100);
	m_panel2->Resize(580, 600);

	//DBG_PRINT_MSG("pos: %d, %d", fs->position().x(), fs->position().y());
	//DBG_PRINT_MSG("size: %d, %d", fs->size().width(), fs->size().height());

	m_button->SetPosition(550, 20);

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
