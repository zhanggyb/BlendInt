#include "ViewportTest1.hpp"
#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/Viewport.hpp>

using namespace BlendInt;

ViewportTest1::ViewportTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ViewportTest1::~ViewportTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ViewportTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("Viewport - Foo1", 1280, 800);

    // TODO: add test code here
	Context* context = Manage (new Context);
	SetContext(context);
	context->Resize(1280, 800);

	Viewport* vp = Manage(new Viewport);
	vp->MoveTo(100, 100);

	context->AddFrame(vp);

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
