#include "StackTest1.hpp"
#include <BlendInt/Gui/Stack.hpp>

using namespace BlendInt;

StackTest1::StackTest1()
: testing::Test()
{
	// TODO: add constructor code
}

StackTest1::~StackTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(StackTest1, Foo1)
{
	Init ();
	GLFWwindow* window = CreateWindow("Stack - Foo1", 640, 480);

	// TODO: add test code here

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
