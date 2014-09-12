#include "ScrollViewTest1.hpp"
#include <BlendInt/Gui/ScrollView.hpp>
#include <BlendInt/Gui/Button.hpp>

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
    Interface::instance->SetCurrentContext(context);

	ScrollView* scrollview = Manage(new ScrollView);
	DBG_SET_NAME(scrollview, "ScrollView");
	scrollview->SetPosition(50, 50);
	scrollview->Resize(500, 400);
	
	Button* btn = Manage(new Button);
	DBG_SET_NAME(btn, "Button");
	btn->Resize(200, 200);
	
	scrollview->Setup(btn);
	
	//DBG_PRINT_MSG ("percentages: %d, %d", scrollview->GetHPercentage(), scrollview->GetVPercentage());

	context->Append(scrollview);

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
//TEST_F(ScrollViewTest1, Nest1)
//{
//	Init ();
//
//    GLFWwindow* win = CreateWindow("ScrollView - Foo1", 640, 640);
//
//	Context* context = Manage (new Context);
//    Interface::instance->SetCurrentContext(context);
//    context->Resize(640, 640);
//
//	ScrollView* scrollview1 = Manage(new ScrollView);
//	scrollview1->SetPosition(50, 50);
//	scrollview1->Resize(400, 400);
//
//	ScrollView* scrollview2 = Manage(new ScrollView);
//	scrollview2->SetPosition(100, 100);
//	scrollview2->Resize(500, 500);
//
//	ToolButton* btn = Manage (new ToolButton);
//	btn->Resize(400, 400);
//
//	scrollview1->SetViewport (btn);
//
//	scrollview2->SetViewport(scrollview1);
//
//	btn->move(50, 10);
//
//	context->Add(scrollview2);
//
//	RunLoop(win);
//
//	Interface::Release();
//
//	Terminate();
//
//	ASSERT_TRUE(true);
//}
