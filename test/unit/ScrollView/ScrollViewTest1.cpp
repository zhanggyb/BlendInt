#include "ScrollViewTest1.hpp"
#include <BlendInt/Gui/ScrollView.hpp>
#include <BlendInt/Gui/ToolBar.hpp>

using namespace BlendInt;

ScrollViewTest1::ScrollViewTest1()
: testing::Test()
{

}

ScrollViewTest1::~ScrollViewTest1()
{

}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ScrollViewTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("ScrollView - Foo1", 640, 480);

	Context* context = Manage (new Context);
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);

	ScrollView* scrollview = Manage(new ScrollView);
	scrollview->set_name("ScrollView");
	scrollview->SetPosition(50, 50);
	scrollview->Resize(500, 400);
	
	ToolButton* btn = Manage(new ToolButton);
	btn->set_name("ToolButton");
	btn->Resize(200, 200);

	scrollview->SetViewport(btn);
	scrollview->CentralizeViewport();

	DBG_PRINT_MSG ("percentages: %d, %d", scrollview->GetHPercentage(), scrollview->GetVPercentage());

	context->Add(scrollview);

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
TEST_F(ScrollViewTest1, Nest1)
{
	Init ();

    GLFWwindow* win = CreateWindow("ScrollView - Foo1", 640, 640);

	Context* context = Manage (new Context);
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);
    context->Resize(640, 640);

	ScrollView* scrollview1 = Manage(new ScrollView);
	scrollview1->set_name("ScrollView1");
	scrollview1->SetPosition(50, 50);
	scrollview1->Resize(400, 400);

	ScrollView* scrollview2 = Manage(new ScrollView);
	scrollview2->set_name("ScrollView2");
	scrollview2->SetPosition(100, 100);
	scrollview2->Resize(500, 500);

	ToolButton* btn = Manage (new ToolButton);
	btn->set_name("ToolButton");
	btn->Resize(400, 400);

	scrollview1->SetViewport (btn);

	scrollview2->SetViewport(scrollview1);

	btn->move(50, 10);

	context->Add(scrollview2);

	RunLoop(win);

	Interface::Release();

	Terminate();

	ASSERT_TRUE(true);
}
