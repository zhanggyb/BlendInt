#include "HorizontalLayoutTest1.hpp"
#include <BlendInt/Interface.hpp>
#include <BlendInt/HorizontalLayout.hpp>

using namespace BlendInt;

HorizontalLayoutTest1::HorizontalLayoutTest1()
: testing::Test()
{
	// TODO: add constructor code
}

HorizontalLayoutTest1::~HorizontalLayoutTest1()
{
	// TODO: add destructor code
}

/**
 * Test the default behavior
 *
 * Expected result: the 2 widgets should fill the layout and resize with the preferred size
 */
TEST_F(HorizontalLayoutTest1, Add1)
{
	Init ();
	GLFWwindow* window = CreateWindow("HorizontalLayout Test");

	HorizontalLayout* hlayout = new HorizontalLayout;
	hlayout->SetPosition(200, 200);

	Widget* widget1 = new Widget;
	widget1->SetExpand(false);
	Widget* widget2 = new Widget;
	widget1->SetExpand(false);

	hlayout->Add(widget1);
	hlayout->Add(widget2);

	Interface::instance()->bind(hlayout);

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
TEST_F(HorizontalLayoutTest1, Add2)
{
	Init ();
	GLFWwindow* window = CreateWindow("HorizontalLayout Test");

	HorizontalLayout* hlayout = new HorizontalLayout;
	hlayout->SetPosition(200, 200);
	hlayout->Resize(400, 200);

#ifdef DEBUG
	hlayout->lock(true);
#endif

	Widget* widget1 = new Widget;
	widget1->SetExpand(true);
	Widget* widget2 = new Widget;
	widget2->SetExpand(true);

	hlayout->Add(widget1);
	hlayout->Add(widget2);

	Interface::instance()->bind(hlayout);

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
