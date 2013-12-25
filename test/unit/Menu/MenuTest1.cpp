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
TEST_F(MenuTest1, Add1)
{
	Init ();
	GLFWwindow* window = CreateWindow("Menu - Add1");

	// TODO: add test code here
    
    Menu* menu = new Menu("Menu");

    menu->Add("item1");
    menu->Add("item2");
    menu->Add("item3");

#ifdef DEBUG
    menu->print_menu_items();
#endif

    delete menu;

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(MenuTest1, Add2)
{
	Init ();
	GLFWwindow* window = CreateWindow("Menu - Add2");

	// TODO: add test code here
    
    Menu* menu = new Menu("Menu");

    MenuItem* item1 = new MenuItem("item1");
    MenuItem* item2 = new MenuItem("item2");
    MenuItem* item3 = new MenuItem("item3");

    menu->Add(item1);
    menu->Add(item2);
    menu->Add(item3);

#ifdef DEBUG
    menu->print_menu_items();
#endif

    delete menu;

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(MenuTest1, Add3)
{
	Init ();
	GLFWwindow* window = CreateWindow("Menu - Add3");

	// TODO: add test code here
    
    Menu* menu1 = new Menu("Menu1");

    MenuItem* item1 = new MenuItem("item1");
    MenuItem* item2 = new MenuItem("item2");
    MenuItem* item3 = new MenuItem("item3");

    menu1->Add(item1);
    menu1->Add(item2);
    menu1->Add(item3);

    Menu* menu2 = new Menu("Menu2");
    MenuItem* item4 = new MenuItem("item4");
    MenuItem* item5 = new MenuItem("item5");
    MenuItem* item6 = new MenuItem("item6");
    menu2->Add(item4);
    menu2->Add(item5);
    menu2->Add(item6);

    item3->SetSubMenu(menu2);

#ifdef DEBUG
    menu1->print_menu_items();
#endif

    delete menu1;

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(MenuTest1, Add4)
{
	Init ();
	GLFWwindow* window = CreateWindow("Menu - Add4");

	// TODO: add test code here
    
    Menu* menu1 = new Menu("Menu1");

    MenuItem* item1 = new MenuItem("item1");
    MenuItem* item2 = new MenuItem("item2");
    MenuItem* item3 = new MenuItem("item3");

    menu1->Add(item1);
    menu1->Add(item2);
    menu1->Add(item3);

    Menu* menu2 = new Menu("Menu2");
    MenuItem* item4 = new MenuItem("item4");
    MenuItem* item5 = new MenuItem("item5");
    MenuItem* item6 = new MenuItem("item6");
    menu2->Add(item4);
    menu2->Add(item5);
    menu2->Add(item6);

    item3->SetSubMenu(menu2);

    MenuItem* item7 = new MenuItem("item7");
    item7->SetParentMenu(menu1);
    item7->SetSubMenu(menu2);

#ifdef DEBUG
    menu1->print_menu_items();
#endif

    delete menu1;

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
