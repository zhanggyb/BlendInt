#include "ToolBoxTest1.hpp"
#include <BlendInt/Gui/ToolBox.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/Context.hpp>
#include <Common/UnitTestContext.hpp>

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

    GLFWwindow* win = CreateWindow("ToolBox - Foo1", 1280, 800);

    // TODO: add test code here
    UnitTestContext* context = Manage (new UnitTestContext);
	DBG_SET_NAME(context, "Context");
	SetContext(context);
	context->Resize(1280, 800);

    ToolBox* tb1 = Manage(new ToolBox(Vertical));
    DBG_SET_NAME(tb1, "ToolBox1");
    tb1->MoveTo(200, 200);

//    ToolBox* tb2 = Manage(new ToolBox(Vertical));
//    tb2->SetPosition(700, 100);

    context->AddFrame(tb1);
//    context->AddFrame(tb2);

    Button* btn1 = Manage(new Button("Hello, World!"));
    DBG_SET_NAME(btn1, "Button1");
    btn1->Resize(200, 50);
    //Button* btn2 = Manage(new Button("Hello2"));
    //Button* btn3 = Manage(new Button("Hello3"));
//    Button* btn4 = Manage(new Button("Hello4"));
//    Button* btn5 = Manage(new Button("Hello5"));
//    Button* btn6 = Manage(new Button("Hello6"));

    tb1->AddWidget(btn1);
    //tb1->AddWidget(btn2);
    //tb1->AddWidget(btn3);

//    tb2->Add(btn4);
//    tb2->Add(btn5);
//    tb2->Add(btn6);

    tb1->RequestRedraw();

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
