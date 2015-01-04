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
#include <BlendInt/Gui/ColorSelector.hpp>

#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Gui/Dialog.hpp>

#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/ToolBox.hpp>
#include <BlendInt/Gui/Block.hpp>
#include <BlendInt/Gui/CloseButton.hpp>

using namespace BI;
using BI::Stock::Shaders;

GLFWDemoContext::GLFWDemoContext(GLFWwindow* win)
: BI::Context(),
  window_(win)
{
//	TryToolBox();

	InitializeGLFWDemoContext ();
}

GLFWDemoContext::~GLFWDemoContext ()
{
}

void GLFWDemoContext::SynchronizeWindow()
{
	glfwPostEmptyEvent();
}

void GLFWDemoContext::TryToolBox()
{
	ToolBox* tool = Manage(new ToolBox);

	tool->MoveTo(200, 200);

	AddFrame(tool);
}

void GLFWDemoContext::InitializeGLFWDemoContext ()
{
	ToolBox* vp1 = Manage(new ToolBox(Vertical));
	//vp1->SetPosition(200, 200);
	//frame->Resize(400, 32);

	Block* block = Manage(new Block(Vertical));

	Button* btn1 = Manage(new Button("Hello"));
	DBG_SET_NAME(btn1, "Button1");
	Button* btn2 = Manage(new Button("Hello"));
	DBG_SET_NAME(btn2, "Button2");
	Button* btn3 = Manage(new Button("Hello"));
	DBG_SET_NAME(btn3, "Button3");

	block->AddWidget(btn1);
	block->AddWidget(btn2);
	block->AddWidget(btn3);

	ScrollBar* bar = Manage(new ScrollBar);

	vp1->AddWidget(block);
	vp1->AddWidget(bar);

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

	splitter1->AddFrame(splitter2);
	splitter1->AddFrame(vp1, PreferredWidth);

	AddFrame(splitter1);

	splitter1->Resize(1200, 760);

	events()->connect(resized(), splitter1, static_cast<void (BI::AbstractView::*)(const BI::Size&) >(&BI::FrameSplitter::Resize));

	Dialog* dlg = Manage(new Dialog(String("Hello")));
    dlg->Resize(800, 600);
    dlg->MoveTo(200, 150);
	AddFrame(dlg);

	Button* ok = Manage(new Button("OK"));
	Button* cancel = Manage(new Button("Cancel"));
	dlg->AddWidget(cancel);
	dlg->AddWidget(ok);

	ok->MoveTo(dlg->size().width() - ok->size().width() - 10, 10);
	cancel->MoveTo(ok->position().x() - 5 - cancel->size().width(), 10);

	PopupFrame* cs = Manage(new PopupFrame);
	cs->Resize(240, 320);
	cs->MoveTo(500, 300);
	AddFrame(cs);

	TextEntry* text = Manage(new TextEntry);
	text->MoveTo(20, 20);
	cs->AddWidget(text);

	ScrollBar* scroll = Manage(new ScrollBar(Horizontal));
	scroll->MoveTo(20, 100);
	cs->AddWidget(scroll);

	Button* b = Manage(new Button("Test"));
	b->MoveTo(20, 150);
	cs->AddWidget(b);

}
