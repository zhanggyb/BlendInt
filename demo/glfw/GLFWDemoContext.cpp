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
	frame1->SetPosition(200, 200);
	AddFrame(frame1);

	ColorButton* cb = Manage(new ColorButton);
	cb->SetColor(Color(0xFF88449F));

	frame1->Setup(cb);

	Frame* frame2 = Manage(new Frame);
	frame2->SetPosition(400, 200);
	AddFrame(frame2);

	ScrollBar* exp = Manage(new ScrollBar);
	exp->Resize(100, 14);

	frame2->Setup(exp);

	frame2->SetPosition(600, 200);
}
