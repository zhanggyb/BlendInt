#include "BoxFrameTest1.hpp"
#include <BlendInt/BoxFrame.hpp>
#include <BlendInt/HorizontalLayout.hpp>

using namespace BlendInt;

BoxFrameTest1::BoxFrameTest1()
: testing::Test()
{
	// TODO: add constructor code
}

BoxFrameTest1::~BoxFrameTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(BoxFrameTest1, SetLayout1)
{
	Init ();
	GLFWwindow* window = CreateWindow("BoxFrame - Foo1");

	// TODO: add test code here
	BoxFrame* boxframe = new BoxFrame;

	HorizontalLayout* hlayout = new HorizontalLayout;
	hlayout->Resize(400, 400);

	boxframe->SetLayout(hlayout);

	Interface::Instance()->Bind(boxframe);

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
