/*
 * GLFWContext.cpp
 */

#include <GLFW/glfw3.h>

#include "HPEContext.hpp"

#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/ImageViewport.hpp>
#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/Expander.hpp>
#include <BlendInt/Gui/NumericalSlider.hpp>
#include <BlendInt/Gui/Block.hpp>
#include <BlendInt/Gui/ComboBox.hpp>

#include <BlendInt/Gui/ToolBox.hpp>
#include <BlendInt/Gui/HLayout.hpp>
#include <BlendInt/Gui/MenuButton.hpp>
#include <BlendInt/Gui/ToggleButton.hpp>
#include <BlendInt/Gui/RadioButton.hpp>

using namespace BlendInt;

HPEContext::HPEContext()
: BI::Context(),
  viewport_image_(0),
  viewport_3d_(0),
  status_(VideoStop)
{
	FrameSplitter* vsplitter = Manage(new FrameSplitter(Vertical));

	FrameSplitter* splitter = Manage(new FrameSplitter);

	ToolBox* tools = CreateToolBoxOnce();
	Workspace* workspace = CreateWorkspaceOnce();

	splitter->AddFrame(workspace);
	splitter->AddFrame(tools, PreferredWidth);

	ToolBox* bar = CreateToolBarOnce();

	vsplitter->AddFrame(bar);
	vsplitter->AddFrame(splitter, ExpandY);

	AddFrame(vsplitter);

	events()->connect(resized(), vsplitter, static_cast<void (BI::AbstractView::*)(const BI::Size&) >(&BI::FrameSplitter::Resize));

	timer_.reset(new Timer);
	timer_->SetInterval(1000 / 30);	// 30 fps

	events()->connect(timer_->timeout(), this, &HPEContext::OnTimeout);

	viewport_image_->OpenFile("test.jpg");
}

HPEContext::~HPEContext ()
{

}

void HPEContext::SynchronizeWindow()
{
	glfwPostEmptyEvent();
}

ToolBox* HPEContext::CreateToolBoxOnce()
{
	ToolBox* tools = Manage(new ToolBox(Vertical));

	HLayout* head_layout = CreateRadios();

	Expander* expander = Manage(new Expander("Resolution"));

	NumericalSlider* ns1 = Manage(new NumericalSlider);
	NumericalSlider* ns2 = Manage(new NumericalSlider);

	Block* vblock = Manage(new Block(Vertical));
	vblock->AddWidget(ns1);
	vblock->AddWidget(ns2);

	expander->Setup(vblock);
	expander->Resize(expander->GetPreferredSize());

	Panel* btn_panel = CreateButtons();

	tools->AddWidget(head_layout);
	tools->AddWidget(expander);
	tools->AddWidget(btn_panel);

	return tools;
}

ToolBox* HPEContext::CreateToolBarOnce()
{
	ToolBox* bar = Manage(new ToolBox(Horizontal));

	ComboBox* combo = Manage(new ComboBox);
	combo->Resize(48, combo->size().height());

	MenuButton* btn1 = Manage(new MenuButton("File"));
	MenuButton* btn2 = Manage(new MenuButton("Edit"));
	MenuButton* btn3 = Manage(new MenuButton("View"));

	bar->AddWidget(combo);
	bar->AddWidget(btn1);
	bar->AddWidget(btn2);
	bar->AddWidget(btn3);

	bar->Resize(bar->GetPreferredSize());

	return bar;
}

Workspace* HPEContext::CreateWorkspaceOnce()
{
	Workspace* workspace = Manage(new Workspace);
	
	return workspace;
}

HLayout* HPEContext::CreateRadios()
{
	radio_group_.reset(new ButtonGroup);

	HLayout* layout = Manage(new HLayout);

	ComboBox* combo = Manage(new ComboBox);

	Block* hblock = Manage(new Block(Horizontal));

	RadioButton* radio1 = Manage(new RadioButton(Context::icons->icon_16x16(Icons::SCENE)));
	RadioButton* radio2 = Manage(new RadioButton(Context::icons->icon_16x16(Icons::SCENE_DATA)));
	RadioButton* radio3 = Manage(new RadioButton(Context::icons->icon_16x16(Icons::SURFACE_NSURFACE)));
	RadioButton* radio4 = Manage(new RadioButton(Context::icons->icon_16x16(Icons::SURFACE_NCIRCLE)));
	RadioButton* radio5 = Manage(new RadioButton(Context::icons->icon_16x16(Icons::SURFACE_NCURVE)));

	radio_group_->AddButton(radio1);
	radio_group_->AddButton(radio2);
	radio_group_->AddButton(radio3);
	radio_group_->AddButton(radio4);
	radio_group_->AddButton(radio5);

	radio1->SetChecked(true);

	hblock->AddWidget(radio1);
	hblock->AddWidget(radio2);
	hblock->AddWidget(radio3);
	hblock->AddWidget(radio4);
	hblock->AddWidget(radio5);

	layout->AddWidget(combo);
	layout->AddWidget(hblock);

	layout->Resize(layout->GetPreferredSize());

	return layout;
}

Panel* HPEContext::CreateButtons()
{
	Panel* panel = Manage(new Panel);
	panel->SetRoundType(RoundAll);

	Block* hblock1 = Manage(new Block(Horizontal));

	ComboBox* camera_no = Manage(new ComboBox);
	ToggleButton* btn1 = Manage(new ToggleButton("Open Camera"));

	hblock1->AddWidget(camera_no);
	hblock1->AddWidget(btn1);

	VLayout* layout = Manage(new VLayout);

	Block* hblock2 = Manage(new Block(Horizontal));

	Button* play = Manage(new Button("Play"));
	Button* pause = Manage(new Button("Pause"));
	Button* stop = Manage(new Button("Stop"));

	hblock2->AddWidget(play);
	hblock2->AddWidget(pause);
	hblock2->AddWidget(stop);

	layout->AddWidget(hblock1);
	layout->AddWidget(hblock2);

	panel->SetLayout(layout);
	panel->Resize(layout->GetPreferredSize());

	events()->connect(play->clicked(), this, &HPEContext::OnPlay);
	events()->connect(pause->clicked(), this, &HPEContext::OnPause);
	events()->connect(stop->clicked(), this, &HPEContext::OnStop);

	return panel;
}

bool HPEContext::OpenCamera(int n, const BI::Size& resolution)
{
	bool retval = false;

	video_stream_.open(n);
	if(video_stream_.isOpened()) {

		video_stream_.set(CV_CAP_PROP_FRAME_WIDTH, resolution.width());
		video_stream_.set(CV_CAP_PROP_FRAME_HEIGHT, resolution.height());

		RequestRedraw();
	} else {
		DBG_PRINT_MSG("Error: %s", "Could not acess the camera or video!");
	}

	return retval;
}

void HPEContext::OnPlay(AbstractButton* sender)
{
	DBG_PRINT_MSG("%s", "Start Play");
	//viewport_->OpenCamera(0, Size(800, 600));
	//viewport_->Play();
	timer_->Start();
}

void HPEContext::OnPause (AbstractButton* sender)
{
	DBG_PRINT_MSG("%s", "Pause");
	//viewport_->Pause();
	timer_->Stop();
}

void HPEContext::OnStop(AbstractButton* sender)
{
	DBG_PRINT_MSG("%s", "Stop Play");
	//viewport_->Stop();
	timer_->Stop();
}

void HPEContext::OnTimeout(Timer* t)
{
	DBG_PRINT_MSG("%s", "refresh");

	// TODO: create opengl context and load texture

	RequestRedraw();
}
