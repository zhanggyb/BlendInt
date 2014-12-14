#include "NumericalSliderTest1.hpp"
#include <BlendInt/Gui/NumericalSlider.hpp>
#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/Dialog.hpp>

using namespace BlendInt;

NumericalSliderTest1::NumericalSliderTest1()
: testing::Test()
{
	// TODO: add constructor code
}

NumericalSliderTest1::~NumericalSliderTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(NumericalSliderTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("NumericalSlider - Foo1", 640, 480);

    // TODO: add test code here
	Context* context = Manage (new Context);
	SetContext(context);
	context->Resize(640, 480);

	Dialog* dialog = Manage(new Dialog);

	NumericalSlider* slider = Manage(new NumericalSlider);
	slider->Resize(200, slider->size().height());
	slider->MoveTo(20, 100);
	slider->SetValue(20.0);

	dialog->AddWidget(slider);

	context->AddFrame(dialog);

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
