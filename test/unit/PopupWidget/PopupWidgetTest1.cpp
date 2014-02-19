#include "PopupWidgetTest1.hpp"
#include <BlendInt/Gui/PopupWidget.hpp>

using namespace BlendInt;

PopupWidgetTest1::PopupWidgetTest1()
: testing::Test()
{
	// TODO: add constructor code
}

PopupWidgetTest1::~PopupWidgetTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(PopupWidgetTest1, Foo1)
{
	Init ();
	GLFWwindow* window = CreateWindow("PopupWidget - Foo1");

	// TODO: add test code here

	PopupWidget* popup = new PopupWidget;
	
	popup->SetPosition(200, 200);
	popup->Resize(200, 400);

    popup->Register();
	
	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
