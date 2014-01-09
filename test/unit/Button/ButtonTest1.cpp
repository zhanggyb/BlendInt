#include "ButtonTest1.hpp"
#include <BlendInt/Button.hpp>

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
	Init ();
	GLFWwindow* window = CreateWindow("Button - Foo1");

	// TODO: add test code here

    Button* button = new Button;
    button->SetPosition(200, 200);

    button->Register();
    button->show();

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
