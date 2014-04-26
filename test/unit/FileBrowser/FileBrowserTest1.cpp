#include "FileBrowserTest1.hpp"
#include <BlendInt/Gui/FileBrowser.hpp>

using namespace BlendInt;

FileBrowserTest1::FileBrowserTest1()
: testing::Test()
{
	// TODO: add constructor code
}

FileBrowserTest1::~FileBrowserTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(FileBrowserTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("FileBrowser - Foo1", 640, 480);

    // TODO: add test code here
	Context* context = Manage (new Context);
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);

	FileBrowser* fb = Manage(new FileBrowser);
	fb->set_name("FileBrowser");
	fb->SetPosition(50, 50);
	
	context->Add(fb);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
