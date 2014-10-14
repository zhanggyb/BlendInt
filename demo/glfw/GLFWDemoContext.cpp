/*
 * GLFWContext.cpp
 */

#include "GLFWDemoContext.hpp"

#include <BlendInt/Core/Image.hpp>
#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Gui/RadioButton.hpp>
#include <BlendInt/Gui/CheckButton.hpp>
#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/FrameSplitter.hpp>
#include <BlendInt/Gui/FileButton.hpp>
#include <BlendInt/Gui/ColorWheel.hpp>
#include <BlendInt/Gui/Viewport2D.hpp>
#include <BlendInt/Gui/Viewport.hpp>

#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Gui/HFrame.hpp>
#include <BlendInt/Gui/VFrame.hpp>
#include <BlendInt/Gui/FloatingFrame.hpp>

using BI::Stock::Shaders;

GLFWDemoContext::GLFWDemoContext()
: BI::Context()
{
	Initialize ();
}

GLFWDemoContext::~GLFWDemoContext ()
{
}

void GLFWDemoContext::Initialize ()
{
	using namespace BI;

	Viewport* vp1 = Manage(new Viewport);
	DBG_SET_NAME(vp1, "Viewport1");

	Viewport* vp2 = Manage(new Viewport);
	DBG_SET_NAME(vp2, "Viewport2");

	Viewport* vp3 = Manage(new Viewport);
	DBG_SET_NAME(vp3, "VFrame");

	FrameSplitter* splitter1 = Manage(new FrameSplitter);
	DBG_SET_NAME(splitter1, "Splitter1");

	FrameSplitter* splitter2 = Manage(new FrameSplitter(Vertical));
	DBG_SET_NAME(splitter2, "Splitter2");
	splitter2->Resize(600, 720);

	splitter1->Resize(1200, 720);
	//splitter1->SetPosition(20, 20);

	splitter2->AddFrame(vp2);
	splitter2->AddFrame(vp3);

	splitter1->AddFrame(vp1);
	splitter1->AddFrame(splitter2);

	AddFrame(splitter1);

	splitter1->Resize(1200, 760);

	events()->connect(resized(), splitter1, static_cast<void (BI::AbstractWidget::*)(const BI::Size&) >(&BI::FrameSplitter::Resize));

	/*
	VFrame* frame = Manage(new VFrame);
	frame->SetPosition(200, 200);
	//frame->Resize(400, 32);

	Button* btn1 = Manage(new Button("Hello"));
	Button* btn2 = Manage(new Button("Hello"));
	Button* btn3 = Manage(new Button("Hello"));

	frame->AddWidget(btn1);
	frame->AddWidget(btn2);
	frame->AddWidget(btn3);

	AddFrame(frame);
	*/

	FloatingFrame* ff = Manage(new FloatingFrame);

	ff->SetPosition(400, 200);
	ff->Resize(600, 400);

	AddFrame(ff);

	FloatingFrame* ff2 = Manage(new FloatingFrame);

	Button* btn = Manage(new Button("Test Floating Frame"));

	ff2->Setup(btn);

	ff2->SetPosition(500, 200);
	ff2->Resize(500, 300);

	AddFrame(ff2);

}
