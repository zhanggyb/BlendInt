/*
 * GLFWContext.cpp
 */

#include <GLFW/glfw3.h>

#include "HPEContext.hpp"

#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/CVImageViewport.hpp>
#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/MenuBar.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/Expander.hpp>
#include <BlendInt/Gui/NumericalSlider.hpp>
#include <BlendInt/Gui/Block.hpp>
#include <BlendInt/Gui/ComboBox.hpp>

#include <BlendInt/Gui/ToolBox.hpp>
#include <BlendInt/Gui/FrameSplitter.hpp>

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
	viewport_image_ = Manage(new CVImageViewport);
	viewport_3d_ = Manage(new Viewport);

	splitter->AddFrame(viewport_image_);
	splitter->AddFrame(viewport_3d_, ExpandX);
	splitter->AddFrame(tools, PreferredWidth);

	ToolBox* bar = CreateToolBarOnce();

	vsplitter->AddFrame(bar);
	vsplitter->AddFrame(splitter, ExpandY);

	AddFrame(vsplitter);

	events()->connect(resized(), vsplitter, static_cast<void (BI::AbstractInteractiveForm::*)(const BI::Size&) >(&BI::FrameSplitter::Resize));

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

	Button* play = Manage(new Button("Play"));
	Button* pause = Manage(new Button("Pause"));
	Button* stop = Manage(new Button("Stop"));

	Block* vblock1 = Manage(new Block(Vertical));
	vblock1->AddWidget(play);
	vblock1->AddWidget(pause);
	vblock1->AddWidget(stop);

	tools->AddWidget(expander);
	tools->AddWidget(vblock1);

	events()->connect(play->clicked(), this, &HPEContext::OnPlay);
	events()->connect(pause->clicked(), this, &HPEContext::OnPause);
	events()->connect(stop->clicked(), this, &HPEContext::OnStop);

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

bool HPEContext::OpenCamera(int n, const BI::Size& resolution)
{
	bool retval = false;

	video_stream_.open(n);
	if(video_stream_.isOpened()) {

		video_stream_.set(CV_CAP_PROP_FRAME_WIDTH, resolution.width());
		video_stream_.set(CV_CAP_PROP_FRAME_HEIGHT, resolution.height());

		Refresh();
	} else {
		DBG_PRINT_MSG("Error: %s", "Could not acess the camera or video!");
	}

	return retval;
}

void HPEContext::OnPlay()
{
	DBG_PRINT_MSG("%s", "Start Play");
	//viewport_->OpenCamera(0, Size(800, 600));
	//viewport_->Play();
	timer_->Start();
}

void HPEContext::OnPause ()
{
	DBG_PRINT_MSG("%s", "Pause");
	//viewport_->Pause();
	timer_->Stop();
}

void HPEContext::OnStop()
{
	DBG_PRINT_MSG("%s", "Stop Play");
	//viewport_->Stop();
	timer_->Stop();
}

void HPEContext::OnTimeout(Timer* t)
{
	DBG_PRINT_MSG("%s", "refresh");

	// TODO: create opengl context and load texture

	Refresh();
}
