#include "MenuTest1.hpp"
#include <BlendInt/Menu.hpp>
#include "InfoWidget.hpp"
#include <BlendInt/StockIcon.hpp>

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
    Menu* menubin = new Menu;
    menubin->SetRoundType(RoundAll);

    menubin->SetPosition(200, 200);

    menubin->AddMenuItem("MenuItem1");
    menubin->AddMenuItem("MenuItem2");
    menubin->AddMenuItem("MenuItem3");
    menubin->AddMenuItem("MenuItem4");
    menubin->AddMenuItem("MenuItem5");

    menubin->Register();

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(MenuTest1, Click1)
{
	Init ();
	GLFWwindow* window = CreateWindow("MenuTest1 - Click1");

	// TODO: add test code here
    //
    VertexIcon* icon = StockIcon::Instance()->menu();
    Menu* menubin = new Menu;
    menubin->SetRoundType(RoundAll);

    menubin->SetPosition(200, 200);

    menubin->AddMenuItem(icon, "MenuItem1", "Ctrl + A");
    menubin->AddMenuItem(icon, "MenuItem2", "Ctrl + B");
    menubin->AddMenuItem(icon, "MenuItem3", "Ctrl + C");
    menubin->AddMenuItem(icon, "MenuItem4", "Ctrl + D");
    menubin->AddMenuItem(icon, "MenuItem5", "Ctrl + E");
    
    menubin->Register();

	InfoWidget* info = new InfoWidget;
	
	info->SetPosition(600, 200);

    info->Register();
	
	Interface::Instance()->events()->connect(menubin->triggered(), info, &InfoWidget::onShowText);

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
