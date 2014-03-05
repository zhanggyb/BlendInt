#include "WindowTest1.hpp"
#include <Common/Window.hpp>
#include <BlendInt/Gui/ClockWidget.hpp>

using namespace BlendInt;

WindowTest1::WindowTest1()
: testing::Test()
{
	// TODO: add constructor code
}

WindowTest1::~WindowTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(WindowTest1, Create1)
{
    Init();

    GLFWwindow* win = CreateWindow("WindowTest1 -- Create1");

    // TODO: add test code here
    ClockWidget* m_clock = new ClockWidget;
    m_clock->SetPosition(200, 200);

    RunLoop (win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
