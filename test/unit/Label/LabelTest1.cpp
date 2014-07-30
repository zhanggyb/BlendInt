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
	Context* context = Manage(new Context);
	DBG_SET_NAME(context, "Context");
	Interface::instance->SetCurrentContext(context);

	Font my_font ("Droid Sans Mono", 24);
	Font my_shadow_font ("Source Code Pro", 24);
	my_shadow_font.SetShadow(true, 2.5f, -2.f);

	Label* label1 = Manage (new Label("Hello, World1!"));
	label1->SetFont(my_font);
	label1->set_foreground_color(Color(0xF0B0E0EF));
	label1->set_background_color(Color(0x101010A0));
	label1->Resize(label1->GetPreferredSize());
//	label1->Resize(300, 200);
	label1->SetPosition(200, 200);

	Label* label2 = Manage (new Label("Hello, World2!"));
	label2->SetFont(my_shadow_font);
	label2->set_background_color(Color(0xAF60E0BF));
	label2->Resize(label2->GetPreferredSize());
	label2->SetPosition(200, 100);

	context->PushBack(label1);
	context->PushBack(label2);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
