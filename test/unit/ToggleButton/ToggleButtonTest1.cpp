#include "ToggleButtonTest1.hpp"
#include <BlendInt/Gui/ToggleButton.hpp>
#include <Common/Window.hpp>

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
	Init ();
	GLFWwindow* win = CreateWindow("ToggleButton - Foo1", 640, 480);

    // TODO: add test code here
	Context* context = Manage (new Context);
	context->set_name("Context");
	Interface::instance->SetCurrentContext(context);
	
    ToggleButton* btn = Manage(new ToggleButton);
    btn->SetPosition(200, 200);

	context->Add(btn);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
