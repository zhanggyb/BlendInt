#include "ComboBoxTest1.hpp"
#include <BlendInt/Gui/ComboBox.hpp>

using namespace BlendInt;

ComboBoxTest1::ComboBoxTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ComboBoxTest1::~ComboBoxTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ComboBoxTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("ComboBox - Foo1", 640, 480);

	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    ComboBox* combo = Manage(new ComboBox);

    combo->SetPosition(200, 200);

    context->AddWidget(combo);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
