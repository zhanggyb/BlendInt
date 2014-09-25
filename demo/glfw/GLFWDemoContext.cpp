/*
 * GLFWContext.cpp
 */

#include "GLFWDemoContext.hpp"

#include <BlendInt/Core/Image.hpp>
#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Gui/RadioButton.hpp>
#include <BlendInt/Gui/CheckButton.hpp>
#include <BlendInt/Gui/Screen.hpp>
#include <BlendInt/Gui/ScreenSplitter.hpp>
#include <BlendInt/Gui/FileButton.hpp>

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

	Screen* screen = Manage(new Screen);

	AddScreen(screen);

	StaticFrame* f = Manage(new StaticFrame);
//	Frame* f = Manage(new Frame);

	Viewport3D* view3d = Manage(new Viewport3D);

	f->Setup(view3d);
	f->Resize(600, 500);

	screen->SetPosition(200, 100);

	screen->Setup(f);
}
