#include "TimeRulerTest1.hpp"
#include <Common/UnitTestContext.hpp>
#include <BlendInt/Gui/TimeRuler.hpp>
#include <BlendInt/Gui/Dialog.hpp>

using namespace BlendInt;

TimeRulerTest1::TimeRulerTest1()
: testing::Test()
{
	// TODO: add constructor code
}

TimeRulerTest1::~TimeRulerTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(TimeRulerTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("TimeRuler - Foo1", 1280, 800);

    UnitTestContext* context = Manage (new UnitTestContext);
	SetContext(context);
	context->Resize(1280, 800);
	
    // TODO: add test code here
	Dialog* dialog = Manage(new Dialog);
	dialog->Resize(600, 480);

	TimeRuler* tr = Manage(new TimeRuler);
	tr->MoveTo(50, 50);
	dialog->AddWidget(tr);

	context->AddFrame(dialog);

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
