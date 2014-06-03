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
	splitter->SetPosition(100, 100);
	
	Button* btn1 = Manage(new Button);
	Button* btn2 = Manage(new Button);
	
	splitter->PushBack(btn1);
	splitter->PushBack(btn2);
	
	splitter->Resize(200, 200);
	
	context->Add(splitter);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
