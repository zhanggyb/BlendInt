#include "MenuButtonTest1.hpp"
#include <BlendInt/Gui/MenuButton.hpp>
#include <BlendInt/Service/StockItems.hpp>

using namespace BlendInt;

MenuButtonTest1::MenuButtonTest1()
: testing::Test()
{
	// TODO: add constructor code
}

MenuButtonTest1::~MenuButtonTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(MenuButtonTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("MenuButton - Foo1", 640, 480);

    // TODO: add test code here
    Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    MenuButton* button = Manage(new MenuButton("File"));
    button->SetPosition(200, 200);

    RefPtr<Menu> menu(new Menu);

    menu->SetRoundType(RoundTopLeft | RoundTopRight);
    //m_menu->SetPosition(200, 200);
    //menu->Resize (200, 200);

    /*
    menu->AddActionItem(StockItems::instance->icon_check(), "MenuItem1", "Ctrl + 1");
    menu->AddActionItem("MenuItem2", "Ctrl + 1");
    menu->AddActionItem("MenuItem3", "Ctrl + 1");
    menu->AddActionItem("MenuItem4", "Ctrl + 1");
    menu->AddActionItem("MenuItem5");
    */

    button->SetMenu(menu);

    context->Add(button);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
