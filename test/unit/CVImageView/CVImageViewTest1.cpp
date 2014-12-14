#include "CVImageViewTest1.hpp"
#include <BlendInt/Gui/CVImageView.hpp>

using namespace BlendInt;

CVImageViewTest1::CVImageViewTest1()
: testing::Test()
{
	// TODO: add constructor code
}

CVImageViewTest1::~CVImageViewTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(CVImageViewTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("CVImageView - Foo1", 1280, 800);

    // TODO: add test code here
	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);
    context->Resize(1280, 800);

    ImageViewport* img = Manage(new ImageViewport);
    img->MoveTo(100, 100);
    context->Append(img);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
