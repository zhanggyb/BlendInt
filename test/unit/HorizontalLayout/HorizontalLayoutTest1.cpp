#include "HorizontalLayoutTest1.hpp"
#include <BlendInt/Interface.hpp>
#include <BlendInt/Gui/HLayout.hpp>
#include <BlendInt/Gui/Widget.hpp>

using namespace BlendInt;

HLayoutTest1::HLayoutTest1()
: testing::Test()
{
	// TODO: add constructor code
}

HLayoutTest1::~HLayoutTest1()
{
	// TODO: add destructor code
}

/**
 * Test the default behavior
 *
 * Expected result: the 2 widgets should fill the layout and resize with the preferred size
 */
TEST_F(HLayoutTest1, Add1)
{
	Init ();
	GLFWwindow* window = CreateWindow("HLayoutTest1 - Add1", 640, 480);

	Context* context = Manage (new Context);
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);

	HLayout* hlayout = new HLayout;
	hlayout->SetPosition(100, 100);

	Widget* widget1 = new Widget;
	widget1->SetExpand(false);
	Widget* widget2 = new Widget;
	widget1->SetExpand(false);

	hlayout->Add(Manage(widget1));
	hlayout->Add(Manage(widget2));

	context->Add(hlayout);

	RunLoop(window);

	delete hlayout;

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
TEST_F(HLayoutTest1, Add2)
{
	Init ();
	GLFWwindow* window = CreateWindow("HLayoutTest1 - Add2", 640, 480);

	Context* context = Manage (new Context);
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);

	HLayout* hlayout = new HLayout;
	hlayout->SetPosition(100, 100);
	hlayout->Resize(400, 200);

#ifdef DEBUG
	hlayout->lock(true);
#endif

	Widget* widget1 = new Widget;
	widget1->SetExpand(true);
	Widget* widget2 = new Widget;
	widget2->SetExpand(true);

	hlayout->Add(Manage(widget1));
	hlayout->Add(Manage(widget2));

	context->Add(hlayout);

	RunLoop(window);

	delete hlayout;

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
TEST_F(HLayoutTest1, Add3)
{
	Init ();
	GLFWwindow* window = CreateWindow("HLayoutTest1 - Add3", 640, 480);

	Context* context = Manage (new Context);
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);

	HLayout* hlayout = new HLayout;
	hlayout->SetPosition(100, 100);
	hlayout->Resize(400, 200);

#ifdef DEBUG
	hlayout->lock(true);
#endif

	Widget* widget1 = new Widget;
	widget1->SetExpandX(false);
	widget1->SetExpandY(true);
	Widget* widget2 = new Widget;
	widget2->SetExpand(true);

	hlayout->Add(Manage(widget1));
	hlayout->Add(Manage(widget2));

	context->Add(hlayout);

	RunLoop(window);

	delete hlayout;

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * Test behavior if more than 1 expandable child with max width 
 *
 * Expected result: the widgets should fill the layout
 */
TEST_F(HLayoutTest1, Add4)
{
	Init ();
	GLFWwindow* window = CreateWindow("HLayoutTest1 - Add4", 640, 480);

	Context* context = Manage (new Context);
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);

	HLayout* hlayout = new HLayout;
	hlayout->SetPosition(100, 100);
	hlayout->Resize(400, 200);

	Widget* widget1 = new Widget;
	widget1->SetExpandX(true);
    widget1->SetPreferredSize(40, widget1->preferred_size().height());
    widget1->SetMaximalSize(80, widget1->maximal_size().height());
	Widget* widget2 = new Widget;
	widget2->SetExpandX(true);
    widget2->SetPreferredSize(40, widget2->preferred_size().height());
    widget2->SetMaximalSize(80, widget2->maximal_size().height());

	hlayout->Add(Manage(widget1));
	hlayout->Add(Manage(widget2));

	context->Add(hlayout);

	RunLoop(window);
	
	delete hlayout;

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * Test behavior if more than 1 expandable child with max width and 1 more fixed child
 *
 * Expected result: the widgets should fill the layout
 */
TEST_F(HLayoutTest1, Add5)
{
	Init ();
	GLFWwindow* window = CreateWindow("HLayoutTest1 - Add5", 640, 480);

	Context* context = Manage (new Context);
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);

	HLayout* hlayout = new HLayout;
	hlayout->SetPosition(100, 100);
	hlayout->Resize(600, 200);

	Widget* widget1 = new Widget;
	widget1->SetExpandX(true);
    //widget1->SetPreferredSize(40, widget1->preferred_size().height());
    //widget1->SetMaximalSize(80, widget1->maximal_size().height());
	Widget* widget2 = new Widget;
	widget2->SetExpandX(true);
    widget2->SetPreferredSize(40, widget2->preferred_size().height());
    widget2->SetMaximalSize(80, widget2->maximal_size().height());
	Widget* widget3 = new Widget;
	widget3->SetExpandX(true);
    widget3->SetPreferredSize(40, widget3->preferred_size().height());
    widget3->SetMaximalSize(60, widget3->maximal_size().height());
	Widget* widget4 = new Widget;
	//widget4->SetExpandX(true);
    //widget4->SetPreferredSize(40, widget4->preferred_size().height());
    //widget4->SetMaximalSize(70, widget4->maximal_size().height());
	widget4->Resize(40, widget4->size().height());
	
    hlayout->Add(Manage(widget1));
	hlayout->Add(Manage(widget2));
	hlayout->Add(Manage(widget3));
	hlayout->Add(Manage(widget4));

	context->Add(hlayout);

	RunLoop(window);

	delete hlayout;

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * Test behavior if more than 1 expandable child and 1 more fixed child in small size
 *
 * Expected result: the widgets should fill the layout
 */
TEST_F(HLayoutTest1, Add6)
{
	Init ();
	GLFWwindow* window = CreateWindow("HLayoutTest1 - Add6", 640, 480);

	Context* context = Manage (new Context);
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);

	HLayout* hlayout = new HLayout;
	hlayout->SetPosition(100, 100);
	hlayout->Resize(50, 200);

#ifdef DEBUG
	hlayout->lock(true);
#endif

	Widget* widget1 = new Widget;
	widget1->SetExpandX(true);
    //widget1->SetPreferredSize(40, widget1->preferred_size().height());
    //widget1->SetMaximalSize(80, widget1->maximal_size().height());
	Widget* widget2 = new Widget;
    widget2->SetMinimalSize(10, widget2->minimal_size().height());
	Widget* widget3 = new Widget;
    widget3->SetMinimalSize(20, widget3->minimal_size().height());
	Widget* widget4 = new Widget;
	widget4->SetExpandX(true);
	
    hlayout->Add(Manage(widget1));
	hlayout->Add(Manage(widget2));
	hlayout->Add(Manage(widget3));
	hlayout->Add(Manage(widget4));

	context->Add(hlayout);

	RunLoop(window);

	delete hlayout;

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * Test behavior if more than 1 expandable child and 1 more fixed child in small size
 *
 * Expected result: the widgets should fill the layout
 */
TEST_F(HLayoutTest1, Add7)
{
	Init ();
	GLFWwindow* window = CreateWindow("HLayoutTest1 - Add7", 640, 480);

	Context* context = Manage (new Context);
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);

	HLayout* hlayout = new HLayout;
	hlayout->SetPosition(100, 100);

	Widget* widget1 = new Widget;
	//widget1->SetExpandX(true);
	//widget1->SetMinimalSize(10, widget1->minimal_size().height());
	Widget* widget2 = new Widget;
    widget2->SetMinimalSize(30, widget2->minimal_size().height());
	Widget* widget3 = new Widget;
    widget3->SetMinimalSize(20, widget3->minimal_size().height());
	Widget* widget4 = new Widget;

	hlayout->Add(Manage(widget1));
	hlayout->Add(Manage(widget2));
	hlayout->Add(Manage(widget3));
	hlayout->Add(Manage(widget4));

	hlayout->Resize(80, 100);

	context->Add(hlayout);

	RunLoop(window);

	delete hlayout;

	Terminate();

	ASSERT_TRUE(true);
}


/**
 * Test resize
 *
 * Expected result: the children in the HLayout resize too according to the layout size
 */
TEST_F(HLayoutTest1, Resize1)
{
	Init();
	GLFWwindow* window = CreateWindow("HLayoutTest1 - Resize1", 640, 480);

	Context* context = Manage (new Context);
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);

	// add test code here

	HLayout* hlayout = new HLayout;
	hlayout->SetPosition(100, 100);

	Widget* widget1 = new Widget;
	widget1->SetExpand(true);
	Widget* widget2 = new Widget;
	widget2->SetExpand(true);

	hlayout->Add(Manage(widget1));
	hlayout->Add(Manage(widget2));

	hlayout->Resize(400, 200);

	context->Add(hlayout);

	RunLoop(window);

	delete hlayout;

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * Test resize, no child is expandable
 *
 * Expected result: the children in the HLayout resize too according to the layout size
 */
TEST_F(HLayoutTest1, Resize2)
{
	Init();
	GLFWwindow* window = CreateWindow("HLayoutTest1 - Resize2", 640, 480);

	Context* context = Manage (new Context);
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);

	// add test code here

	HLayout* hlayout = new HLayout;
	hlayout->SetPosition(100, 100);

	Widget* widget1 = new Widget;
	Widget* widget2 = new Widget;

	hlayout->Add(Manage(widget1));
	hlayout->Add(Manage(widget2));

	hlayout->Resize(400, 200);

	context->Add(hlayout);

	RunLoop(window);

	delete hlayout;

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * Test resize to small size
 *
 * Expected result: the children in the HLayout resize too according to the layout size
 */
TEST_F(HLayoutTest1, Resize3)
{
	Init();
	GLFWwindow* window = CreateWindow("HLayoutTest1 - Resize3", 640, 480);

	Context* context = Manage (new Context);
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);

	// add test code here

	HLayout* hlayout = new HLayout;
	hlayout->SetPosition(100, 100);

	Widget* widget1 = new Widget;
	Widget* widget2 = new Widget;
    widget2->SetMinimalSize(30, widget2->minimal_size().height());
	Widget* widget3 = new Widget;
    widget3->SetMinimalSize(20, widget3->minimal_size().height());
	Widget* widget4 = new Widget;

	hlayout->Add(Manage(widget1));
	hlayout->Add(Manage(widget2));
	hlayout->Add(Manage(widget3));
	hlayout->Add(Manage(widget4));

	hlayout->Resize(80, 100);

	context->Add(hlayout);

	RunLoop(window);

	delete hlayout;

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * Test SetMargin
 *
 * Expected result: the children in the HLayout resize too according to the new margin
 */
TEST_F(HLayoutTest1, SetMargin1)
{
	Init();
	GLFWwindow* window = CreateWindow("HLayoutTest1 - SetMargin1", 640, 480);

	Context* context = Manage (new Context);
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);

	// add test code here

	HLayout* hlayout = new HLayout;
	hlayout->SetPosition(100, 100);

	Widget* widget1 = new Widget;
    widget1->SetExpand(true);
    widget1->Resize(100, 80);
	Widget* widget2 = new Widget;
    widget2->SetExpand(true);
    widget2->Resize(100, 80);

	hlayout->Add(Manage(widget1));
	hlayout->Add(Manage(widget2));

    hlayout->Resize(400, 200);

    hlayout->SetMargin(20, 20, 20, 20);

	context->Add(hlayout);

	RunLoop(window);

	delete hlayout;

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * Test SetSpace
 *
 * Expected result: the children in the HLayout resize too according to the new space
 */
TEST_F(HLayoutTest1, SetSpace1)
{
	Init();
	GLFWwindow* window = CreateWindow("HLayoutTest1 - SetSpace1", 640, 480);

	Context* context = Manage (new Context);
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);

	// add test code here

	HLayout* hlayout = new HLayout;
	hlayout->SetPosition(100, 100);

	Widget* widget1 = new Widget;
    widget1->SetExpand(true);
    widget1->Resize(100, 80);
	Widget* widget2 = new Widget;
    widget2->SetExpand(true);
    widget2->Resize(100, 80);

	hlayout->Add(Manage(widget1));
	hlayout->Add(Manage(widget2));

    hlayout->Resize(400, 200);

    hlayout->SetSpace(10);

	context->Add(hlayout);

	RunLoop(window);

	delete hlayout;

	Terminate();

	ASSERT_TRUE(true);
}
