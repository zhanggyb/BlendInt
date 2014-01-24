#include "ButtonTest1.hpp"
#include <BlendInt/Button.hpp>
#include <Common/Window.hpp>
#include <BlendInt/HorizontalLayout.hpp>

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
    GLFWwindow* win = CreateWindow("Button Test - Foo1");

	// TODO: add test code here
    HorizontalLayout* hl1 = new HorizontalLayout;
    hl1->SetPosition(300, 400);
    hl1->set_name("Hlayout1");

    Button* bt1 = new Button;
    bt1->SetPosition(200, 200);
    bt1->set_name("Button 1");

    Button* bt2 = new Button;
    bt2->SetPosition(300, 200);
    bt2->set_name("Button 2");

    Button* bt3 = new Button;
    bt3->SetPosition(400, 200);
    bt3->set_name("Button 3");
    
    bt1->Register();
    bt2->Register();
    bt3->Register();

    hl1->Add(bt1);
    hl1->Add(bt2);
    hl1->Add(bt3);

    hl1->Register();

    RunLoop(win);
    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}

