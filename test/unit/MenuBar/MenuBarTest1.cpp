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

    menubar->AddMenuButton("File", menu);
    menubar->AddMenuButton("Edit", menu);
    menubar->AddMenuButton("Help", menu);

    menubar->SetPosition(200, 400);
	context->Add(menubar);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
