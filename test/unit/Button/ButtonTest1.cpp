#include "ButtonTest1.hpp"
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/Frame.hpp>
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
TEST_F(ButtonTest1, Foo1)
{
    Init();
    GLFWwindow* win = CreateWindow("Button Test - Foo1", 640, 480);

	Context* context = Manage (new Context);
	SetContext(context);
	context->Resize(640, 480);

	Frame* frame = Manage(new Frame);
	frame->Resize(640, 480);

	context->AddFrame(frame);

    Button* bt1 = Manage(new Button(BlendInt::Stock::Icons::instance->icon_16x16(0), "Hello World!"));
    DBG_SET_NAME(bt1, "Button1");
    bt1->SetPosition(200, 200);
    bt1->SetText(String("Button1"));
    bt1->Resize(bt1->GetPreferredSize());

    frame->AddWidget(bt1);

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}

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
	SetContext(context);
	context->Resize(640, 480);

	Frame* frame = Manage(new Frame);
	frame->Resize(640, 480);

	context->AddFrame(frame);

    Button* bt1 = Manage(new Button(BlendInt::Stock::Icons::instance->icon_16x16(0), "Hello World!"));
    DBG_SET_NAME(bt1, "Button1");
    //bt1->SetIcon(BlendInt::Stock::Icons::instance->icon_16x16(0));
    bt1->SetPosition(200, 200);

    Size prefer = bt1->GetPreferredSize();

    DBG_PRINT_MSG("Preferred size: %d, %d", prefer.width(), prefer.height());

    bt1->Resize(120, 40);

    frame->AddWidget(bt1);

    Button* bt2 = Manage(new Button("Hello World!"));
    DBG_SET_NAME(bt2, "Button2");
    //bt1->SetIcon(BlendInt::Stock::Icons::instance->icon_16x16(0));
    bt2->SetPosition(400, 200);

    bt2->Resize(120, 40);

    frame->AddWidget(bt2);

    Button* bt3 = Manage(new Button(BlendInt::Stock::Icons::instance->icon_16x16(0), "Hello World!"));
    DBG_SET_NAME(bt3, "Button3");
    bt3->SetPosition(200, 100);
    bt3->Resize(120, 40);
    bt3->SetRoundType(RoundNone);

    frame->AddWidget(bt3);

    Button* bt4 = Manage(new Button(BlendInt::Stock::Icons::instance->icon_16x16(0), "Hello World!"));
    DBG_SET_NAME(bt4, "Button4");
    bt4->SetPosition(400, 100);
    bt4->Resize(120, 40);
    bt4->SetRoundRadius(10.f);

    frame->AddWidget(bt4);

    RunLoop(win);
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
	SetContext(context);
	context->Resize(640, 480);

	Frame* frame = Manage(new Frame);
	frame->Resize(640, 480);

	context->AddFrame(frame);

    Button* bt1 = Manage(new Button(BlendInt::Stock::Icons::instance->icon_16x16(0), "Hello World!"));
    DBG_SET_NAME(bt1, "Button1");
    //bt1->SetIcon(BlendInt::Stock::Icons::instance->icon_16x16(0));
    bt1->SetPosition(200, 200);

    Size prefer = bt1->GetPreferredSize();

    DBG_PRINT_MSG("Preferred size: %d, %d", prefer.width(), prefer.height());

    bt1->Resize(bt1->GetPreferredSize());

    frame->AddWidget(bt1);

    Button* bt2 = Manage(new Button("Hello World!"));
    DBG_SET_NAME(bt2, "Button2");
    //bt1->SetIcon(BlendInt::Stock::Icons::instance->icon_16x16(0));
    bt2->SetPosition(400, 200);

    bt2->Resize(bt2->GetPreferredSize());

    frame->AddWidget(bt2);

    Button* bt3 = Manage(new Button(BlendInt::Stock::Icons::instance->icon_16x16(0), "Hello World!"));
    DBG_SET_NAME(bt3, "Button3");
    bt3->SetPosition(200, 100);
    bt3->Resize(bt3->GetPreferredSize());
    bt3->SetRoundType(RoundNone);

    frame->AddWidget(bt3);

    Button* bt4 = Manage(new Button(BlendInt::Stock::Icons::instance->icon_16x16(0), "Hello World!"));
    DBG_SET_NAME(bt4, "Button4");
    bt4->SetPosition(400, 100);
    bt4->Resize(bt4->GetPreferredSize());
    bt4->SetRoundRadius(10.f);

    frame->AddWidget(bt4);

    RunLoop(win);
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
	SetContext(context);
	context->Resize(640, 480);

	Frame* frame = Manage(new Frame);
	frame->Resize(640, 480);

	context->AddFrame(frame);

    Button* bt1 = Manage(new Button(BlendInt::Stock::Icons::instance->icon_16x16(0), String()));
    DBG_SET_NAME(bt1, "Button1");
    //bt1->SetIcon(BlendInt::Stock::Icons::instance->icon_16x16(0));
    bt1->SetPosition(200, 200);

    Size prefer = bt1->GetPreferredSize();

    DBG_PRINT_MSG("Preferred size: %d, %d", prefer.width(), prefer.height());

    bt1->Resize(bt1->GetPreferredSize());

    frame->AddWidget(bt1);

    Button* bt2 = Manage(new Button(BlendInt::Stock::Icons::instance->icon_16x16(0), String()));
    DBG_SET_NAME(bt2, "Button2");
    bt2->SetPosition(300, 200);
    bt2->SetRoundType(RoundNone);

    Size prefer2 = bt2->GetPreferredSize();

    DBG_PRINT_MSG("Preferred size: %d, %d", prefer2.width(), prefer2.height());

    bt2->Resize(bt2->GetPreferredSize());

    frame->AddWidget(bt2);

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}

