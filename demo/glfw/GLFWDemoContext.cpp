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

	Screen* screen2 = Manage(new Screen);
	screen2->SetPosition(400, 200);
	AddScreen(screen2);

	ToolBox* tb = Manage(new ToolBox);
	screen2->Setup(tb);

	Button* btn1 = Manage(new Button("Button1"));
	Button* btn2 = Manage(new Button("Button1"));
	Button* btn3 = Manage(new Button("Button1"));

	tb->Append(btn1);
	tb->Append(btn2);
	tb->Append(btn3);

	screen2->SetPosition(600, 200);

	screen2->Resize(tb->GetPreferredSize());
}
