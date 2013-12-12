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
	GLFWwindow* window = CreateWindow("HorizontalLayoutTest1 - Add1");

	HorizontalLayout* hlayout = new HorizontalLayout;
	hlayout->SetPosition(200, 200);

	Widget* widget1 = new Widget;
	widget1->SetExpand(false);
	Widget* widget2 = new Widget;
	widget1->SetExpand(false);

	hlayout->Add(widget1);
	hlayout->Add(widget2);

	Interface::Instance()->Bind(hlayout);

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
	GLFWwindow* window = CreateWindow("HorizontalLayoutTest1 - Add2");

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

	Interface::Instance()->Bind(hlayout);

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * Test resize
 *
 * Expected result: the children in the HLayout resize too according to the layout size
 */
TEST_F(HorizontalLayoutTest1, Resize1)
{
	Init();
	GLFWwindow* window = CreateWindow("HorizontalLayoutTest1 - Resize1");

	// add test code here

	HorizontalLayout* hlayout = new HorizontalLayout;
	hlayout->SetPosition(200, 200);

	Widget* widget1 = new Widget;
	widget1->SetExpand(true);
	Widget* widget2 = new Widget;
	widget2->SetExpand(true);

	hlayout->Add(widget1);
	hlayout->Add(widget2);

	hlayout->Resize(400, 200);

	Interface::Instance()->Bind(hlayout);

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * Test resize, no child is expandable
 *
 * Expected result: the children in the HLayout resize too according to the layout size
 */
TEST_F(HorizontalLayoutTest1, Resize2)
{
	Init();
	GLFWwindow* window = CreateWindow("HorizontalLayoutTest1 - Resize2");

	// add test code here

	HorizontalLayout* hlayout = new HorizontalLayout;
	hlayout->SetPosition(200, 200);

	Widget* widget1 = new Widget;
	Widget* widget2 = new Widget;

	hlayout->Add(widget1);
	hlayout->Add(widget2);

	hlayout->Resize(400, 200);

	Interface::Instance()->Bind(hlayout);

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
