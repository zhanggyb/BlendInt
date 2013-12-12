#include "VerticalLayoutTest1.hpp"
#include <BlendInt/VerticalLayout.hpp>

using namespace BlendInt;

VerticalLayoutTest1::VerticalLayoutTest1()
: testing::Test()
{
	// TODO: add constructor code
}

VerticalLayoutTest1::~VerticalLayoutTest1()
{
	// TODO: add destructor code
}

/**
 * test default behavior of VLayout
 *
 * Expected result: 
 */
TEST_F(VerticalLayoutTest1, Add1)
{
	Init ();
	GLFWwindow* window = CreateWindow("VerticalLayout Test");

	VerticalLayout* vlayout = new VerticalLayout;
	vlayout->SetPosition(200, 200);

	Widget* widget1 = new Widget;
	widget1->SetExpand(false);
	Widget* widget2 = new Widget;
	widget1->SetExpand(false);

	vlayout->Add(widget1);
	vlayout->Add(widget2);

	Interface::Instance()->Bind(vlayout);

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * Test behavior if HLayout geometry is locked
 *
 * Expected result: the 2 widgets should fill the layout
 *
 * Note: this test is only valid in DEBUG build
 */
TEST_F(VerticalLayoutTest1, Add2)
{
	Init ();
	GLFWwindow* window = CreateWindow("VerticalLayout Test");

	VerticalLayout* vlayout = new VerticalLayout;
	vlayout->SetPosition(200, 200);
	vlayout->Resize(200, 400);

#ifdef DEBUG
	vlayout->lock(true);
#endif

	Widget* widget1 = new Widget;
	widget1->SetExpand(true);
	Widget* widget2 = new Widget;
	widget2->SetExpand(true);

	vlayout->Add(widget1);
	vlayout->Add(widget2);

	Interface::Instance()->Bind(vlayout);

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
