#include "ToolBoxTest1.hpp"
#include <BlendInt/Gui/ToolBox.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/Context.hpp>

using namespace BlendInt;

ToolBoxTest1::ToolBoxTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ToolBoxTest1::~ToolBoxTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ToolBoxTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("ToolBox - Foo1", 640, 480);

    // TODO: add test code here
	Context* context = Manage (new Context);
	SetContext(context);
	context->Resize(640, 480);

    ToolBoxExt* tb = Manage(new ToolBoxExt);

    tb->SetPosition(200, 200);
    tb->Resize(160, 300);

    context->AddFrame(tb);

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
