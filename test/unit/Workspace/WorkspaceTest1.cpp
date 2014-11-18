#include "WorkspaceTest1.hpp"
#include <Common/UnitTestContext.hpp>
#include <BlendInt/Gui/Workspace.hpp>
#include <BlendInt/Gui/Viewport.hpp>
#include <BlendInt/Gui/Button.hpp>

using namespace BlendInt;

WorkspaceTest1::WorkspaceTest1()
: testing::Test()
{
	// TODO: add constructor code
}

WorkspaceTest1::~WorkspaceTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(WorkspaceTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("Workspace - Foo1", 1280, 800);

    UnitTestContext* context = Manage (new UnitTestContext);
	SetContext(context);
	context->Resize(1280, 800);
	
    // TODO: add test code here

	Workspace* workspace = Manage(new Workspace);
	workspace->Resize(1280, 800);

	ToolBox* left = Manage(new ToolBox(Vertical));
	Button* btn1 = Manage(new Button("Hello1"));
	Button* btn2 = Manage(new Button("Hello2"));
	Button* btn3 = Manage(new Button("Hello3"));
	left->AddWidget(btn1);
	left->AddWidget(btn2);
	left->AddWidget(btn3);

	ToolBox* right = Manage(new ToolBox(Vertical));
	Button* btn4 = Manage(new Button("Hello4"));
	Button* btn5 = Manage(new Button("Hello5"));
	Button* btn6 = Manage(new Button("Hello6"));
	right->AddWidget(btn4);
	right->AddWidget(btn5);
	right->AddWidget(btn6);

	Viewport* view = Manage(new Viewport);

	workspace->SetLeftSideBar(left);
	workspace->SetViewport(view);
	workspace->SetRightSideBar(right);

	context->AddFrame(workspace);

	context->LinkResizeEvent(workspace);
    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
