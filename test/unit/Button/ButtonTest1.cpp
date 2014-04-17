#include "ButtonTest1.hpp"
#include <BlendInt/Gui/Button.hpp>
#include <Common/Window.hpp>
#include <BlendInt/Gui/HLayout.hpp>

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
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);

    Button* bt1 = new Button;
    bt1->SetPosition(200, 200);
    bt1->SetText("Hello");
    bt1->set_name("Button 1");

    context->Add(bt1);

    RunLoop(win);

	delete bt1;

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}

