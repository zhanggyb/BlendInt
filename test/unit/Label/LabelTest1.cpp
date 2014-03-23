#include "LabelTest1.hpp"
#include <BlendInt/Gui/Label.hpp>

using namespace BlendInt;

LabelTest1::LabelTest1()
: testing::Test()
{
	// TODO: add constructor code
}

LabelTest1::~LabelTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(LabelTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("Label - Foo1", 640, 480);

    // TODO: add test code here
	//Font my_font ("sans-serif", 24, true);

	Label* label = Manage (new Label("Hello, World!"));
	label->set_name("Label");
	//label->SetFont(my_font);
	// label->SetForegroundColor(Color(0xFF00001F));
	label->SetPosition(200, 200);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
