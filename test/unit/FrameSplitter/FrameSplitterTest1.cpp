#include "FrameSplitterTest1.hpp"

#include <Common/UnitTestContext.hpp>
#include <BlendInt/Gui/FrameSplitter.hpp>
#include <BlendInt/Gui/Viewport.hpp>

#include <BlendInt/Gui/ToolBox.hpp>
#include <BlendInt/Gui/Button.hpp>

using namespace BlendInt;

FrameSplitterTest1::FrameSplitterTest1()
: testing::Test()
{
	// TODO: add constructor code
}

FrameSplitterTest1::~FrameSplitterTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
/*
TEST_F(FrameSplitterTest1, AddFrame1)
{
	Init ();

    GLFWwindow* win = CreateWindow("FrameSplitter - Foo1", 1280, 800);

    // TODO: add test code here
    UnitTestContext* context = Manage (new UnitTestContext);
	DBG_SET_NAME(context, "Context");
	SetContext(context);
	context->Resize(1280, 800);

	FrameSplitter* fs = Manage(new FrameSplitter(Vertical));
	DBG_SET_NAME(fs, "FrameSplitter");
	fs->MoveTo(20, 20);
	fs->Resize(1240, 760);

	Viewport* f1 = Manage(new Viewport);
	DBG_SET_NAME(f1, "Frame1");
	ToolBox* f2 = Manage(new ToolBox);
	DBG_SET_NAME(f2, "Frame2");
	f2->Resize(240, 100);

	fs->AddFrame(f1);
	fs->AddFrame(f2, PreferredHeight);

	context->AddFrame(fs);

	//Viewport* f3 = Manage(new Viewport);
	//DBG_SET_NAME(f3, "Frame3");

//	ToolBox* f3 = Manage(new ToolBox(Horizontal));
//	DBG_SET_NAME(f3, "Frame3");
//
//	Button* btn1 = Manage(new Button("Hello"));
//	DBG_SET_NAME(btn1, "Button1");
//	f3->AddWidget(btn1);
//	Button* btn2 = Manage(new Button("Hello"));
//	DBG_SET_NAME(btn2, "Button2");
//	f3->AddWidget(btn2);
//
//	fs->AddFrame(f1);
//	fs->AddFrame(f2);
//
//	FrameSplitter* vsplitter = Manage(new FrameSplitter(Vertical));
//	vsplitter->SetPosition(100, 100);
//	vsplitter->Resize(1000, 700);
//
//	vsplitter->AddFrame(f3);
//	vsplitter->AddFrame(fs);
//
//	context->AddFrame(vsplitter);
//
//	vsplitter->SetPosition(20, 20);
//	vsplitter->Resize(1240, 760);

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
*/

/**
 * test Foo() method
 *
 * Expected result:
 */
/*
TEST_F(FrameSplitterTest1, GetFrame1)
{
	Init ();

    GLFWwindow* win = CreateWindow("FrameSplitter - Foo1", 1280, 800);

    // TODO: add test code here
    UnitTestContext* context = Manage (new UnitTestContext);
	DBG_SET_NAME(context, "Context");
	SetContext(context);
	context->Resize(1280, 800);

	FrameSplitter* fs = Manage(new FrameSplitter(Horizontal));
	DBG_SET_NAME(fs, "FrameSplitter");
	fs->MoveTo(20, 20);
	fs->Resize(1240, 760);

	Viewport* f1 = Manage(new Viewport);
	DBG_SET_NAME(f1, "Frame1");
	ToolBox* f2 = Manage(new ToolBox);
	DBG_SET_NAME(f2, "Frame2");
	f2->Resize(240, 100);

	fs->AddFrame(f1);
	fs->AddFrame(f2);

	Frame* frame1 = fs->GetFrame(0);
	Frame* frame2 = fs->GetFrame(1);

	Frame* frame0 = fs->GetFrame(2);

	context->AddFrame(fs);

    RunLoop(win);
    Terminate();

	ASSERT_TRUE((frame1 == f1) && (frame2 == f2) && (frame0 == 0));
}
*/

/**
 * test Foo() method
 *
 * Expected result:
 */
TEST_F(FrameSplitterTest1, InsertFrame1)
{
	Init ();

    GLFWwindow* win = CreateWindow("FrameSplitter - Foo1", 1280, 800);

    // TODO: add test code here
    UnitTestContext* context = Manage (new UnitTestContext);
	DBG_SET_NAME(context, "Context");
	SetContext(context);
	context->Resize(1280, 800);

	FrameSplitter* fs = Manage(new FrameSplitter(Horizontal));
	DBG_SET_NAME(fs, "FrameSplitter");
	fs->MoveTo(20, 20);
	fs->Resize(1240, 760);

	Viewport* f1 = Manage(new Viewport);
	DBG_SET_NAME(f1, "Frame1");
	ToolBox* f2 = Manage(new ToolBox);
	DBG_SET_NAME(f2, "Frame2");
	f2->Resize(240, 100);

	fs->InsertFrame(0, f1);
	fs->InsertFrame(0, f2);

	context->AddFrame(fs);

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
