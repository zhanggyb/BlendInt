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
TEST_F(WidgetTest1, Pointer1)
{
    Init();
    GLFWwindow* win = CreateWindow("WidgetTest1 -- Show1", 640, 480);

	// TODO: add test code here
    Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Widget* widget1 = new Widget;
    DBG_SET_NAME(widget1, "Widget1");

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
TEST_F(WidgetTest1, PushBack1)
{
    Init();
    GLFWwindow* win = CreateWindow("WidgetTest1 -- Show1", 640, 480);

	// TODO: add test code here
    Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Container* container = Manage(new Container);
    DBG_SET_NAME(container, "Container");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");

    container->PushBackSubWidgetExt(widget1);
    container->PushBackSubWidgetExt(widget2);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    int size = container->GetSubWidgetSize();

    delete container;

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(size == 2);
}

/**
 * test Foo() method
 *
 * Expected result:
 */
TEST_F(WidgetTest1, PushBack2)
{
    Init();
    GLFWwindow* win = CreateWindow("WidgetTest1 -- Show1", 640, 480);

	// TODO: add test code here
    Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Container* container1 = Manage(new Container);
    DBG_SET_NAME(container1, "Container1");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");

    container1->PushBackSubWidgetExt(widget1);
    container1->PushBackSubWidgetExt(widget2);

    Container* container2 = Manage(new Container);
    DBG_SET_NAME(container2, "Container2");
    container2->PushBackSubWidgetExt(widget2);
    container2->PushBackSubWidgetExt(widget1);

    int sum1 = container1->GetSubWidgetSize();
    int sum2 = container2->GetSubWidgetSize();

#ifdef DEBUG
    container1->ListSubWidgets();
    container2->ListSubWidgets();
#endif

    delete container1;
    delete container2;

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(sum1 == 0 && sum2 == 2);
}

/**
 * test Foo() method
 *
 * Expected result:
 */
TEST_F(WidgetTest1, Insert1)
{
    Init();
    GLFWwindow* win = CreateWindow("WidgetTest1 -- Show1", 640, 480);

	// TODO: add test code here
    Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Container* container = Manage(new Container);
    DBG_SET_NAME(container, "Container");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");

    container->InsertSubWidgetExt(0, widget1);
    container->InsertSubWidgetExt(0, widget2);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    delete container;

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
TEST_F(WidgetTest1, Insert2)
{
    Init();
    GLFWwindow* win = CreateWindow("WidgetTest1 -- Show1", 640, 480);

	// TODO: add test code here
    Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Container* container = Manage(new Container);
    DBG_SET_NAME(container, "Container");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");

    container->InsertSubWidgetExt(0, widget1);
    container->InsertSubWidgetExt(1, widget2);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    delete container;

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
TEST_F(WidgetTest1, Insert3)
{
    Init();
    GLFWwindow* win = CreateWindow("WidgetTest1 -- Show1", 640, 480);

	// TODO: add test code here
    Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Container* container = Manage(new Container);
    DBG_SET_NAME(container, "Container");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");

    container->InsertSubWidgetExt(5, widget1);
    container->InsertSubWidgetExt(5, widget2);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    delete container;

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
TEST_F(WidgetTest1, PushFront1)
{
    Init();
    GLFWwindow* win = CreateWindow("WidgetTest1 -- Show1", 640, 480);

	// TODO: add test code here
    Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Container* container = Manage(new Container);
    DBG_SET_NAME(container, "Container");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");

    container->PushFrontSubWidgetExt(widget1);
    container->PushFrontSubWidgetExt(widget2);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    delete container;

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
TEST_F(WidgetTest1, PushFront2)
{
    Init();
    GLFWwindow* win = CreateWindow("WidgetTest1 -- Show1", 640, 480);

	// TODO: add test code here
    Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Container* container1 = Manage(new Container);
    DBG_SET_NAME(container1, "Container1");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");

    container1->PushFrontSubWidgetExt(widget1);
    container1->PushFrontSubWidgetExt(widget2);

    Container* container2 = Manage(new Container);
    DBG_SET_NAME(container2, "Container2");
    container2->PushFrontSubWidgetExt(widget2);
    container2->PushFrontSubWidgetExt(widget1);

#ifdef DEBUG
    container1->ListSubWidgets();
    container2->ListSubWidgets();
#endif

    delete container1;
    delete container2;

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
TEST_F(WidgetTest1, Remove1)
{
    Init();
    GLFWwindow* win = CreateWindow("WidgetTest1 -- Show1", 640, 480);

	// TODO: add test code here
    Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Container* container = Manage(new Container);
    DBG_SET_NAME(container, "Container");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");
    Widget* widget3 = Manage(new Widget);
    DBG_SET_NAME(widget3, "Widget3");

    container->PushBackSubWidgetExt(widget1);
    container->PushBackSubWidgetExt(widget2);
    container->PushBackSubWidgetExt(widget3);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    delete widget2;
    delete widget1;
    delete widget3;

    delete container;

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
