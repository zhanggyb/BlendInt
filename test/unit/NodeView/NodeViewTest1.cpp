#include "NodeViewTest1.hpp"
#include <BlendInt/Gui/NodeView.hpp>
#include <BlendInt/Gui/Dialog.hpp>
#include <Common/UnitTestContext.hpp>

using namespace BlendInt;

NodeViewTest1::NodeViewTest1()
: testing::Test()
{
	// TODO: add constructor code
}

NodeViewTest1::~NodeViewTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(NodeViewTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("NodeView - Foo1", 640, 480);

    // TODO: add test code here
	Context* context = Manage (new Context);
	SetContext(context);
	context->Resize(640, 480);

	Dialog* dialog = Manage(new Dialog);
	context->AddFrame(dialog);
	dialog->MoveTo(100, 100);
	dialog->Resize(500, 500);

    NodeView* nv = Manage(new NodeView);
    nv->MoveTo(20, 20);

    dialog->AddWidget(nv);

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
