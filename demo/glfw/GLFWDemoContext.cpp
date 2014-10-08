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
	frame1->SetPosition(20, 100);
	AddFrame(frame1);

	Viewport3D* view1 = Manage(new Viewport3D);
	view1->Resize(600, 500);

	frame1->Setup(view1);

	Frame* frame2 = Manage(new Frame);
	frame2->SetPosition(660, 100);
	AddFrame(frame2);

	Viewport3D* view2 = Manage(new Viewport3D);
	view2->Resize(600, 500);

	frame2->Setup(view2);
}
