#include "FrameTest1.hpp"
#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/MenuButton.hpp>

#include <BlendInt/Gui/FileSelector.hpp>

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

    GLFWwindow* win = CreateWindow("Frame - Foo1", 1280, 800);

	Context* context = Manage(new Context);
	Interface::instance->SetCurrentContext(context);
	context->Resize(1280, 800);

    // TODO: add test code here
    Panel* frame = Manage (new Panel);
    //frame->SetMargin(10, 10, 10, 10);
    frame->SetPosition(100, 100);

    //Button* ref_btn = Manage (new Button);
    //ref_btn->SetPosition(200, 200);

    FileSelector* fs = Manage(new FileSelector);

    frame->Setup(fs);

    frame->SetMargin(10, 10, 4, 4);
    frame->Resize(800, 600);

    context->Append(frame);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
