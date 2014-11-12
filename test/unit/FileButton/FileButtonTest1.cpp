#include "FileButtonTest1.hpp"
#include <BlendInt/Gui/FileButton.hpp>
#include <BlendInt/Gui/Dialog.hpp>

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
	SetContext(context);
	context->Resize(1280, 800);

	Dialog* dialog = Manage(new Dialog);

    FileButton* fb = Manage(new FileButton);
	fb->MoveTo(100, 100);

	dialog->AddWidget(fb);

	context->AddFrame(dialog);

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
