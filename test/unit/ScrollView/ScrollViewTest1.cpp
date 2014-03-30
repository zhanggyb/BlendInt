#include "ScrollViewTest1.hpp"
#include <BlendInt/Gui/ScrollView.hpp>
#include <BlendInt/Gui/Widget.hpp>

using namespace BlendInt;

ScrollViewTest1::ScrollViewTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ScrollViewTest1::~ScrollViewTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ScrollViewTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("ScrollView - Foo1", 640, 640);

    // TODO: add test code here

	ScrollView* scrollview = Manage(new ScrollView);
	scrollview->set_name("ScrollView");
	scrollview->SetPosition(100, 100);
	scrollview->Resize(500, 500);
	
	Widget* widget = Manage (new Widget);
	widget->set_name("Widget");
	widget->Resize(400, 400);
	
	scrollview->SetViewport (widget);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
