#include "FileSelectorTest1.hpp"
#include <BlendInt/Gui/FileSelector.hpp>
#include <BlendInt/Gui/Dialog.hpp>
#include <Common/UnitTestContext.hpp>

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
    UnitTestContext* context = Manage (new UnitTestContext);
	SetContext(context);
	context->Resize(1280, 800);

    FileSelector* fs = Manage(new FileSelector);
	fs->Resize(800, 600);

	context->AddFrame(fs);

    RunLoop(win);
    Terminate();

    ASSERT_TRUE(true);
}
