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

#ifdef DEBUG

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
TEST_F(WidgetTest1, Pointer2)
{
    Init();
    GLFWwindow* win = CreateWindow("WidgetTest1 -- Show1", 640, 480);

	// TODO: add test code here
    Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Layout* container = Manage(new Layout);
    DBG_SET_NAME(container, "Layout");

    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");

    container->PushBackSubWidget(widget1);

    bool result1 = (container->first() == widget1) && (container->last() == widget1);

    delete widget1;

    bool result2 = (container->first() == 0) && (container->last() == 0);

    delete container;

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(result1 && result2);
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

    Layout* container = Manage(new Layout);
    DBG_SET_NAME(container, "Layout");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");

    container->PushBackSubWidget(widget1);
    container->PushBackSubWidget(widget2);

    bool result = (container->first() == widget1) && (container->last() == widget2);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    int size = container->widget_count();

    delete container;

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(result && (size == 2));
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

    Layout* container1 = Manage(new Layout);
    DBG_SET_NAME(container1, "Container1");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");

    container1->PushBackSubWidget(widget1);
    container1->PushBackSubWidget(widget2);

    Layout* container2 = Manage(new Layout);
    DBG_SET_NAME(container2, "Container2");
    container2->PushBackSubWidget(widget2);
    container2->PushBackSubWidget(widget1);

    int sum1 = container1->widget_count();
    int sum2 = container2->widget_count();

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
 * test widget_count
 *
 * Expected result:
 */
TEST_F(WidgetTest1, PushBack3)
{
    Init();
    GLFWwindow* win = CreateWindow("WidgetTest1 -- Show1", 640, 480);

	// TODO: add test code here
    Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Layout* container1 = Manage(new Layout);
    DBG_SET_NAME(container1, "Container1");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");

    container1->PushBackSubWidget(widget1);
    container1->PushBackSubWidget(widget2);

    int sum1 = container1->widget_count();
	int sum2 = 0;

	AbstractWidget* p = container1->first();
	while(p) {
		sum2++;
		p = p->next_view();
	}

#ifdef DEBUG
    container1->ListSubWidgets();
#endif

    delete container1;

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(sum1 == 2 && sum2 == 2);
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

    Layout* container = Manage(new Layout);
    DBG_SET_NAME(container, "Layout");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");

    container->InsertSubWidget(0, widget1);
    container->InsertSubWidget(0, widget2);

    int sum = container->widget_count();

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    delete container;

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(sum == 2);
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

    Layout* container = Manage(new Layout);
    DBG_SET_NAME(container, "Layout");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");

    container->InsertSubWidget(0, widget1);
    container->InsertSubWidget(1, widget2);

    int sum = container->widget_count();

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    delete container;

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(sum == 2);
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

    Layout* container = Manage(new Layout);
    DBG_SET_NAME(container, "Layout");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");

    container->InsertSubWidget(5, widget1);
    container->InsertSubWidget(5, widget2);

    int sum = container->widget_count();
	int sum2 = 0;

	AbstractWidget* p = container->first();
	while(p) {
		sum2++;
		p = p->next_view();
	}

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    delete container;

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(sum == 2 && sum2 == 2);
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

    Layout* container = Manage(new Layout);
    DBG_SET_NAME(container, "Layout");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");

    container->PushFrontSubWidget(widget1);
    container->PushFrontSubWidget(widget2);

    int sum = container->widget_count();

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    delete container;

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(sum == 2);
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

    Layout* container1 = Manage(new Layout);
    DBG_SET_NAME(container1, "Container1");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");

    container1->PushFrontSubWidget(widget1);
    container1->PushFrontSubWidget(widget2);

    Layout* container2 = Manage(new Layout);
    DBG_SET_NAME(container2, "Container2");
    container2->PushFrontSubWidget(widget2);
    container2->PushFrontSubWidget(widget1);

    int sum1 = container1->widget_count();
	int sum2 = 0;

	AbstractWidget* p = container2->first();
	while(p) {
		sum2++;
		p = p->next_view();
	}

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
TEST_F(WidgetTest1, Remove1)
{
    Init();
    GLFWwindow* win = CreateWindow("WidgetTest1 -- Show1", 640, 480);

	// TODO: add test code here
    Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Layout* container = Manage(new Layout);
    DBG_SET_NAME(container, "Layout");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");
    Widget* widget3 = Manage(new Widget);
    DBG_SET_NAME(widget3, "Widget3");

    container->PushBackSubWidget(widget1);
    container->PushBackSubWidget(widget2);
    container->PushBackSubWidget(widget3);

    int sum = container->widget_count();
	int sum2 = 0;

	AbstractWidget* p = container->first();
	while(p) {
		sum2++;
		p = p->next_view();
	}

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

	ASSERT_TRUE(sum == 3 && sum2 == 3);
}

/**
 * test Foo() method
 *
 * Expected result:
 */
TEST_F(WidgetTest1, GetWidgetAt1)
{
    Init();
    GLFWwindow* win = CreateWindow("WidgetTest1 -- Show1", 640, 480);

	// TODO: add test code here
    Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Layout* container = Manage(new Layout);
    DBG_SET_NAME(container, "Layout");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");
    Widget* widget3 = Manage(new Widget);
    DBG_SET_NAME(widget3, "Widget3");

    container->PushBackSubWidget(widget1);
    container->PushBackSubWidget(widget2);
    container->PushBackSubWidget(widget3);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    const AbstractWidget* p = container->GetWidgetAt(0);
    if(p) {
    	DBG_PRINT_MSG("get widget at 0 : %s", p->name().c_str());
    }

    p = container->GetWidgetAt(1);
    if(p) {
    	DBG_PRINT_MSG("get widget at 1 : %s", p->name().c_str());
    }

    p = container->GetWidgetAt(2);
    if(p) {
    	DBG_PRINT_MSG("get widget at 2 : %s", p->name().c_str());
    }

    p = container->GetWidgetAt(-1);
    if(p) {
    	DBG_PRINT_MSG("get widget at -1 : %s", p->name().c_str());
    }

    p = container->GetWidgetAt(3);
    if(p) {
    	DBG_PRINT_MSG("get widget at 3 : %s", p->name().c_str());
    }

    p = (*container)[1];
    if(p) {
    	DBG_PRINT_MSG("get widget at 1 : %s", p->name().c_str());
    }

    delete widget2;
    delete widget1;
    delete widget3;

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
TEST_F(WidgetTest1, MoveToFirst1)
{
    Init();
    GLFWwindow* win = CreateWindow("WidgetTest1 -- Show1", 640, 480);

	// TODO: add test code here
    Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Layout* container = Manage(new Layout);
    DBG_SET_NAME(container, "Layout");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");
    Widget* widget3 = Manage(new Widget);
    DBG_SET_NAME(widget3, "Widget3");

    container->PushBackSubWidget(widget1);
    container->PushBackSubWidget(widget2);
    container->PushBackSubWidget(widget3);

    int sum = container->widget_count();

    ASSERT_TRUE(sum == 3);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    DBG_PRINT_MSG("%s", "Move to first");

    widget1->MoveToFirst();
    sum = container->widget_count();

    ASSERT_TRUE(sum == 3);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    widget2->MoveToFirst();
    sum = container->widget_count();

    ASSERT_TRUE(sum == 3);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    widget3->MoveToFirst();

    widget3->MoveToFirst();
    sum = container->widget_count();

    ASSERT_TRUE(sum == 3);

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

/**
 * test Foo() method
 *
 * Expected result:
 */
TEST_F(WidgetTest1, MoveToLast1)
{
    Init();
    GLFWwindow* win = CreateWindow("WidgetTest1 -- Show1", 640, 480);

	// TODO: add test code here
    Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Layout* container = Manage(new Layout);
    DBG_SET_NAME(container, "Layout");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");
    Widget* widget3 = Manage(new Widget);
    DBG_SET_NAME(widget3, "Widget3");

    container->PushBackSubWidget(widget1);
    container->PushBackSubWidget(widget2);
    container->PushBackSubWidget(widget3);

    int sum = container->widget_count();

    ASSERT_TRUE(sum == 3);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    DBG_PRINT_MSG("%s", "Move to last");

    widget1->MoveToLast();
    sum = container->widget_count();

    ASSERT_TRUE(sum == 3);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    widget2->MoveToLast();
    sum = container->widget_count();

    ASSERT_TRUE(sum == 3);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    widget3->MoveToLast();

    widget3->MoveToLast();
    sum = container->widget_count();

    ASSERT_TRUE(sum == 3);

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

/**
 * test Foo() method
 *
 * Expected result:
 */
TEST_F(WidgetTest1, MoveForward1)
{
    Init();
    GLFWwindow* win = CreateWindow("WidgetTest1 -- Show1", 640, 480);

	// TODO: add test code here
    Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Layout* container = Manage(new Layout);
    DBG_SET_NAME(container, "Layout");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");
    Widget* widget3 = Manage(new Widget);
    DBG_SET_NAME(widget3, "Widget3");

    container->PushBackSubWidget(widget1);
    container->PushBackSubWidget(widget2);
    container->PushBackSubWidget(widget3);

    int sum = container->widget_count();

    ASSERT_TRUE(sum == 3);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    DBG_PRINT_MSG("%s", "Move forward");

    widget1->MoveForward();
    sum = container->widget_count();

    ASSERT_TRUE(sum == 3);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    widget1->MoveForward();
    sum = container->widget_count();

    ASSERT_TRUE(sum == 3);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    widget1->MoveForward();

    widget1->MoveForward();
    sum = container->widget_count();

    ASSERT_TRUE(sum == 3);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    //delete widget2;
    //delete widget1;
    //delete widget3;

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
TEST_F(WidgetTest1, MoveBackward1)
{
    Init();
    GLFWwindow* win = CreateWindow("WidgetTest1 -- Show1", 640, 480);

	// TODO: add test code here
    Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Layout* container = Manage(new Layout);
    DBG_SET_NAME(container, "Layout");
    Widget* widget1 = Manage(new Widget);
    DBG_SET_NAME(widget1, "Widget1");
    Widget* widget2 = Manage(new Widget);
    DBG_SET_NAME(widget2, "Widget2");
    Widget* widget3 = Manage(new Widget);
    DBG_SET_NAME(widget3, "Widget3");

    container->PushBackSubWidget(widget1);
    container->PushBackSubWidget(widget2);
    container->PushBackSubWidget(widget3);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    DBG_PRINT_MSG("%s", "Move backward");

    widget3->MoveBackward();
	
	int sum = container->widget_count();
	
	DBG_PRINT_MSG("sum: %d", sum);
	
	ASSERT_TRUE(sum == 3);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    widget3->MoveBackward();
    sum = container->widget_count();

    ASSERT_TRUE(sum == 3);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    widget3->MoveBackward();

    widget3->MoveBackward();
    sum = container->widget_count();

    ASSERT_TRUE(sum == 3);

#ifdef DEBUG
    container->ListSubWidgets();
#endif

    const AbstractWidget* widget = container->last();
	while(widget) {
		DBG_PRINT_MSG("sub widget: %s", widget->name().c_str());
		widget = widget->previous_view();
	}

    //delete widget2;
    //delete widget1;
    //delete widget3;

    delete container;

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}

#endif
