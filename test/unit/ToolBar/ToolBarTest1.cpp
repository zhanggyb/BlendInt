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
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);
	
    ToolBar* toolbar = Manage (new ToolBar);
    toolbar->set_name("ToolBar");

    ToolButton* btn1 = Manage (new ToolButton);
    btn1->set_name("ToolButton1");
    //btn->SetPosition(200, 200);

    ToolButton* btn2 = Manage (new ToolButton);
    btn2->set_name("ToolButton2");

    ToolButton* btn3 = Manage (new ToolButton);
    btn3->set_name("ToolButton3");

	toolbar->Add(btn1);
	toolbar->Add(btn2);
	toolbar->Add(btn3);

    toolbar->SetPosition(100, 100);

	context->Add(toolbar);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
