#include "ToolBarTest1.hpp"
#include <BlendInt/Gui/ToolBar.hpp>
#include <BlendInt/Gui/Context.hpp>

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
    // TODO: add test code here
	Context* context = Manage (new Context);
	SetContext(context);
	context->Resize(640, 480);
	
    ToolBarExt* toolbar = Manage (new ToolBarExt);
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

	toolbar->Add(btn1);
	toolbar->Add(btn2);
	toolbar->Add(btn3);
	toolbar->Add(btn4);
	toolbar->Add(btn5);
	toolbar->Add(btn6);
	toolbar->Add(btn7);
	toolbar->Add(btn8);

    //toolbar->SetPosition(100, 100);
	toolbar->Resize(toolbar->GetPreferredSize());

	context->AddFrame(toolbar);

    RunLoop(win);
    Terminate();

    ASSERT_TRUE(true);
}
