#include "ExpanderTest1.hpp"
#include <BlendInt/Gui/Expander.hpp>
#include <BlendInt/Gui/Button.hpp>

using namespace BlendInt;

ExpanderTest1::ExpanderTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ExpanderTest1::~ExpanderTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ExpanderTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("Expander - Foo1", 640, 480);

	Context* context = Manage(new Context);
	context->set_name("Context");
	Interface::instance->SetCurrentContext(context);

	Expander* expander = Manage(new Expander);
	expander->set_name("Expander");
	expander->SetPosition(200, 200);
	
	Button* btn1 = Manage(new Button);
	btn1->set_name("Button1");

	expander->Setup(btn1);
	
	context->Add(expander);

	expander->Resize(200, 200);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
