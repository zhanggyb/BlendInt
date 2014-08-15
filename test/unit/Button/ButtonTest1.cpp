#include "ButtonTest1.hpp"
#include <BlendInt/Gui/Button.hpp>
#include <Common/Window.hpp>

#include <BlendInt/Stock/Icons.hpp>

using namespace BlendInt;

ButtonTest1::ButtonTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ButtonTest1::~ButtonTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
/*
TEST_F(ButtonTest1, Foo1)
{
    Init();
    GLFWwindow* win = CreateWindow("Button Test - Foo1", 640, 480);

	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Button* bt1 = new Button;
    DBG_SET_NAME(bt1, "Button1");
    bt1->SetPosition(100, 200);
    bt1->SetText(String("Button1"));

    Button* bt2 = new Button("Button2");
    DBG_SET_NAME(bt2, "Button2");
    bt2->SetPosition(240, 200);

    context->PushBack(bt1);
    context->PushBack(bt2);

    RunLoop(win);

	delete bt1;
	delete bt2;

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
*/

/**
 * test Foo() method
 *
 * Expected result:
 */
TEST_F(ButtonTest1, SetIcon1)
{
    Init();
    GLFWwindow* win = CreateWindow("Button Test - Foo1", 640, 480);

	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Button* bt1 = Manage(new Button(BlendInt::Stock::Icons::instance->image_rgb_16x16(), "Hello World!"));
    DBG_SET_NAME(bt1, "Button1");
    //bt1->SetIcon(BlendInt::Stock::Icons::instance->image_rgb_16x16());
    bt1->SetPosition(200, 200);

    Size prefer = bt1->GetPreferredSize();

    DBG_PRINT_MSG("Preferred size: %d, %d", prefer.width(), prefer.height());

    bt1->Resize(120, 40);

    context->PushBack(bt1);

    Button* bt2 = Manage(new Button("Hello World!"));
    DBG_SET_NAME(bt2, "Button2");
    //bt1->SetIcon(BlendInt::Stock::Icons::instance->image_rgb_16x16());
    bt2->SetPosition(400, 200);

    bt2->Resize(120, 40);

    context->PushBack(bt2);

    Button* bt3 = Manage(new Button(BlendInt::Stock::Icons::instance->image_rgb_16x16(), "Hello World!"));
    DBG_SET_NAME(bt3, "Button3");
    bt3->SetPosition(200, 100);
    bt3->Resize(120, 40);
    bt3->SetRoundType(RoundNone);

    context->PushBack(bt3);

    Button* bt4 = Manage(new Button(BlendInt::Stock::Icons::instance->image_rgb_16x16(), "Hello World!"));
    DBG_SET_NAME(bt4, "Button4");
    bt4->SetPosition(400, 100);
    bt4->Resize(120, 40);
    bt4->SetRoundRadius(10.f);

    context->PushBack(bt4);

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
TEST_F(ButtonTest1, PreferSizing1)
{
    Init();
    GLFWwindow* win = CreateWindow("Button Test - Foo1", 640, 480);

	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Button* bt1 = Manage(new Button(BlendInt::Stock::Icons::instance->image_rgb_16x16(), "Hello World!"));
    DBG_SET_NAME(bt1, "Button1");
    //bt1->SetIcon(BlendInt::Stock::Icons::instance->image_rgb_16x16());
    bt1->SetPosition(200, 200);

    Size prefer = bt1->GetPreferredSize();

    DBG_PRINT_MSG("Preferred size: %d, %d", prefer.width(), prefer.height());

    bt1->Resize(bt1->GetPreferredSize());

    context->PushBack(bt1);

    Button* bt2 = Manage(new Button("Hello World!"));
    DBG_SET_NAME(bt2, "Button2");
    //bt1->SetIcon(BlendInt::Stock::Icons::instance->image_rgb_16x16());
    bt2->SetPosition(400, 200);

    bt2->Resize(bt2->GetPreferredSize());

    context->PushBack(bt2);

    Button* bt3 = Manage(new Button(BlendInt::Stock::Icons::instance->image_rgb_16x16(), "Hello World!"));
    DBG_SET_NAME(bt3, "Button3");
    bt3->SetPosition(200, 100);
    bt3->Resize(bt3->GetPreferredSize());
    bt3->SetRoundType(RoundNone);

    context->PushBack(bt3);

    Button* bt4 = Manage(new Button(BlendInt::Stock::Icons::instance->image_rgb_16x16(), "Hello World!"));
    DBG_SET_NAME(bt4, "Button4");
    bt4->SetPosition(400, 100);
    bt4->Resize(bt4->GetPreferredSize());
    bt4->SetRoundRadius(10.f);

    context->PushBack(bt4);

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
TEST_F(ButtonTest1, PreferSizing2)
{
    Init();
    GLFWwindow* win = CreateWindow("Button Test - Foo1", 640, 480);

	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    Button* bt1 = Manage(new Button(BlendInt::Stock::Icons::instance->image_rgb_16x16(), String()));
    DBG_SET_NAME(bt1, "Button1");
    //bt1->SetIcon(BlendInt::Stock::Icons::instance->image_rgb_16x16());
    bt1->SetPosition(200, 200);

    Size prefer = bt1->GetPreferredSize();

    DBG_PRINT_MSG("Preferred size: %d, %d", prefer.width(), prefer.height());

    bt1->Resize(bt1->GetPreferredSize());

    context->PushBack(bt1);

    Button* bt2 = Manage(new Button(BlendInt::Stock::Icons::instance->image_rgb_16x16(), String()));
    DBG_SET_NAME(bt2, "Button2");
    bt2->SetPosition(300, 200);
    bt2->SetRoundType(RoundNone);

    Size prefer2 = bt2->GetPreferredSize();

    DBG_PRINT_MSG("Preferred size: %d, %d", prefer2.width(), prefer2.height());

    bt2->Resize(bt2->GetPreferredSize());

    context->PushBack(bt2);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}

