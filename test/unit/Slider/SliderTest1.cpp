#include "SliderTest1.hpp"
#include <BlendInt/Gui/Slider.hpp>

using namespace BlendInt;

SliderTest1::SliderTest1()
: testing::Test()
{
	// TODO: add constructor code
}

SliderTest1::~SliderTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(SliderTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("Slider - Foo1", 640, 480);

    // TODO: add test code here
	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);
	
    Slider* slider = Manage(new Slider);
    slider->SetPosition(200, 200);

	context->Append(slider);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
