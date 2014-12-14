#include "ColorSelectorTest1.hpp"
#include <BlendInt/Gui/ColorSelector.hpp>
#include <Common/UnitTestContext.hpp>

using namespace BlendInt;

ColorSelectorTest1::ColorSelectorTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ColorSelectorTest1::~ColorSelectorTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ColorSelectorTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("ColorSelector - Foo1", 640, 480);

    // TODO: add test code here
    UnitTestContext* context = Manage (new UnitTestContext);
	DBG_SET_NAME(context, "Context");
	SetContext(context);
	context->Resize(640, 480);

    ColorSelector* cs = Manage(new ColorSelector);
    cs->Resize(220, 320);
    cs->MoveTo(200, 50);

    context->AddFrame(cs);

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
