#include "MenuTest1.hpp"
#include <BlendInt/Gui/Menu.hpp>
#include <BlendInt/Stock/Icons.hpp>

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
	GLFWwindow* window = CreateWindow("Menu - Foo1", 640, 480);

	Context* context = Manage (new Context);
	SetContext(context);
	context->Resize(640, 480);

	// TODO: add test code here
	Menu* menu = Manage(new Menu);
	//menu->SetRoundType(RoundAll);

	menu->MoveTo(200, 200);
	//menu->Resize (200, 200);

	menu->AddAction(Context::icons->check(), "MenuItem1", "Ctrl + 1");
	menu->AddAction("MenuItem2", "Ctrl + 1");
	menu->AddAction("MenuItem3", "Ctrl + 1");
	menu->AddAction("MenuItem4", "Ctrl + 1");
	menu->AddAction("MenuItem5");

	context->AddFrame(menu);

	RunLoop(window);
	Terminate();

	ASSERT_TRUE(true);
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
/*
TEST_F(MenuTest1, Click1)
{
	Init ();
	GLFWwindow* window = CreateWindow("MenuTest1 - Click1");

	// TODO: add test code here
    //
    RefPtr<VertexIcon> icon = StockItems::instance->icon_menu();
    Menu* menubin = new Menu;
    menubin->SetRoundType(RoundAll);

    menubin->SetPosition(200, 200);

    menubin->AddMenuItem(icon, "MenuItem1", "Ctrl + A");
    menubin->AddMenuItem(icon, "MenuItem2", "Ctrl + B");
    menubin->AddMenuItem(icon, "MenuItem3", "Ctrl + C");
    menubin->AddMenuItem(icon, "MenuItem4", "Ctrl + D");
    menubin->AddMenuItem(icon, "MenuItem5", "Ctrl + E");
    
	InfoWidget* info = new InfoWidget;
	
	info->SetPosition(600, 200);

	//Interface::instance->events()->connect(menubin->triggered(), info, &InfoWidget::onShowText);

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
*/
