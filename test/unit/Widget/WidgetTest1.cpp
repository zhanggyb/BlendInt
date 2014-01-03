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
TEST_F(WidgetTest1, Show1)
{
	Init ();
	GLFWwindow* window = CreateWindow("Widget - Show1");

	// TODO: add test code here
    Widget* widget1 = new Widget;
    widget1->set_name("widget1");
    widget1->SetPosition(200, 200);

    Widget* widget2 = new Widget;
    widget2->set_name("widget2");
    widget2->SetPosition(400, 200);

    widget1->Register();
    widget2->Register();

	RunLoop(window);

	Terminate();

    std::cout << "size of char: " << sizeof (char) << std::endl;
    std::cout << "size of widget: " << sizeof (Widget) << std::endl;

	ASSERT_TRUE(true);
}
