#include "ToolBoxTest1.hpp"
#include <BlendInt/Gui/ToolBox.hpp>

#include <BlendInt/Gui/Button.hpp>

using namespace BlendInt;

ToolBoxTest1::ToolBoxTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ToolBoxTest1::~ToolBoxTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ToolBoxTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("ToolBox - Foo1");

    // TODO: add test code here

	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    ToolBox* tb = Manage(new ToolBox);
    tb->SetPosition(200, 200);

    Button* btn1 = Manage(new Button("Hello World 1!"));
    Button* btn2 = Manage(new Button("Hello World 2!"));
    Button* btn3 = Manage(new Button("Hello World 3!"));

    tb->PushBack(btn1);
    tb->PushBack(btn2);
    tb->PushBack(btn3);

    DBG_PRINT_MSG("Button 1 position: %d %d", btn1->position().x(), btn1->position().y());

    context->PushBack(tb);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
