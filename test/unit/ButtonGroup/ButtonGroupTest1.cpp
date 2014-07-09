#include "ButtonGroupTest1.hpp"
#include <BlendInt/Gui/ButtonGroup.hpp>
#include <BlendInt/Gui/TabButton.hpp>
#include <BlendInt/Gui/ToggleButton.hpp>
#include <BlendInt/Gui/MenuButton.hpp>

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
	/*
    TabButton* btn1 = Manage(new TabButton);
    btn1->set_name("TabButton1");
    btn1->SetPosition(100, 100);
    TabButton* btn2 = Manage(new TabButton);
    btn2->set_name("TabButton2");
    btn2->SetPosition(100 + btn1->size().width() + 2, 100);
    TabButton* btn3 = Manage(new TabButton);
    btn3->set_name("TabButton3");
    btn3->SetPosition(100 + btn1->size().width() + btn2->size().width() + 4, 100);
    */

	ToggleButton* btn1 = Manage(new ToggleButton);
	btn1->SetPosition(100, 100);
	//btn1->SetDown(true);
	ToggleButton* btn2 = Manage(new ToggleButton);
	btn2->SetPosition(190, 100);
	ToggleButton* btn3 = Manage(new ToggleButton);
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

TEST_F(ButtonGroupTest1, MenuGroup1)
{
	Init ();

    GLFWwindow* win = CreateWindow("ButtonGroup - MenuGroup1", 640, 480);

	Context* context = Manage(new Context);
	Interface::instance->SetCurrentContext(context);

    // TODO: add test code here
	/*
    TabButton* btn1 = Manage(new TabButton);
    btn1->set_name("TabButton1");
    btn1->SetPosition(100, 100);
    TabButton* btn2 = Manage(new TabButton);
    btn2->set_name("TabButton2");
    btn2->SetPosition(100 + btn1->size().width() + 2, 100);
    TabButton* btn3 = Manage(new TabButton);
    btn3->set_name("TabButton3");
    btn3->SetPosition(100 + btn1->size().width() + btn2->size().width() + 4, 100);
    */

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

