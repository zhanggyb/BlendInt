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
	Viewport* vp2 = Manage(new Viewport);
	Viewport* vp3 = Manage(new Viewport);

	FrameSplitter* splitter = Manage(new FrameSplitter);

	FrameSplitter* splitter2 = Manage(new FrameSplitter(Vertical));
	splitter2->Resize(600, 720);

	splitter->Resize(1200, 720);
	//splitter->SetPosition(20, 20);

	splitter2->AddFrame(vp2);
	splitter2->AddFrame(vp3);

	splitter->AddFrame(vp1);
	splitter->AddFrame(splitter2);

	AddFrame(splitter);

	splitter->Resize(1200, 760);

	events()->connect(resized(), splitter, static_cast<void (BI::AbstractWidget::*)(const BI::Size&) >(&BI::FrameSplitter::Resize));
}
