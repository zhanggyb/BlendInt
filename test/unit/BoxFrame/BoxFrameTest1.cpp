#include "BoxFrameTest1.hpp"
#include <BlendInt/BoxFrame.hpp>
#include <BlendInt/HorizontalLayout.hpp>
#include <BlendInt/Button.hpp>

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
	GLFWwindow* window = CreateWindow("BoxFrameTest1 - SetLayout1");

	// TODO: add test code here
	BoxFrame* boxframe = new BoxFrame;

	HorizontalLayout* hlayout = new HorizontalLayout;
	// hlayout->Resize(400, 400);

	Button* button = new Button;
	Widget* widget = new Widget;

	hlayout->Add(button);
	hlayout->Add(widget);

	boxframe->SetLayout(hlayout);

	Interface::Instance()->Bind(boxframe);

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * test Foo() method
 *
 * Expected result:
 */
TEST_F(BoxFrameTest1, Move1)
{
	Init ();
	GLFWwindow* window = CreateWindow("BoxFrameTest1 - Move1");

	// TODO: add test code here
	BoxFrame* boxframe = new BoxFrame;

	HorizontalLayout* hlayout = new HorizontalLayout;
	// hlayout->Resize(400, 400);

	Button* button = new Button;
	Widget* widget = new Widget;

	hlayout->Add(button);
	hlayout->Add(widget);

	boxframe->SetLayout(hlayout);

	boxframe->SetPosition(400, 400);

	Interface::Instance()->Bind(boxframe);

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
