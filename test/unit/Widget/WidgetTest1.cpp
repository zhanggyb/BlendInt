#include "WidgetTest1.hpp"
#include <BlendInt/Widget.hpp>

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
TEST_F(WidgetTest1, Insert1)
{
    Window::Initialize();
    Window::Create("WidgetTest1 -- Show1", 1280, 800);

    if(!Interface::Initialize()) {
        Window::Release();
        ASSERT_TRUE(false);
    }
    Interface::Instance()->Resize(1280, 800);

	// TODO: add test code here
    Widget* widget1 = new Widget;
    widget1->set_name("widget1");
    widget1->SetPosition(200, 200);

    Widget* widget2 = new Widget;
    widget2->set_name("widget2");
    widget2->SetPosition(400, 200);

    widget1->Register();
    widget2->Register();

    widget1->Insert(widget2);

    delete widget1;

    Window::Run();
    Interface::Release();
    Window::Release();

	ASSERT_TRUE(true);
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(WidgetTest1, Insert2)
{
    Window::Initialize();
    Window::Create("WidgetTest1 -- Insert2", 1280, 800);

    if(!Interface::Initialize()) {
        Window::Release();
        ASSERT_TRUE(false);
    }
    Interface::Instance()->Resize(1280, 800);

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
    widget1->Insert(widget2);
    widget2->Insert(widget3);
    widget3->Insert(widget4);
    widget4->Insert(widget1);
    */

    widget2->InsertedTo(widget1);
    widget3->InsertedTo(widget2);
    widget4->InsertedTo(widget3);
    widget1->InsertedTo(widget4);

    widget1->Remove(widget2);
    widget2->Remove(widget3);
    widget3->Remove(widget4);

    delete widget1;

    Window::Run();
    Interface::Release();
    Window::Release();

	ASSERT_TRUE(true);
}
