/*
 * GLFWContext.cpp
 */

#include <GLFW/glfw3.h>

#include "CartoonifierContext.hpp"

#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/CVImageViewport.hpp>
#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/MenuBar.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/Expander.hpp>
#include <BlendInt/Gui/NumericalSlider.hpp>
#include <BlendInt/Gui/VBlockLayout.hpp>

#include <BlendInt/Gui/ToolBox.hpp>
#include <BlendInt/Gui/FrameSplitter.hpp>

using namespace BlendInt;

CartoonifierContext::CartoonifierContext()
: BI::Context(),
  video_(0)
{
	FrameSplitter* splitter = Manage(new FrameSplitter);

	ToolBox* tools = CreateToolBoxOnce();
	video_ = Manage(new CVVideoViewport);
	video_->OpenCamera(0);

	splitter->AddFrame(video_);
	splitter->AddFrame(tools);

	AddFrame(splitter);

	events()->connect(resized(), splitter, static_cast<void (BI::AbstractWidget::*)(const BI::Size&) >(&BI::FrameSplitter::Resize));
}

CartoonifierContext::~CartoonifierContext ()
{

}

void CartoonifierContext::SynchronizeWindow()
{
	glfwPostEmptyEvent();
}

ToolBox* CartoonifierContext::CreateToolBoxOnce()
{
	ToolBox* tools = Manage(new ToolBox(Vertical));

	Expander* expander = Manage(new Expander("Light"));

	NumericalSlider* ns1 = Manage(new NumericalSlider);
	NumericalSlider* ns2 = Manage(new NumericalSlider);
	NumericalSlider* ns3 = Manage(new NumericalSlider);

	VBlockLayout* vblock = Manage(new VBlockLayout);
	vblock->Append(ns1);
	vblock->Append(ns2);
	vblock->Append(ns3);

	expander->Setup(vblock);
	expander->Resize(expander->GetPreferredSize());

	Button* play = Manage(new Button("Play"));
	Button* stop = Manage(new Button("Stop"));

	tools->Add(expander);
	tools->Add(play);
	tools->Add(stop);

	events()->connect(play->clicked(), this, &CartoonifierContext::OnPlay);
	events()->connect(stop->clicked(), this, &CartoonifierContext::OnStop);

	return tools;
}

void CartoonifierContext::OnPlay()
{
	DBG_PRINT_MSG("%s", "Start Play");
	video_->Play();
}

void CartoonifierContext::OnStop()
{
	DBG_PRINT_MSG("%s", "Stop Play");
	video_->Pause();
}
