#include "WidgetTest1.hpp"
#include <BlendInt/Gui/Widget.hpp>
#include <Common/Window.hpp>

using namespace BlendInt;

WidgetTest1::WidgetTest1()
: testing::Test()
{
	// TODO: add constructor code
}

WidgetTest1::~WidgetTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(WidgetTest1, Add1)
{
    Init();
    GLFWwindow* win = CreateWindow("WidgetTest1 -- Show1", 640, 480);

	// TODO: add test code here
    Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);
    
    Widget* widget1 = new Widget;
    widget1->SetPosition(200, 200);

    context->Add(widget1);

    widget1->Resize(200, 200);
    widget1->SetPosition(100, 100);

    RunLoop(win);

    delete widget1;

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}

