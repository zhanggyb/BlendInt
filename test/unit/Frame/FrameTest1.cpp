#include "FrameTest1.hpp"
#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/MenuButton.hpp>

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

	Context* context = Manage(new Context);
	context->set_name("Context");
	Interface::instance->SetCurrentContext(context);

    // TODO: add test code here
    Frame* frame = Manage (new Frame);
    frame->set_name("Frame");
    //frame->SetMargin(10, 10, 10, 10);
    frame->SetPosition(200, 200);

    Button* ref_btn = Manage (new Button);
    ref_btn->set_name("Ref Button");
    ref_btn->SetPosition(400, 200);

    frame->Add(ref_btn);

    DBG_PRINT_MSG("frame layer: %d", frame->z());
    frame->SetMargin(10, 10, 4, 4);

    context->Add(frame);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
