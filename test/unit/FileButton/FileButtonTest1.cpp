#include "FileButtonTest1.hpp"
#include <BlendInt/Gui/FileButton.hpp>

using namespace BlendInt;

FileButtonTest1::FileButtonTest1()
: testing::Test()
{
	// TODO: add constructor code
}

FileButtonTest1::~FileButtonTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(FileButtonTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("FileButton - Foo1", 1280, 800);

    // TODO: add test code here
	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);
    context->Resize(1280, 800);

    FileButton* fb = Manage(new FileButton);
	fb->SetPosition(100, 100);

	context->PushBack(fb);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
