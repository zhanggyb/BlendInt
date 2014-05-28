#include "ColorWheelTest1.hpp"
#include <BlendInt/Gui/ColorWheel.hpp>

using namespace BlendInt;

ColorWheelTest1::ColorWheelTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ColorWheelTest1::~ColorWheelTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ColorWheelTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("ColorWheel - Foo1", 640, 480);

	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    ColorWheel* wheel = Manage(new ColorWheel);

    wheel->SetPosition(100, 100);
    //wheel->Resize(200, 200);

    context->Add(wheel);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
