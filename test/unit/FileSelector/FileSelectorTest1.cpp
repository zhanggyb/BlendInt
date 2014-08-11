#include "FileSelectorTest1.hpp"
#include <BlendInt/Gui/FileSelector.hpp>

using namespace BlendInt;

FileSelectorTest1::FileSelectorTest1()
: testing::Test()
{
	// TODO: add constructor code
}

FileSelectorTest1::~FileSelectorTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(FileSelectorTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("FileSelector - Foo1", 1280, 800);

    // TODO: add test code here
    Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);
	context->Resize(1200, 800);

    FileSelector* fb = Manage(new FileSelector);
    fb->SetPosition(100, 100);
	fb->Resize(800, 600);

    context->PushBack(fb);

    RunLoop(win);

    Interface::Release();

    Terminate();

    ASSERT_TRUE(true);
}
