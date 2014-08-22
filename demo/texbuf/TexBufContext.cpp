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

	FileBrowser* view = Manage(new FileBrowser);

	m_panel = Manage(new FramePanel);
	//m_panel->SetMargin(10, 10, 10, 10);
	m_panel->Setup(view);
	m_panel->SetPosition(100, 100);
	m_panel->Resize(800, 600);

	//DBG_PRINT_MSG("pos: %d, %d", fs->position().x(), fs->position().y());
	//DBG_PRINT_MSG("size: %d, %d", fs->size().width(), fs->size().height());

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
	label_frame_->SetPosition(100, 750);

	PushBack(m_panel);

	PushBack(label_frame_);
	
	PushBack(btn_panel);

	events()->connect(m_button->clicked(), this, &TexBufContext::OnTakeScreenShot);
}

void TexBufContext::OnTakeScreenShot ()
{
	DBG_PRINT_MSG("%s", "Take a screen shot of panel");

	m_panel->ExportTextureToFile("Panel.png");
}
