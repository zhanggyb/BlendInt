#include "FrameSplitterTest1.hpp"
#include <BlendInt/Gui/FrameSplitter.hpp>
#include <BlendInt/Gui/Context.hpp>

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
	Context* context = Manage (new Context);
	DBG_SET_NAME(context, "Context");
	SetContext(context);
	context->Resize(1280, 800);

	FrameSplitter* fs = Manage(new FrameSplitter);
	DBG_SET_NAME(fs, "FrameSplitter");
	fs->SetPosition(100, 100);
	fs->Resize(1000, 600);

	Frame* f1 = Manage(new Frame);
	DBG_SET_NAME(f1, "Frame1");
	Frame* f2 = Manage(new Frame);
	DBG_SET_NAME(f2, "Frame2");

	fs->AddFrame(f1);
	fs->AddFrame(f2);

	context->AddFrame(fs);

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
