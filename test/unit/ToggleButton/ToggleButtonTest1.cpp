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
	
    ToggleButton* btn1 = Manage(new ToggleButton);
    btn1->set_name("ToggleButton");
    btn1->SetPosition(160, 200);

    ToggleButton* btn2 = Manage(new ToggleButton);
    btn2->set_name("ToggleButton");
    btn2->SetPosition(300, 200);

	context->Add(btn1);
	context->Add(btn2);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
