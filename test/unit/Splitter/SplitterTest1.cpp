#include "SplitterTest1.hpp"
#include <BlendInt/Gui/Splitter.hpp>
#include <BlendInt/Gui/Button.hpp>

using namespace BlendInt;

SplitterTest1::SplitterTest1()
: testing::Test()
{
	// TODO: add constructor code
}

SplitterTest1::~SplitterTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(SplitterTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("Splitter - Foo1", 640, 480);

    // TODO: add test code here
	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

	Splitter* splitter = Manage(new Splitter);
#ifdef DEBUG
	splitter->set_name("Splitter");
#endif
	splitter->SetPosition(50, 50);
	splitter->Resize(450, 200);
	
	Button* btn1 = Manage(new Button("Button1"));
	Button* btn2 = Manage(new Button("Button2"));
	Button* btn3 = Manage(new Button("Button3"));
	
	splitter->Append(btn1);
	splitter->Append(btn2);
	splitter->Append(btn3);
	
	context->Append(splitter);

	splitter->Resize(400, 200);
	splitter->SetPosition(100, 100);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
