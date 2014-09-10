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
TEST_F(LabelTest1, Resize1)
{
	Init ();

    GLFWwindow* win = CreateWindow("Label - Foo1", 640, 480);

    // TODO: add test code here
	Context* context = Manage(new Context);
	DBG_SET_NAME(context, "Context");
	Interface::instance->SetCurrentContext(context);

	Font my_font ("Droid Sans", 24);

	Label* label1 = Manage (new Label("Hello, World1!"));
	label1->SetText("3D Mesh is loaded");
	label1->SetFont(my_font);
	label1->SetForegroundColor(Color(0xF0B0E0EF));
	label1->SetBackgroundColor(Color(0x101010A0));
	label1->Resize(label1->GetPreferredSize());
	label1->Resize(300, 40);
	label1->SetPosition(200, 200);

	context->Append(label1);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
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

	Font my_font ("Droid Sans", 24);
	Font my_shadow_font ("Droid Sans", 24);
	my_shadow_font.SetShadow(true, 2.5f, -2.f);

	Label* label1 = Manage (new Label("Hello, World1!"));
	label1->SetText("3D Mesh is loaded");
	label1->SetFont(my_font);
	label1->SetForegroundColor(Color(0xF0B0E0EF));
	label1->SetBackgroundColor(Color(0x101010A0));
	label1->Resize(label1->GetPreferredSize());
//	label1->Resize(300, 200);
	label1->SetPosition(200, 200);

	Label* label2 = Manage (new Label("Hello, World2!"));
	label2->SetFont(my_shadow_font);
	label2->SetBackgroundColor(Color(0xAF60E0BF));
	label2->Resize(label2->GetPreferredSize());
	label2->SetPosition(200, 100);

	label1->Resize(label1->GetPreferredSize());

	context->Append(label1);
	context->Append(label2);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
