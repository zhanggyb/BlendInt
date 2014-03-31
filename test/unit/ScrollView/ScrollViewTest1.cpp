#include "ScrollViewTest1.hpp"
#include <BlendInt/Gui/ScrollView.hpp>
#include <BlendInt/Gui/ToolBar.hpp>

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
	
	ToolBar* toolbar = Manage (new ToolBar);
	toolbar->set_name("ToolBar");
	toolbar->Resize(400, 400);
	
	scrollview->SetViewport (toolbar);

	toolbar->move(50, 10);

	DBG_PRINT_MSG ("percentages: %d, %d", scrollview->GetHPercentage(), scrollview->GetVPercentage());

	RunLoop(win);

	Interface::Release();

	Terminate();

	ASSERT_TRUE(true);
}
