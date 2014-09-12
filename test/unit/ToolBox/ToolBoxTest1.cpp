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

    GLFWwindow* win = CreateWindow("ToolBox - Foo1", 640, 480);

    // TODO: add test code here

	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    ToolBox* tb = Manage(new ToolBox);

    Button* btn1 = Manage(new Button("Hello World 1!"));
    Button* btn2 = Manage(new Button("Hello World 2!"));
    Button* btn3 = Manage(new Button("Hello World 3!"));

    tb->Append(btn1);
    tb->Append(btn2);
    tb->Append(btn3);

    tb->SetPosition(100, 100);
    tb->Resize(160, 300);

    //btn1->Resize(200, 40);
    //btn1->SetPosition(105, 300);

    DBG_PRINT_MSG("Button 1 position: %d %d", btn1->position().x(), btn1->position().y());
    DBG_PRINT_MSG("Button 2 position: %d %d", btn1->position().x(), btn2->position().y());
    DBG_PRINT_MSG("Button 3 position: %d %d", btn1->position().x(), btn3->position().y());

    context->Append(tb);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
