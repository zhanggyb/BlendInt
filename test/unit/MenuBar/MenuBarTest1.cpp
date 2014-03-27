#include "MenuBarTest1.hpp"
#include <BlendInt/Gui/MenuBar.hpp>

using namespace BlendInt;

MenuBarTest1::MenuBarTest1()
: testing::Test()
{
	// TODO: add constructor code
}

MenuBarTest1::~MenuBarTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(MenuBarTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("MenuBar - Foo1", 640, 480);

    // TODO: add test code here

    MenuBar* menubar = Manage(new MenuBar);
    menubar->set_name("MenuBar");
    menubar->SetPosition(100, 100);
    menubar->AddMenu("File");
    menubar->AddMenu("Edit");
    menubar->AddMenu("Help");

    MenuButton* button = 0;
    button = menubar->GetMenuButton(0);
    if(button) button->set_name("File Button");

    button = menubar->GetMenuButton(1);
    if(button) button->set_name("Edit Button");

    button = menubar->GetMenuButton(2);
    if(button) button->set_name("Help Button");

    menubar->SetPosition(200, 100);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
