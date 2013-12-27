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
TEST_F(WidgetTest1, Foo1)
{
	Init ();
	GLFWwindow* window = CreateWindow("Widget - Foo1");

	// TODO: add test code here
    Widget* widget = new Widget;

    widget->SetPosition(200, 200);

    Interface::Instance()->Bind(widget);

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
