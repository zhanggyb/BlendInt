#include "PopupWidgetTest1.hpp"
#include <BlendInt/Gui/PopupFrame.hpp>
#include <BlendInt/Gui/Widget.hpp>

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
	GLFWwindow* window = CreateWindow("PopupWidget - Foo1", 640, 480);

	// TODO: add test code here

	PopupFrame* popup = Manage(new PopupFrame);
	
	Widget* widget = Manage(new Widget);

	popup->Add(widget);

	popup->SetPosition(100, 100);
	popup->Resize(200, 200);
	
	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
