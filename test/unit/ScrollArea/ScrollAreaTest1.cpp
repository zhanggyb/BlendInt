#include "ScrollAreaTest1.hpp"
#include <BlendInt/Gui/ScrollArea.hpp>

using namespace BlendInt;

ScrollAreaTest1::ScrollAreaTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ScrollAreaTest1::~ScrollAreaTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ScrollAreaTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("ScrollArea - Foo1", 640, 480);

    // TODO: add test code here

	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    ScrollArea* area = Manage(new ScrollArea);

    area->SetPosition(20, 20);
    area->Resize(500, 400);

    context->Append(area);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
