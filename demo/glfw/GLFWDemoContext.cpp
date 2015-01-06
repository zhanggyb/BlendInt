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

void GLFWDemoContext::Debug()
{
	Dialog * dialog = Manage(new Dialog("ScrollView", true));
	dialog->Resize(500, 400);
	dialog->MoveTo(100, 100);

	Block* main_block = Manage(new Block(Vertical));
//	VLayout* main_block = Manage(new VLayout);
	DBG_SET_NAME(main_block, "MainBlock");

//	HLayout* block1 = Manage(new HLayout);
	Block* block1 = Manage(new Block(Horizontal));
	DBG_SET_NAME(block1, "Block1");
	Button* btn1 = Manage(new Button("Button1"));
	Button* btn2 = Manage(new Button("Button2"));
	Button* btn3 = Manage(new Button("Button3"));
	block1->AddWidget(btn1);
	block1->AddWidget(btn2);
	block1->AddWidget(btn3);

//	HLayout* block2 = Manage(new HLayout);
	Block* block2 = Manage(new Block(Horizontal));
	DBG_SET_NAME(block2, "Block2");
	Button* btn4 = Manage(new Button("Button4"));
	Button* btn5 = Manage(new Button("Button5"));
	Button* btn6 = Manage(new Button("Button6"));
	block2->AddWidget(btn4);
	block2->AddWidget(btn5);
	block2->AddWidget(btn6);

	main_block->AddWidget(block1);
	main_block->AddWidget(block2);
	main_block->Resize(main_block->GetPreferredSize());
	main_block->MoveTo(100, 100);

	DBG_PRINT_MSG("block1 size: (%d, %d)", block1->size().width(), block1->size().height());
	DBG_PRINT_MSG("button1 size: (%d, %d)", btn1->size().width(), btn1->size().height());
	DBG_PRINT_MSG("button2 size: (%d, %d)", btn2->size().width(), btn2->size().height());
	DBG_PRINT_MSG("button3 size: (%d, %d)", btn3->size().width(), btn3->size().height());
	DBG_PRINT_MSG("block2 size: (%d, %d)", block2->size().width(), block2->size().height());
	DBG_PRINT_MSG("main block size: (%d, %d)", main_block->size().width(), main_block->size().height());

	DBG_PRINT_MSG("main block size: (%d, %d)", main_block->size().width(), main_block->size().height());

	dialog->AddWidget(main_block);

	AddFrame(dialog);
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
