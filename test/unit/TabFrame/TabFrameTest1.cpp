#include "TabFrameTest1.hpp"
#include <BlendInt/TabFrame.hpp>

using namespace BlendInt;

TabFrameTest1::TabFrameTest1()
: testing::Test()
{
	// TODO: add constructor code
}

TabFrameTest1::~TabFrameTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(TabFrameTest1, Foo1)
{
	Init ();
	GLFWwindow* window = CreateWindow("TabFrame - Foo1");

	// TODO: add test code here

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
