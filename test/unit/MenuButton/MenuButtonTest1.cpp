#include "MenuButtonTest1.hpp"
#include <BlendInt/Gui/MenuButton.hpp>

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

    MenuButton* button = Manage(new MenuButton("File"));

    button->SetPosition(200, 200);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
