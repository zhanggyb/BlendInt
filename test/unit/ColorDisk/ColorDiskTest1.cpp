#include "ColorDiskTest1.hpp"
#include <BlendInt/Gui/ColorDisk.hpp>

using namespace BlendInt;

ColorDiskTest1::ColorDiskTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ColorDiskTest1::~ColorDiskTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ColorDiskTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("ColorDisk - Foo1", 640, 480);

	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    ColorDisk* disk = Manage(new ColorDisk);

    disk->SetPosition(200, 200);

    context->Add(disk);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
