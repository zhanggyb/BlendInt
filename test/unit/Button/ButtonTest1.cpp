#include "ButtonTest1.hpp"
#include <BlendInt/Gui/Button.hpp>
#include <Common/Window.hpp>

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
    Interface::instance->SetCurrentContext(context);

    Button* bt1 = new Button;
    bt1->SetPosition(100, 200);
    bt1->SetText(String("Button1"));

    Button* bt2 = new Button("Button2");
    bt2->SetPosition(240, 200);

    context->Add(bt1);
    context->Add(bt2);

    RunLoop(win);

	delete bt1;
	delete bt2;

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}

