#include "VerticalLayoutTest1.hpp"
#include <BlendInt/Interface.hpp>
#include <BlendInt/Gui/VLayout.hpp>
#include <BlendInt/Gui/Widget.hpp>

using namespace BlendInt;

VLayoutTest1::VLayoutTest1()
: testing::Test()
{
	// TODO: add constructor code
}

VLayoutTest1::~VLayoutTest1()
{
	// TODO: add destructor code
}

/**
 * Test the default behavior
 *
 * Expected result: the 2 widgets should fill the layout and resize with the preferred size
 */
TEST_F(VLayoutTest1, Add1)
{
	Init ();
	GLFWwindow* window = CreateWindow("VLayoutTest1 - Add1", 640, 480);

	Context* context = Manage(new Context);
    Interface::instance->SetCurrentContext(context);

	VLayout* vlayout = new VLayout;
	vlayout->SetPosition(100, 100);

	Widget* widget1 = new Widget;
	Widget* widget2 = new Widget;

	vlayout->Append(Manage(widget1));
	vlayout->Append(Manage(widget2));

	context->Append(vlayout);

	RunLoop(window);

	delete vlayout;
	
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
TEST_F(VLayoutTest1, Add2)
{
	Init ();
	GLFWwindow* window = CreateWindow("VLayoutTest1 - Add2", 640, 480);
	Context* context = Manage(new Context);
    Interface::instance->SetCurrentContext(context);

	VLayout* vlayout = new VLayout;
	vlayout->SetPosition(100, 100);
	vlayout->Resize(200, 400);

	Widget* widget1 = new Widget;
	Widget* widget2 = new Widget;

	vlayout->Append(Manage(widget1));
	vlayout->Append(Manage(widget2));

	context->Append(vlayout);

	RunLoop(window);

	delete vlayout;

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * Test behavior if HLayout geometry is locked and 1 expandable + 1 fixed size
 *
 * Expected result: the 2 widgets should fill the layout
 *
 * Note: this test is only valid in DEBUG build
 */
TEST_F(VLayoutTest1, Add3)
{
	Init ();
	GLFWwindow* window = CreateWindow("VLayoutTest1 - Add3", 640, 480);

	Context* context = Manage(new Context);
    Interface::instance->SetCurrentContext(context);

	VLayout* vlayout = new VLayout;
	vlayout->SetPosition(100, 100);
	vlayout->Resize(200, 400);

	Widget* widget1 = new Widget;
	Widget* widget2 = new Widget;

	vlayout->Append(Manage(widget1));
	vlayout->Append(Manage(widget2));

	context->Append(vlayout);

	RunLoop(window);

	delete vlayout;

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * Test behavior if more than 1 expandable child with max width 
 *
 * Expected result: the widgets should fill the layout
 */
TEST_F(VLayoutTest1, Add4)
{
	Init ();
	GLFWwindow* window = CreateWindow("VLayoutTest1 - Add4", 640, 480);

	Context* context = Manage(new Context);
    Interface::instance->SetCurrentContext(context);

	VLayout* vlayout = new VLayout;
	vlayout->SetPosition(100, 100);
	vlayout->Resize(200, 400);

	Widget* widget1 = new Widget;
    //widget1->SetPreferredSize(widget1->preferred_size().width(), 40);
    //widget1->SetMaximalSize(widget1->maximal_size().width(), 80);
	Widget* widget2 = new Widget;
    //widget2->SetPreferredSize(widget2->preferred_size().width(), 40);
    //widget2->SetMaximalSize(widget2->maximal_size().width(), 80);

	vlayout->Append(Manage(widget1));
	vlayout->Append(Manage(widget2));

	context->Append(vlayout);

	RunLoop(window);

	delete vlayout;

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * Test behavior if more than 1 expandable child with max width and 1 more fixed child
 *
 * Expected result: the widgets should fill the layout
 */
TEST_F(VLayoutTest1, Add5)
{
	Init ();
	GLFWwindow* window = CreateWindow("VLayoutTest1 - Add5", 640, 480);

	Context* context = Manage(new Context);
    Interface::instance->SetCurrentContext(context);

	VLayout* vlayout = new VLayout;
	vlayout->SetPosition(100, 100);
	vlayout->Resize(200, 600);

	Widget* widget1 = new Widget;
    //widget1->SetPreferredSize(widget1->preferred_size().width(), 40);
    //widget1->SetMaximalSize(widget1->preferred_size().width(), 80);
	Widget* widget2 = new Widget;
    //widget2->SetPreferredSize(widget1->preferred_size().width(), 40);
    //widget2->SetMaximalSize(widget1->preferred_size().width(), 80);
	Widget* widget3 = new Widget;
    //widget3->SetPreferredSize(widget1->preferred_size().width(), 40);
    //widget3->SetMaximalSize(widget1->preferred_size().width(), 60);
	Widget* widget4 = new Widget;
	//widget4->SetExpandY(true);
    //widget4->SetPreferredSize(widget1->preferred_size().width(), 40);
    //widget4->SetMaximalSize(widget1->preferred_size().width(), 70);
	widget4->Resize(widget4->size().width(), 40);
	
    vlayout->Append(Manage(widget1));
	vlayout->Append(Manage(widget2));
	vlayout->Append(Manage(widget3));
	vlayout->Append(Manage(widget4));

	context->Append(vlayout);

	RunLoop(window);

	delete vlayout;

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * Test behavior if more than 1 expandable child and 1 more fixed child in small size
 *
 * Expected result: the widgets should fill the layout
 */
TEST_F(VLayoutTest1, Add6)
{
	Init ();
	GLFWwindow* window = CreateWindow("VLayoutTest1 - Add6", 640, 480);

	Context* context = Manage(new Context);
    Interface::instance->SetCurrentContext(context);

	VLayout* vlayout = new VLayout;
	vlayout->SetPosition(100, 100);
	vlayout->Resize(100, 50);

	Widget* widget1 = new Widget;
    //widget1->SetPreferredSize(widget1->preferred_size().height(), 40);
    //widget1->SetMaximalSize(widget1->maximal_size().height(), 80);
	Widget* widget2 = new Widget;
    //widget2->SetMinimalSize(widget2->minimal_size().height(), 10);
	Widget* widget3 = new Widget;
    //widget3->SetMinimalSize(widget3->minimal_size().height(), 20);
	Widget* widget4 = new Widget;
	
    vlayout->Append(Manage(widget1));
	vlayout->Append(Manage(widget2));
	vlayout->Append(Manage(widget3));
	vlayout->Append(Manage(widget4));

	context->Append(vlayout);

	RunLoop(window);

	delete vlayout;

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * Test behavior if more than 1 expandable child and 1 more fixed child in small size
 *
 * Expected result: the widgets should fill the layout
 */
TEST_F(VLayoutTest1, Add7)
{
	Init ();
	GLFWwindow* window = CreateWindow("VLayoutTest1 - Add7", 640, 480);

	Context* context = Manage(new Context);
    Interface::instance->SetCurrentContext(context);

	VLayout* vlayout = new VLayout;
	vlayout->SetPosition(100, 100);

	Widget* widget1 = new Widget;
	//widget1->SetExpandX(true);
	//widget1->SetMinimalSize(10, widget1->minimal_size().height());
	Widget* widget2 = new Widget;
    //widget2->SetMinimalSize(30, widget2->minimal_size().height());
	Widget* widget3 = new Widget;
    //widget3->SetMinimalSize(20, widget3->minimal_size().height());
	Widget* widget4 = new Widget;

	vlayout->Append(Manage(widget1));
	vlayout->Append(Manage(widget2));
	vlayout->Append(Manage(widget3));
	vlayout->Append(Manage(widget4));

	vlayout->Resize(100, 80);

	context->Append(vlayout);

	RunLoop(window);

	delete vlayout;

	Terminate();

	ASSERT_TRUE(true);
}


/**
 * Test resize
 *
 * Expected result: the children in the HLayout resize too according to the layout size
 */
TEST_F(VLayoutTest1, Resize1)
{
	Init();
	GLFWwindow* window = CreateWindow("VLayoutTest1 - Resize1", 640, 480);

	Context* context = Manage(new Context);
    Interface::instance->SetCurrentContext(context);

	// add test code here

	VLayout* vlayout = new VLayout;
	vlayout->SetPosition(100, 100);

	Widget* widget1 = new Widget;
	Widget* widget2 = new Widget;

	vlayout->Append(Manage(widget1));
	vlayout->Append(Manage(widget2));

	vlayout->Resize(200, 400);

	context->Append(vlayout);

	RunLoop(window);

	delete vlayout;

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * Test resize, no child is expandable
 *
 * Expected result: the children in the HLayout resize too according to the layout size
 */
TEST_F(VLayoutTest1, Resize2)
{
	Init();
	GLFWwindow* window = CreateWindow("VLayoutTest1 - Resize2", 640, 480);

	Context* context = Manage(new Context);
    Interface::instance->SetCurrentContext(context);

	// add test code here

	VLayout* vlayout = new VLayout;
	vlayout->SetPosition(100, 100);

	Widget* widget1 = new Widget;
	Widget* widget2 = new Widget;

	vlayout->Append(Manage(widget1));
	vlayout->Append(Manage(widget2));

	vlayout->Resize(200, 400);

	context->Append(vlayout);

	RunLoop(window);

	delete vlayout;

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * Test resize to small size
 *
 * Expected result: the children in the HLayout resize too according to the layout size
 */
TEST_F(VLayoutTest1, Resize3)
{
	Init();
	GLFWwindow* window = CreateWindow("VLayoutTest1 - Resize3", 640, 480);

	Context* context = Manage(new Context);
    Interface::instance->SetCurrentContext(context);

	// add test code here

	VLayout* vlayout = new VLayout;
	vlayout->SetPosition(100, 100);

	Widget* widget1 = new Widget;
	Widget* widget2 = new Widget;
    //widget2->SetMinimalSize(30, widget2->minimal_size().height());
	Widget* widget3 = new Widget;
    //widget3->SetMinimalSize(20, widget3->minimal_size().height());
	Widget* widget4 = new Widget;

	vlayout->Append(Manage(widget1));
	vlayout->Append(Manage(widget2));
	vlayout->Append(Manage(widget3));
	vlayout->Append(Manage(widget4));

	vlayout->Resize(100, 80);

	context->Append(vlayout);

	RunLoop(window);

	delete vlayout;

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * Test SetMargin
 *
 * Expected result: the children in the HLayout resize too according to the new margin
 */
TEST_F(VLayoutTest1, SetMargin1)
{
	Init();
	GLFWwindow* window = CreateWindow("VLayoutTest1 - SetMargin1", 640, 480);

	Context* context = Manage(new Context);
    Interface::instance->SetCurrentContext(context);

	// add test code here

	VLayout* vlayout = new VLayout;
	vlayout->SetPosition(100, 100);

	Widget* widget1 = new Widget;
    widget1->Resize(100, 80);
	Widget* widget2 = new Widget;
    widget2->Resize(100, 80);

	vlayout->Append(Manage(widget1));
	vlayout->Append(Manage(widget2));

    vlayout->Resize(200, 400);

    vlayout->SetMargin(20, 20, 20, 20);

    context->Append(vlayout);

	RunLoop(window);

	delete vlayout;

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * Test SetSpace
 *
 * Expected result: the children in the HLayout resize too according to the new space
 */
TEST_F(VLayoutTest1, SetSpace1)
{
	Init();
	GLFWwindow* window = CreateWindow("VLayoutTest1 - SetSpace1", 640, 480);

	Context* context = Manage(new Context);
    Interface::instance->SetCurrentContext(context);

	// add test code here

	VLayout* vlayout = new VLayout;
	vlayout->SetPosition(100, 100);

	Widget* widget1 = new Widget;
    widget1->Resize(100, 80);
	Widget* widget2 = new Widget;
    widget2->Resize(100, 80);

	vlayout->Append(Manage(widget1));
	vlayout->Append(Manage(widget2));

    vlayout->Resize(200, 400);

    vlayout->SetSpace(10);

    context->Append(vlayout);

	RunLoop(window);

	delete vlayout;

	Terminate();

	ASSERT_TRUE(true);
}
