#include "ClockTest1.hpp"
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
	Context* context = Manage (new Context);
	SetContext(context);
	context->Resize(640, 480);

	Dialog* dialog = Manage(new Dialog);
	dialog->SetPosition(100, 100);

	Clock* clock = Manage(new Clock);
	clock->Resize(200, 200);
	clock->SetPosition(25, 25);
	dialog->AddWidget(clock);

	context->AddFrame(dialog);

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
