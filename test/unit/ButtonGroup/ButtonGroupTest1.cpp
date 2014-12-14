#include "ButtonGroupTest1.hpp"
#include <BlendInt/Gui/ButtonGroup.hpp>
#include <BlendInt/Gui/TabButton.hpp>
#include <BlendInt/Gui/ToggleButton.hpp>
#include <BlendInt/Gui/MenuButton.hpp>
#include <BlendInt/Gui/HBlockLayout.hpp>

using namespace BlendInt;

ButtonGroupTest1::ButtonGroupTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ButtonGroupTest1::~ButtonGroupTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ButtonGroupTest1, ToggleGroup1)
{
	Init ();

    GLFWwindow* win = CreateWindow("ButtonGroup - ToggleGroup1", 640, 480);

	Context* context = Manage(new Context);
	Interface::instance->SetCurrentContext(context);

    // TODO: add test code here
	HBlockLayout* hblock = Manage(new HBlockLayout);

	ToggleButton* btn1 = Manage(new ToggleButton("Toggle1"));
	DBG_SET_NAME(btn1, "Toggle Button1");
	//btn1->SetDown(true);
	ToggleButton* btn2 = Manage(new ToggleButton("Toggle2"));
	DBG_SET_NAME(btn2, "Toggle Button2");
	ToggleButton* btn3 = Manage(new ToggleButton("Toggle3"));
	DBG_SET_NAME(btn3, "Toggle Button3");

	hblock->AddButton(btn1);
	hblock->AddButton(btn2);
	hblock->AddButton(btn3);

    ButtonGroup btngroup;
    btngroup.AddButton(btn1);
    btngroup.AddButton(btn2);
    btngroup.AddButton(btn3);

    hblock->MoveTo(200, 200);
    hblock->Resize(hblock->GetPreferredSize());

    btn1->SetChecked(true);

    btngroup.Clear();

    context->AddButton(hblock);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}

/*
TEST_F(ButtonGroupTest1, MenuGroup1)
{
	Init ();

    GLFWwindow* win = CreateWindow("ButtonGroup - MenuGroup1", 640, 480);

	Context* context = Manage(new Context);
	Interface::instance->SetCurrentContext(context);

    // TODO: add test code here
	MenuButton* btn1 = Manage(new MenuButton("Menu1"));
	btn1->SetPosition(100, 100);
	//btn1->SetDown(true);
	MenuButton* btn2 = Manage(new MenuButton("Menu2"));
	btn2->SetPosition(190, 100);
	MenuButton* btn3 = Manage(new MenuButton("Menu3"));
	btn3->SetPosition(280, 100);

    ButtonGroup btngroup;
    btngroup.Add(btn1);
    btngroup.Add(btn2);
    btngroup.Add(btn3);

    context->PushBack(btn1);
    context->PushBack(btn2);
    context->PushBack(btn3);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
*/
