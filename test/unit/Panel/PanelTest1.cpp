#include "PanelTest1.hpp"
#include <BlendInt/Gui/Panel.hpp>
#include <Common/UnitTestContext.hpp>
#include <BlendInt/Gui/Dialog.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/HLayout.hpp>

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

	Button* btn1 = Manage(new Button("Button1"));
	Button* btn2 = Manage(new Button("Button2"));

	HLayout* layout = Manage(new HLayout);
	layout->AddWidget(btn1);
	layout->AddWidget(btn2);

	Panel* panel2 = Manage(new Panel);
	Button* btn3 = Manage(new Button("Button3"));
	btn3->MoveTo(20, 20);
	panel2->AddWidget(btn3);

	layout->AddWidget(panel2);

//	dialog->AddWidget(layout);
	p->SetLayout(layout);

	context->AddFrame(dialog);

	p->Resize(400, 300);

	DBG_PRINT_MSG("btn1: %d, %d", btn1->position().x(), btn1->position().y());

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
