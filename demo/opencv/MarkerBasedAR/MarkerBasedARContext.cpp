/*
 * GLFWContext.cpp
 */

#include <GLFW/glfw3.h>

#include "MarkerBasedARContext.hpp"

#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/CVImageViewport.hpp>
#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/Expander.hpp>
#include <BlendInt/Gui/NumericalSlider.hpp>
#include <BlendInt/Gui/Block.hpp>
#include <BlendInt/Gui/ComboBox.hpp>

#include <BlendInt/Gui/ToolBox.hpp>
#include <BlendInt/Gui/FrameSplitter.hpp>
#include <BlendInt/Gui/MenuButton.hpp>
#include <BlendInt/Gui/ToggleButton.hpp>
#include <BlendInt/Gui/Separator.hpp>

using namespace BlendInt;

MarkerBasedARContext::MarkerBasedARContext(GLFWwindow* window)
: BI::Context(),
  viewport_(0),
  status_(VideoStop),
  window_(window)
{
	FrameSplitter* vsplitter = Manage(new FrameSplitter(Vertical));

	FrameSplitter* splitter = Manage(new FrameSplitter);

	ToolBox* tools = CreateToolBoxOnce();
	viewport_ = Manage(new CVImageViewport);

	splitter->AddFrame(viewport_);
	splitter->AddFrame(tools, PreferredWidth);

	ToolBox* bar = CreateToolBarOnce();

	vsplitter->AddFrame(bar);
	vsplitter->AddFrame(splitter, ExpandY);

	AddFrame(vsplitter);

	events()->connect(resized(), vsplitter, static_cast<void (BI::AbstractInteractiveForm::*)(const BI::Size&) >(&BI::FrameSplitter::Resize));

	timer_.reset(new Timer);
	//timer_->SetInterval(1000 / 30);	// 30 fps
	timer_->SetInterval(1000 / 30);	// 25 fps

	events()->connect(timer_->timeout(), this, &MarkerBasedARContext::OnTimeout);
}

MarkerBasedARContext::~MarkerBasedARContext ()
{

}

void MarkerBasedARContext::SynchronizeWindow()
{
	glfwPostEmptyEvent();
}

void MarkerBasedARContext::MakeGLContextCurrent()
{
	assert(window_);

	glfwMakeContextCurrent(window_);
}

ToolBox* MarkerBasedARContext::CreateToolBoxOnce()
{
	ToolBox* tools = Manage(new ToolBox(Vertical));

	Expander* expander = Manage(new Expander("Resolution"));

	NumericalSlider* ns1 = Manage(new NumericalSlider);
	NumericalSlider* ns2 = Manage(new NumericalSlider);

	ToggleButton* open = Manage(new ToggleButton("Open Camera"));
	Separator* separator1 = Manage(new Separator);
	Separator* separator2 = Manage(new Separator);

	Block* vblock = Manage(new Block(Vertical));
	vblock->AddWidget(ns1);
	vblock->AddWidget(ns2);

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
	tools->AddWidget(separator1);
	tools->AddWidget(open);
	tools->AddWidget(separator2);
	tools->AddWidget(vblock1);

	events()->connect(open->toggled(), this, &MarkerBasedARContext::OnToggleCamera);
	events()->connect(play->clicked(), this, &MarkerBasedARContext::OnPlay);
	events()->connect(pause->clicked(), this, &MarkerBasedARContext::OnPause);
	events()->connect(stop->clicked(), this, &MarkerBasedARContext::OnStop);

	return tools;
}

ToolBox* MarkerBasedARContext::CreateToolBarOnce()
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

bool MarkerBasedARContext::OpenCamera(int n, const BI::Size& resolution)
{
	bool retval = false;

	video_stream_.open(n);
	if(video_stream_.isOpened()) {

		video_stream_.set(CV_CAP_PROP_FRAME_WIDTH, resolution.width());
		video_stream_.set(CV_CAP_PROP_FRAME_HEIGHT, resolution.height());

	} else {
		DBG_PRINT_MSG("Error: %s", "Could not acess the camera or video!");
	}

	return retval;
}

void MarkerBasedARContext::OnToggleCamera(bool toggled)
{
	if(timer_->enabled()) {
		timer_->Stop();
	}

	if(toggled) {
		OpenCamera(0, Size(800, 600));
	} else {
		video_stream_.release();
	}
}

void MarkerBasedARContext::OnPlay()
{
	DBG_PRINT_MSG("%s", "Start Play");
	//viewport_->OpenCamera(0, Size(800, 600));
	//viewport_->Play();
	timer_->Start();
}

void MarkerBasedARContext::OnPause ()
{
	DBG_PRINT_MSG("%s", "Pause");
	//viewport_->Pause();
	timer_->Stop();
}

void MarkerBasedARContext::OnStop()
{
	DBG_PRINT_MSG("%s", "Stop Play");
	//viewport_->Stop();
	timer_->Stop();
}

void MarkerBasedARContext::OnTimeout(Timer* t)
{
	if(video_stream_.isOpened()) {
		MakeGLContextCurrent();
		video_stream_ >> frame_;
		viewport_->LoadImage(frame_);
		Refresh();
	}
}
