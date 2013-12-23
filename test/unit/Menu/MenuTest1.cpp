#include "MenuTest1.hpp"
#include <BlendInt/Menu.hpp>

using namespace BlendInt;

MenuTest1::MenuTest1()
: testing::Test()
{
	// TODO: add constructor code
}

MenuTest1::~MenuTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(MenuTest1, Foo1)
{
	Init ();
	GLFWwindow* window = CreateWindow("Menu - Foo1");

	// TODO: add test code here
	Menu* menu = new Menu;
	
	menu->SetPosition(200, 200);
	menu->Resize(200, 400);
	
	menu->add("Hello");
	menu->add("Sdalfkjas");
	
	Interface::Instance()->Bind(menu);

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
