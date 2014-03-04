#include "BoxFrameTest1.hpp"
#include <BlendInt/Gui/BoxFrame.hpp>
#include <BlendInt/Gui/HLayout.hpp>
#include <BlendInt/Gui/Button.hpp>

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

	HLayout* hlayout = new HLayout;
	// hlayout->Resize(400, 400);

	Button* button = new Button;
	Widget* widget = new Widget;

	hlayout->Add(button);
	hlayout->Add(widget);

	boxframe->SetLayout(hlayout);

    boxframe->Register();

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

	HLayout* hlayout = new HLayout;
	// hlayout->Resize(400, 400);

	Button* button = new Button;
	Widget* widget = new Widget;

	hlayout->Add(button);
	hlayout->Add(widget);

	boxframe->SetLayout(hlayout);

	boxframe->SetPosition(400, 400);

    boxframe->Register();

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
