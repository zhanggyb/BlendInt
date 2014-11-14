#include "PopupFrameTest1.hpp"
#include <BlendInt/Gui/PopupFrame.hpp>
#include <Common/UnitTestContext.hpp>

using namespace BlendInt;

PopupFrameTest1::PopupFrameTest1()
: testing::Test()
{
	// TODO: add constructor code
}

PopupFrameTest1::~PopupFrameTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(PopupFrameTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("PopupFrame - Foo1", 640, 480);

    // TODO: add test code here
    UnitTestContext* context = Manage (new UnitTestContext);
	DBG_SET_NAME(context, "Context");
	SetContext(context);
	context->Resize(640, 480);

	PopupFrame* frame = Manage(new PopupFrame);
	frame->Resize(300, 200);
	frame->MoveTo(200, 200);
	context->AddFrame(frame);

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
