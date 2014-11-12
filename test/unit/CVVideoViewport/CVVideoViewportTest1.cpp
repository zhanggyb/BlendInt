#include "CVVideoViewportTest1.hpp"
#include <BlendInt/Gui/CVVideoViewport.hpp>
#include <Common/UnitTestContext.hpp>

using namespace BlendInt;

CVVideoViewportTest1::CVVideoViewportTest1()
: testing::Test()
{
	// TODO: add constructor code
}

CVVideoViewportTest1::~CVVideoViewportTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(CVVideoViewportTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("CVVideoViewport - Foo1", 1280, 800);

    // TODO: add test code here
    UnitTestContext* context = Manage (new UnitTestContext);
	SetContext(context);
	context->Resize(1280, 800);

	CVVideoViewport* video = Manage(new CVVideoViewport);
	video->MoveTo((1280 - video->size().width()) / 2.f, (800 - video->size().height()) / 2.f);
	video->OpenCamera(0);
	video->Play();

	context->AddFrame(video);

    RunLoop(win);
    Terminate();

	ASSERT_TRUE(true);
}
