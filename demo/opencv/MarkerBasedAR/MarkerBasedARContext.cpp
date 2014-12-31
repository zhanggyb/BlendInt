/*
 * GLFWContext.cpp
 */

#include <GLFW/glfw3.h>

#include "MarkerBasedARContext.hpp"

#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/ImageViewport.hpp>
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
  window_(window)
{
	FrameSplitter* vsplitter = Manage(new FrameSplitter(Vertical));

	FrameSplitter* splitter = Manage(new FrameSplitter);

	ToolBox* tools = CreateToolBoxOnce();
	viewport_ = Manage(new MBARViewport);

	splitter->AddFrame(viewport_);
	splitter->AddFrame(tools, PreferredWidth);

	ToolBox* bar = CreateToolBarOnce();

	vsplitter->AddFrame(bar);
	vsplitter->AddFrame(splitter, ExpandY);

	AddFrame(vsplitter);

	events()->connect(resized(), vsplitter, static_cast<void (BI::AbstractView::*)(const BI::Size&) >(&BI::FrameSplitter::Resize));
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

	ToggleButton* open = Manage(new ToggleButton("Open Camera 0"));
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

void MarkerBasedARContext::OnToggleCamera(AbstractButton* sender, bool toggled)
{
	if(toggled) {
#ifdef __APPLE__
		viewport_->OpenCamera(0, Size(1080, 720));
#else
        viewport_->OpenCamera(0, Size(640, 480));
#endif
	} else {
		viewport_->Release();
	}
}

void MarkerBasedARContext::OnPlay(AbstractButton* sender)
{
	DBG_PRINT_MSG("%s", "Start Play");
	viewport_->Play();
}

void MarkerBasedARContext::OnPause (AbstractButton* sender)
{
	DBG_PRINT_MSG("%s", "Pause");
	viewport_->Pause();
}

void MarkerBasedARContext::OnStop(AbstractButton* sender)
{
	DBG_PRINT_MSG("%s", "Stop Play");
	viewport_->Stop();
}
