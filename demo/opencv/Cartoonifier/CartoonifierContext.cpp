/*
 * GLFWContext.cpp
 */

#include <GLFW/glfw3.h>

#include "CartoonifierContext.hpp"

#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/ImageViewport.hpp>
#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/Expander.hpp>
#include <BlendInt/Gui/NumericalSlider.hpp>
#include <BlendInt/Gui/Block.hpp>

#include <BlendInt/Gui/ToolBox.hpp>
#include <BlendInt/Gui/FrameSplitter.hpp>

using namespace BlendInt;

CartoonifierContext::CartoonifierContext()
: BI::Context(),
  video_(0)
{
	FrameSplitter* splitter = new FrameSplitter;

	ToolBox* tools = CreateToolBoxOnce();
	video_ = new CVVideoViewport;

	splitter->AddFrame(video_);
	splitter->AddFrame(tools);

	AddFrame(splitter);

	events()->connect(resized(), splitter, static_cast<void (BI::AbstractView::*)(const BI::Size&) >(&BI::FrameSplitter::Resize));
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
	ToolBox* tools = new ToolBox(Vertical);

	Expander* expander = new Expander("Light");

	NumericalSlider* ns1 = new NumericalSlider;
	NumericalSlider* ns2 = new NumericalSlider;
	NumericalSlider* ns3 = new NumericalSlider;

	Block* vblock = new Block(Vertical);
	vblock->AddWidget(ns1);
	vblock->AddWidget(ns2);
	vblock->AddWidget(ns3);

	expander->Setup(vblock);
	expander->Resize(expander->GetPreferredSize());

	Button* play = new Button("Play");
	Button* pause = new Button("Pause");
	Button* stop = new Button("Stop");

	tools->AddWidget(expander);
	tools->AddWidget(play);
	tools->AddWidget(pause);
	tools->AddWidget(stop);

	events()->connect(play->clicked(), this, &CartoonifierContext::OnPlay);
	events()->connect(pause->clicked(), this, &CartoonifierContext::OnPause);
	events()->connect(stop->clicked(), this, &CartoonifierContext::OnStop);

	return tools;
}

void CartoonifierContext::OnPlay(AbstractButton* sender)
{
	DBG_PRINT_MSG("%s", "Start Play");
	video_->OpenCamera(0, Size(800, 600));
	video_->Play();
}

void CartoonifierContext::OnPause (AbstractButton* sender)
{
	DBG_PRINT_MSG("%s", "Pause");
	video_->Pause();
}

void CartoonifierContext::OnStop(AbstractButton* sender)
{
	DBG_PRINT_MSG("%s", "Stop Play");
	video_->Stop();
}
