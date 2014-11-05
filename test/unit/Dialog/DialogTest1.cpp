#include "DialogTest1.hpp"
#include <BlendInt/Gui/Dialog.hpp>

using namespace BlendInt;

DialogTest1::DialogTest1()
: testing::Test()
{
	// TODO: add constructor code
}

DialogTest1::~DialogTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(DialogTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("Dialog - Foo1", 640, 480);

    // TODO: add test code here
	Context* context = Manage (new Context);
	SetContext(context);
	context->Resize(640, 480);

	Dialog* dialog = Manage(new Dialog);
	context->AddFrame(dialog);

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
