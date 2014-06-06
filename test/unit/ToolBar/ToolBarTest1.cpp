#include "ToolBarTest1.hpp"
#include <BlendInt/Gui/ToolBar.hpp>

using namespace BlendInt;

ToolBarTest1::ToolBarTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ToolBarTest1::~ToolBarTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ToolBarTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("ToolBar - Foo1", 640, 480);

    // TODO: add test code here
	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);
	
    ToolBar* toolbar = Manage (new ToolBar);
    toolbar->SetPosition(100, 100);

    ToolButton* btn1 = Manage (new ToolButton);
    //btn->SetPosition(200, 200);

    ToolButton* btn2 = Manage (new ToolButton);

    ToolButton* btn3 = Manage (new ToolButton);

    ToolButton* btn4 = Manage (new ToolButton);

    ToolButton* btn5 = Manage (new ToolButton);

    ToolButton* btn6 = Manage (new ToolButton);

    ToolButton* btn7 = Manage (new ToolButton);

    ToolButton* btn8 = Manage (new ToolButton);

	toolbar->PushBack(btn1);
	toolbar->PushBack(btn2);
	toolbar->PushBack(btn3);
	toolbar->PushBack(btn4);
	toolbar->PushBack(btn5);
	toolbar->PushBack(btn6);
	toolbar->PushBack(btn7);
	toolbar->PushBack(btn8);

    //toolbar->SetPosition(100, 100);

	context->Add(toolbar);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
