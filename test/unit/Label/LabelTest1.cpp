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
	Font my_font ("Source Code Pro", 24);
	Font my_shadow_font ("Source Code Pro", 24);

	my_shadow_font.set_shadow(true);

	Label* label1 = Manage (new Label("Hello, World1!"));
	label1->set_name("Label1");
	label1->Resize(200, 50);
	label1->SetFont(my_font);
	label1->set_foreground_color(Color(0xF0B0E0EF));
	label1->set_background_color(Color(0x101010A0));
	label1->SetPosition(200, 200);

	Label* label2 = Manage (new Label("Hello, World2!"));
	label2->set_name("Label2");
	label2->Resize(200, 50);
	label2->SetFont(my_shadow_font);
	label2->set_background_color(Color(0xAF60E0BF));
	label2->SetPosition(200, 100);


    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
