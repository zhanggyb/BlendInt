#include "FileBrowserTest1.hpp"
#include <BlendInt/Gui/FileBrowser.hpp>
#include <BlendInt/Gui/Dialog.hpp>

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
	SetContext(context);
	context->Resize(640, 480);

	FileBrowser* fb = Manage(new FileBrowser);
	fb->MoveTo(50, 50);
	fb->Load(getenv("HOME"));

	Dialog* dialog = Manage(new Dialog);
	dialog->Resize(500, 400);
	dialog->AddWidget(fb);
	
	context->AddFrame(dialog);

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
