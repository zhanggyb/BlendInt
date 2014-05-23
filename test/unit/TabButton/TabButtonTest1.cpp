#include "TabButtonTest1.hpp"
#include <BlendInt/Gui/TabButton.hpp>

using namespace BlendInt;

TabButtonTest1::TabButtonTest1()
: testing::Test()
{
	// TODO: add constructor code
}

TabButtonTest1::~TabButtonTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(TabButtonTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("TabButton - Foo1", 640, 480);

    // TODO: add test code here
    TabButton* btn = Manage(new TabButton);
    btn->SetPosition(200, 200);

    //std::vector<GLfloat> vert;

    //btn->GenerateTabButtonVertices(btn->size(), 1.f, 0, &vert);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
