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
	screen->SetPosition(200, 200);
	AddScreen(screen);

	ColorButton* cb = Manage(new ColorButton);
	cb->SetColor(Color(0xFF88449F));

	screen->Setup(cb);
}
