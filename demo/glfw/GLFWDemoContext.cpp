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

	Screen* vp = Manage(new Screen);

	AddScreen(vp);

	FileButton* btn = Manage(new FileButton);
	btn->SetPosition(200, 200);

	vp->AddWidget(btn);

	events()->connect(resized(), vp, static_cast<void (AbstractWidget::*)(const Size&) >(&Screen::Resize));
}
