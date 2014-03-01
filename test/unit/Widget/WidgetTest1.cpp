#include "WidgetTest1.hpp"
#include <BlendInt/Gui/Widget.hpp>
#include <Common/Window.hpp>

using namespace BlendInt;

WidgetTest1::WidgetTest1()
: testing::Test()
{
	// TODO: add constructor code
}

WidgetTest1::~WidgetTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(WidgetTest1, Add1)
{
    Init();
    GLFWwindow* win = CreateWindow("WidgetTest1 -- Show1");

	// TODO: add test code here
    Widget* widget1 = new Widget;
    widget1->set_name("widget1");
    widget1->SetPosition(200, 200);

    Widget* widget2 = new Widget;
    widget2->set_name("widget2");
    widget2->SetPosition(400, 200);

    widget1->Register();
    widget2->Register();

    widget1->AddChild(widget2);

    delete widget1;

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(WidgetTest1, Add2)
{
    Init();
    GLFWwindow* win = CreateWindow("WidgetTest1 -- Add2");

	// TODO: add test code here
    Widget* widget1 = new Widget;
    widget1->set_name("widget1");
    widget1->SetPosition(200, 200);
    widget1->Register();

    Widget* widget2 = new Widget;
    widget2->set_name("widget2");
    widget2->SetPosition(300, 200);
    widget2->Register();

    Widget* widget3 = new Widget;
    widget3->set_name("widget3");
    widget3->SetPosition(400, 200);
    widget3->Register();

    Widget* widget4 = new Widget;
    widget4->set_name("widget4");
    widget4->SetPosition(500, 200);
    widget4->Register();

    /*
    widget1->AddChild(widget2);
    widget2->AddChild(widget3);
    widget3->AddChild(widget4);
    widget4->AddChild(widget1);
    */

    widget2->SetParent(widget1);
    widget3->SetParent(widget2);
    widget4->SetParent(widget3);
    widget1->SetParent(widget4);

    widget1->RemoveChild(widget2);
    widget2->RemoveChild(widget3);
    widget3->RemoveChild(widget4);

    delete widget1;

    RunLoop(win);
    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(WidgetTest1, RenderTree1)
{
    Init();
    GLFWwindow* win = CreateWindow("WidgetTest1 -- RenderTree, parent-children");

	// TODO: add test code here
    Widget* widget1 = new Widget;
    widget1->set_name("widget1");
    widget1->SetPosition(200, 200);
    widget1->Register();

    Widget* widget2 = new Widget;
    widget2->set_name("widget2");
    widget2->SetPosition(300, 200);
    widget2->Register();

    Widget* widget3 = new Widget;
    widget3->set_name("widget3");
    widget3->SetPosition(400, 200);
    widget3->Register();

    Widget* widget4 = new Widget;
    widget4->set_name("widget4");
    widget4->SetPosition(500, 200);
    widget4->Register();

    widget2->SetParent(widget1);
    widget3->SetParent(widget2);
    widget4->SetParent(widget3);

    RunLoop(win);

	delete widget1;

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
