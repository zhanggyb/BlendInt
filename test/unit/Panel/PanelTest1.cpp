#include "PanelTest1.hpp"
#include <BlendInt/Gui/Panel.hpp>
#include <Common/UnitTestContext.hpp>
#include <BlendInt/Gui/Dialog.hpp>
#include <BlendInt/Gui/Button.hpp>

using namespace BlendInt;

PanelTest1::PanelTest1()
: testing::Test()
{
	// TODO: add constructor code
}

PanelTest1::~PanelTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(PanelTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("Panel - Foo1", 1280, 800);
    UnitTestContext* context = Manage (new UnitTestContext);
	DBG_SET_NAME(context, "Context");
	SetContext(context);
	context->Resize(1280, 800);

    // TODO: add test code here

	Panel* p = Manage(new Panel);
	p->SetRoundType(RoundAll);
	p->SetRoundRadius(10.f);
	p->MoveTo(50, 50);

	Dialog* dialog = Manage(new Dialog);
	dialog->MoveTo(100, 100);
	dialog->Resize(600, 500);

	dialog->AddWidget(p);

	Button* btn = Manage(new Button("Click to check"));
	p->AddWidget(btn);

	context->AddFrame(dialog);

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
