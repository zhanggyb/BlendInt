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
TEST_F(WidgetTest1, Add1)
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

    widget1->AddChild(widget2);

    Object::Destroy(widget1);

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
TEST_F(WidgetTest1, Add2)
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

    Object::Destroy(widget1);

    Window::Run();
    Interface::Release();
    Window::Release();

	ASSERT_TRUE(true);
}
