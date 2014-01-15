#include "ToggleButtonTest1.hpp"
#include <BlendInt/ToggleButton.hpp>

using namespace BlendInt;

ToggleButtonTest1::ToggleButtonTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ToggleButtonTest1::~ToggleButtonTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ToggleButtonTest1, Foo1)
{
	Window::Initialize ();
	Window::Create("ToggleButton - Foo1", 1280, 800);

    if(!Interface::Initialize()) {
        Window::Release();
        ASSERT_TRUE(false);
    }

    Interface::Instance()->Resize(1280, 800);
	
    // TODO: add test code here
    ToggleButton* btn = new ToggleButton;
    btn->SetPosition(200, 200);
    btn->Register();

    Window::Run();

    Interface::Release();
    Window::Release();

	ASSERT_TRUE(true);
}
