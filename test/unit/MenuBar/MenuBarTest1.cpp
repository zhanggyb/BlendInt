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

    RefPtr<Menu> menu1(new Menu);
    menu1->set_name("MenuFile");

    menu1->SetRoundType(RoundBottomLeft | RoundBottomRight);
    //m_menu->SetPosition(200, 200);
    //menu->Resize (200, 200);

    menu1->AddActionItem(StockItems::instance->icon_check(), "MenuItem1", "Ctrl + 1");
    menu1->AddActionItem("MenuItem2", "Ctrl + 1");
    menu1->AddActionItem("MenuItem3", "Ctrl + 1");
    menu1->AddActionItem("MenuItem4", "Ctrl + 1");

    RefPtr<Menu> menu2(new Menu);
    menu2->set_name("MenuEdit");

    menu2->SetRoundType(RoundBottomLeft | RoundBottomRight);

    menu2->AddActionItem(StockItems::instance->icon_check(), "MenuItem1", "Ctrl + 1");
    menu2->AddActionItem("MenuItem2", "Ctrl + 1");
    menu2->AddActionItem("MenuItem3", "Ctrl + 1");
    menu2->AddActionItem("MenuItem4", "Ctrl + 1");

    RefPtr<Menu> menu3(new Menu);
    menu3->set_name("MenuHelp");

    menu3->SetRoundType(RoundBottomLeft | RoundBottomRight);
    //m_menu->SetPosition(200, 200);
    //menu->Resize (200, 200);

    menu3->AddActionItem(StockItems::instance->icon_check(), "MenuItem1", "Ctrl + 1");
    menu3->AddActionItem("MenuItem2", "Ctrl + 1");
    menu3->AddActionItem("MenuItem3", "Ctrl + 1");
    menu3->AddActionItem("MenuItem4", "Ctrl + 1");


    menubar->AddMenuButton("File", menu1);
    menubar->AddMenuButton("Edit", menu2);
    menubar->AddMenuButton("Help", menu3);

    menubar->SetPosition(200, 400);
	context->Add(menubar);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
