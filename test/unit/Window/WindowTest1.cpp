#include "WindowTest1.hpp"
#include <BlendInt/Window.hpp>
#include <BlendInt/ClockWidget.hpp>

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
    Window::Initialize();

    Window::Create("WindowTest1 -- Create1", 1280, 800);

    if(!Interface::Initialize()) {
        Window::Release();
        ASSERT_TRUE(false);
    }
    Interface::Instance()->Resize(1280, 800);

    // TODO: add test code here
    ClockWidget* m_clock = new ClockWidget;
    m_clock->SetPosition(200, 200);
    m_clock->Register();

    Window::Run ();

    Interface::Release();

    Window::Release();

	ASSERT_TRUE(true);
}
