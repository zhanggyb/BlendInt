#include "ScrollBarTest1.hpp"
#include <BlendInt/Gui/ScrollBar.hpp>

using namespace BlendInt;

ScrollBarTest1::ScrollBarTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ScrollBarTest1::~ScrollBarTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ScrollBarTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("ScrollBar - Foo1", 640, 480);

    // TODO: add test code here

    	ScrollBar* bar = Manage(new ScrollBar);

    	bar->set_name("ScrollBar");

    	bar->SetPosition(200, 200);

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
TEST_F(ScrollBarTest1, Show2)
{
	Init ();

    GLFWwindow* win = CreateWindow("ScrollBar - Foo1", 640, 480);

    // TODO: add test code here

    	ScrollBar* bar = Manage(new ScrollBar(Vertical));

    	bar->set_name("ScrollBar");

    	bar->SetPosition(200, 200);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
