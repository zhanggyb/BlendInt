#include "ClockTest1.hpp"

#include <Common/UnitTestContext.hpp>

#include <BlendInt/Gui/Clock.hpp>
#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/Dialog.hpp>

using namespace BlendInt;

ClockTest1::ClockTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ClockTest1::~ClockTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ClockTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("Clock - Foo1", 640, 480);

    // TODO: add test code here
    UnitTestContext* context = Manage (new UnitTestContext);
	SetContext(context);
	context->Resize(640, 480);

	Dialog* dialog = Manage(new Dialog);
	dialog->MoveTo(100, 100);

	Clock* clock = Manage(new Clock);
	clock->Resize(200, 200);
	clock->MoveTo(25, 25);
	dialog->AddWidget(clock);

	context->AddFrame(dialog);

	clock->Start();

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
