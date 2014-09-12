#include "StackTest1.hpp"
#include <BlendInt/Gui/Stack.hpp>
#include <BlendInt/Gui/Label.hpp>
#include <BlendInt/Gui/Button.hpp>

using namespace BlendInt;

StackTest1::StackTest1()
: testing::Test()
{
	// TODO: add constructor code
}

StackTest1::~StackTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(StackTest1, Foo1)
{
	Init ();
	GLFWwindow* window = CreateWindow("Stack - Foo1", 640, 480);

	// TODO: add test code here
	Context* context = Manage (new Context);
	DBG_SET_NAME(context, "Context");
    Interface::instance->SetCurrentContext(context);

//	Label* label1 = Manage (new Label("Label1"));
//	label1->set_name("Labe1");
//	label1->SetFont(Font("Droid Sans Mono", 24));
//	label1->set_background_color(Color(0x0000FF8F));
//	Label* label2 = Manage (new Label("Label2"));
//	label2->set_name("Labe2");
//	label2->SetFont(Font("Droid Sans Mono", 24));
//	label2->set_background_color(Color(0xFF00008F));
//	Label* label3 = Manage (new Label("Label3"));
//	label3->set_name("Labe3");
//	label3->SetFont(Font("Droid Sans Mono", 24));
//	label3->set_background_color(Color(0x00FF008F));

	Button* label1 = Manage (new Button("Label1"));
	DBG_SET_NAME(label1, "Button1");
	label1->SetFont(Font("Droid Sans Mono", 24));
	Button* label2 = Manage (new Button("Label2"));
	DBG_SET_NAME(label2, "Button2");
	label2->SetFont(Font("Droid Sans Mono", 24));
	Button* label3 = Manage (new Button("Label3"));
	DBG_SET_NAME(label3, "Button3");
	label3->SetFont(Font("Droid Sans Mono", 24));

	Stack* stack = Manage (new Stack);
	DBG_SET_NAME(stack, "Stack");
	stack->Append(label1);
	stack->Append(label2);
	stack->Append(label3);

	stack->SetPosition(200, 200);
	stack->Resize(200, 200);
	stack->SetIndex(0);

	context->Append(stack);

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
