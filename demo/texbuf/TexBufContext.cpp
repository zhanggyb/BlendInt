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

TexBufContext::TexBufContext()
: BI::Context(),
	m_button(0),
	m_panel(0),
	view3d_frame_(0),
	label_frame_(0),
	toggle_frame_(0)
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

	Button* btn = Manage(new Button(Icons::instance->icon_16x16(0), "Button"));

	m_panel = Manage(new FramePanel);
	m_panel->Setup(btn);
	m_panel->SetPosition(100, 100);
	m_panel->Resize(m_panel->GetPreferredSize());

	FramePanel* btn_panel = Manage(new FramePanel);
	btn_panel->Setup(m_button);
	btn_panel->Resize(btn_panel->GetPreferredSize());

	btn_panel->SetPosition(1120, 400);

	// view3d_frame_ = Manage(new FramePanel);
	// view3d_frame_->Setup(Manage(new Viewport3D));
	// view3d_frame_->Resize(500, 400);
	// view3d_frame_->SetPosition(100, 400);

	label_frame_ = Manage(new FramePanel);
	label_frame_->Setup (Manage(new Label("Label")));
	label_frame_->Resize(label_frame_->GetPreferredSize());
	label_frame_->SetPosition(100, 600);

	PushBack(m_panel);

	PushBack(label_frame_);
	
	PushBack(btn_panel);

	events()->connect(m_button->clicked(), this, &TexBufContext::OnTakeScreenShot);
}

void TexBufContext::OnTakeScreenShot ()
{
	DBG_PRINT_MSG("%s", "Take a screen shot of panel");

	BI::Section::RenderToFile(m_panel, "Panel.png");
}
