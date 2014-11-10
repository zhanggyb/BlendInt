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
TEST_F(FrameSplitterTest1, Foo1)
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

	Viewport* f1 = Manage(new Viewport);
	DBG_SET_NAME(f1, "Frame1");
	Viewport* f2 = Manage(new Viewport);
	DBG_SET_NAME(f2, "Frame2");

	//Viewport* f3 = Manage(new Viewport);
	//DBG_SET_NAME(f3, "Frame3");

	ToolBox* f3 = Manage(new ToolBox(Vertical));
	DBG_SET_NAME(f3, "Frame3");

	Button* btn1 = Manage(new Button("Hello"));
	f3->AddWidget(btn1);

	fs->AddFrame(f1);
	fs->AddFrame(f2);

	FrameSplitter* vsplitter = Manage(new FrameSplitter(Horizontal));
	vsplitter->SetPosition(100, 100);
	vsplitter->Resize(1000, 700);

	vsplitter->AddFrame(fs);
	vsplitter->AddFrame(f3);

	context->AddFrame(vsplitter);

	vsplitter->SetPosition(20, 20);
	vsplitter->Resize(1240, 760);

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
