#include "MenuBarTest1.hpp"
#include <BlendInt/Gui/MenuBar.hpp>
#include <BlendInt/Service/StockItems.hpp>

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
    Context* context = Manage (new Context);
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);

    MenuBar* menubar = Manage(new MenuBar);
    menubar->set_name("MenuBar");
    menubar->SetPosition(100, 100);
    menubar->AddMenu("File");
    menubar->AddMenu("Edit");
    menubar->AddMenu("Help");

    RefPtr<Menu> menu(new Menu);
    menu->set_name("Menu");

    menu->SetRoundType(RoundBottomLeft | RoundBottomRight);
    //m_menu->SetPosition(200, 200);
    //menu->Resize (200, 200);

    menu->AddActionItem(StockItems::instance->icon_check(), "MenuItem1", "Ctrl + 1");
    menu->AddActionItem("MenuItem2", "Ctrl + 1");
    menu->AddActionItem("MenuItem3", "Ctrl + 1");
    menu->AddActionItem("MenuItem4", "Ctrl + 1");
    menu->AddActionItem("MenuItem5");

    MenuButton* button = 0;
    button = menubar->GetMenuButton(0);
    if(button) {
		button->set_name("File Button");
	    button->SetMenu(menu);
	}

    button = menubar->GetMenuButton(1);
    if(button) {
		button->set_name("Edit Button");
	    button->SetMenu(menu);
	}

    button = menubar->GetMenuButton(2);
    if(button) {
		button->set_name("Help Button");
	    button->SetMenu(menu);
	}
	
    menubar->SetPosition(200, 400);
	context->Add(menubar);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
