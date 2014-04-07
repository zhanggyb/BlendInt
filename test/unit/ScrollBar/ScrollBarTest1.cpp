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

/**
 * test Foo() method
 *
 * Expected result:
 */
TEST_F(ScrollBarTest1, Resize01)
{
	Init ();

    GLFWwindow* win = CreateWindow("ScrollBar - Foo1", 640, 480);

    // TODO: add test code here

    	ScrollBar* bar = Manage(new ScrollBar(Horizontal));

    	bar->set_name("ScrollBar");

    	bar->SetPosition(200, 200);

    	bar->Resize(400, 20);

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
TEST_F(ScrollBarTest1, SetValue01)
{
	Init ();

    GLFWwindow* win = CreateWindow("ScrollBar - Foo1", 640, 480);

    // TODO: add test code here

    	ScrollBar* bar = Manage(new ScrollBar(Horizontal));

    	bar->set_name("ScrollBar");

    	bar->SetPosition(200, 200);

    	bar->SetValue(50);

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
TEST_F(ScrollBarTest1, SetPercentage01)
{
	Init ();

    GLFWwindow* win = CreateWindow("ScrollBar - SetPercentage01", 640, 480);

    // TODO: add test code here

    ScrollBar* bar = Manage(new ScrollBar(Horizontal));

    bar->set_name("ScrollBar");

    bar->SetPosition(200, 200);

    bar->SetPercentage(50);

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
TEST_F(ScrollBarTest1, SetMinimum01)
{
	Init ();

    GLFWwindow* win = CreateWindow("ScrollBar - SetMinimum01", 640, 480);

    // TODO: add test code here

    ScrollBar* bar = Manage(new ScrollBar(Horizontal));

    bar->set_name("ScrollBar");

    bar->SetPosition(200, 200);
    bar->SetMinimum(20);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
