#include "ImageViewTest1.hpp"
#include <BlendInt/Gui/ImageView.hpp>

using namespace BlendInt;

ImageViewTest1::ImageViewTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ImageViewTest1::~ImageViewTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ImageViewTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("ImageView - Foo1", 640, 640);

    // TODO: add test code here

    ImageView* view = Manage(new ImageView);
    view->set_name("ImageView");
    view->SetPosition(20, 20);

    view->Open("rendering.jpg");

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
