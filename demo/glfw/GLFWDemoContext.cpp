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

	Frame* frame1 = Manage(new Frame);
	frame1->SetPosition(800, 200);

	ColorButton* btn = Manage(new ColorButton);
	frame1->Setup(btn);

	AddFrame(frame1);

	Frame* frame2 = Manage(new Frame);
	frame2->SetPosition(800, 300);

	NumericalSlider* slider = Manage(new NumericalSlider);
	slider->Resize(200, slider->size().height());
	frame2->Setup(slider);

	AddFrame(frame2);

	Viewport* vp = Manage(new Viewport);
	vp->SetPosition(20, 100);

	AddFrame(vp);

	vp->Resize(700, 600);
}
