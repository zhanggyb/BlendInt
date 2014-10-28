#include "FrameTest1.hpp"
#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/Context.hpp>

using namespace BlendInt;

FrameTest1::FrameTest1()
: testing::Test()
{
	// TODO: add constructor code
}

FrameTest1::~FrameTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(FrameTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("Frame - Foo1", 640, 480);

	Context* context = Manage (new Context);
	SetContext(context);
	context->Resize(640, 480);



    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
