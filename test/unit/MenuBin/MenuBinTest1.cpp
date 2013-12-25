#include "MenuBinTest1.hpp"
#include <BlendInt/MenuBin.hpp>

using namespace BlendInt;

MenuBinTest1::MenuBinTest1()
: testing::Test()
{
	// TODO: add constructor code
}

MenuBinTest1::~MenuBinTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(MenuBinTest1, Foo1)
{
	Init ();
	GLFWwindow* window = CreateWindow("MenuBin - Foo1");

	// TODO: add test code here
    MenuBin* menubin = new MenuBin;
    menubin->SetRoundType(RoundAll);

    menubin->SetPosition(200, 200);

    //menubin->AddMenuItem("MenuItem1");
    //menubin->AddMenuItem("MenuItem2");
    //menubin->AddMenuItem("MenuItem3");
    
    Interface::Instance()->Bind(menubin);

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
