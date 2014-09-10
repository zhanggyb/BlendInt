#include "ToolButtonTest1.hpp"
#include <BlendInt/Gui/ToolButton.hpp>

using namespace BlendInt;

ToolButtonTest1::ToolButtonTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ToolButtonTest1::~ToolButtonTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ToolButtonTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("ToolButton - Foo1", 640, 480);

    // TODO: add test code here

	Context* context = Manage (new Context);
	DBG_SET_NAME(context, "Context");
    Interface::instance->SetCurrentContext(context);

    ToolButton* tb = Manage(new ToolButton);
    DBG_SET_NAME(tb, "ToolButton");

    tb->SetPosition(200, 200);
    tb->SetRoundType(RoundTopLeft | RoundBottomLeft);

    context->Append(tb);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
