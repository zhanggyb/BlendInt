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
	FrameSplitter* workspace = CreateWorkspaceOnce();

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

	Expander* expander = Manage(new Expander("Light"));

	NumericalSlider* ns1 = Manage(new NumericalSlider);
	NumericalSlider* ns2 = Manage(new NumericalSlider);
	NumericalSlider* ns3 = Manage(new NumericalSlider);

	Block* vblock = Manage(new Block(Vertical));
	vblock->AddWidget(ns1);
	vblock->AddWidget(ns2);
	vblock->AddWidget(ns3);

	expander->Setup(vblock);
	expander->Resize(expander->GetPreferredSize());

	Panel* btn_panel = CreateButtons();

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

Panel* HPEContext::CreateButtons()
{
	Panel* panel = Manage(new Panel);
	panel->SetRoundType(RoundAll);

	Block* hblock1 = Manage(new Block(Horizontal));

	NumericalSlider* camera_no = Manage(new NumericalSlider);
	Button* btn1 = Manage(new Button("Open Camera"));

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

BI::FrameSplitter* HPEContext::CreateWorkspaceOnce()
{
	FrameSplitter* vsplitter = Manage(new FrameSplitter(Vertical));

	FrameSplitter* hsplitter = Manage(new FrameSplitter(Horizontal));
	viewport_image_ = Manage(new ImageViewport);
	viewport_3d_ = Manage(new Viewport);
	hsplitter->AddFrame(viewport_image_);
	hsplitter->AddFrame(viewport_3d_);

	ToolBox* toolbar = Manage(new ToolBox(Horizontal));

	vsplitter->AddFrame(hsplitter);
	vsplitter->AddFrame(toolbar);

	return vsplitter;
}

void HPEContext::OnTimeout(Timer* t)
{
	DBG_PRINT_MSG("%s", "refresh");

	// TODO: create opengl context and load texture

	RequestRedraw();
}
