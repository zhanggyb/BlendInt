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
	screen->Resize(size());

	AddScreen(screen);

	StaticFrame* f = Manage(new StaticFrame);
	f->Resize(400, 300);

	Button* btn = Manage(new Button("hello there!"));
	//Viewport3D* view = Manage(new Viewport3D);
	//view->SetPosition(100, 100);
	//view->Resize(600, 500);

	f->Setup(btn);
	f->SetPosition(200, 100);

	screen->AddContainer(f);

	events()->connect(resized(), screen, static_cast<void (AbstractWidget::*)(const Size&) >(&Screen::Resize));
}
