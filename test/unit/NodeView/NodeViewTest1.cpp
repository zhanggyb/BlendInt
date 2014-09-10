#include "NodeViewTest1.hpp"
#include <BlendInt/Gui/NodeView.hpp>

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
    Interface::instance->SetCurrentContext(context);

    NodeView* nv = Manage(new NodeView);

    nv->SetPosition(100, 100);

    context->Append(nv);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
