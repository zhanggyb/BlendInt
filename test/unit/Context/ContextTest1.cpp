#include "ContextTest1.hpp"
#include <Common/Window.hpp>
#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Gui/Frame.hpp>
#include <BlendInt/Interface.hpp>

using namespace BlendInt;

ContextTest1::ContextTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ContextTest1::~ContextTest1()
{
	// TODO: add destructor code
}

/**
 * test AddSubWidget method, default behavior in constructor
 *
 * Expected result: 
 */
TEST_F(ContextTest1, AddSubWidget01)
{
    Init();

	GLFWwindow* win = CreateWindow("ContextManager - AddSubWidget01", 640, 480);

	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    // TODO: add test code here
    Widget* w1 = new Widget;
    w1->MoveTo(200, 200);

    Widget* w2 = new Widget;
    w2->MoveTo(400, 200);

    context->Append(w1);
    context->Append(w2);

    RunLoop(win);

    delete w1;
    delete w2;

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}

/**
 * test AddSubWidget method, default behavior in constructor
 *
 * Expected result: 
 */
TEST_F(ContextTest1, AddSubWidget02)
{
    Init();

	GLFWwindow* win = CreateWindow("ContextManager - AddSubWidget02", 640, 480);

	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    // TODO: add test code here
    Widget* w1 = new Widget;
    w1->MoveTo(200, 200);

    Widget* w2 = new Widget;
    w2->MoveTo(400, 200);

    Panel* f1 = new Panel;
    f1->MoveTo(240, 320);
    f1->Setup(w2);

    context->Append(w1);
    context->Append(f1);

    bool result = (
            (w1->container() == context) &&
            (w2->container() == f1)
            );

    RunLoop(win);

    delete w1;
    delete w2;
    delete f1;
    Interface::Release();

    Terminate();

	ASSERT_TRUE(result);
}

/**
 * test AddSubWidget method, default behavior in constructor
 *
 * Expected result: 
 */
TEST_F(ContextTest1, AddSubWidget03)
{
    Init();

	GLFWwindow* win = CreateWindow("ContextManager - AddSubWidget03", 640, 480);

	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    // TODO: add test code here
    Widget* w1 = new Widget;
    w1->MoveTo(200, 200);

    Widget* w2 = new Widget;
    w2->MoveTo(400, 200);

    Panel* f1 = new Panel;
    f1->MoveTo(240, 320);
    f1->Setup(w2);

    context->Append(w2);
    context->Append(f1);

    w2->MoveTo(400, 200);

    f1->Setup(w1);

    bool result = (
            (w2->container() == context) &&
            (w1->container() == f1)
            );

    RunLoop(win);

    delete w1;
    delete w2;
    delete f1;
    Interface::Release();

    Terminate();

	ASSERT_TRUE(result);
}

/**
 * test destructor in container
 *
 * Expected result: 
 */
TEST_F(ContextTest1, DestructorInContainer01)
{
	Init();

	GLFWwindow* win = CreateWindow("ContextManager - DestructorInContainer01", 640, 480);

	Context* context = Manage (new Context);
	Interface::instance->SetCurrentContext(context);

	// TODO: add test code here
	Widget* w1 = new Widget;
	w1->MoveTo(200, 200);

	Panel* f1 = new Panel;
	f1->MoveTo(240, 320);

	f1->Setup(w1);

	context->Append(f1);

	delete f1;
	context->Append(w1);
	// now w1 should be in context manager as it's not marked as managed

	bool result = (
			(w1->container() == context)
	);

	RunLoop(win);

	delete w1;
	Interface::Release();

	Terminate();

	ASSERT_TRUE(result);
}

/**
 * test destructor in container
 *
 * Expected result: 
 */
TEST_F(ContextTest1, DestructorInContainer02)
{
	Init();

	GLFWwindow* win = CreateWindow("ContextManager - DestructorInContainer02", 640, 480);

	Context* context = Manage (new Context);
	Interface::instance->SetCurrentContext(context);

	// TODO: add test code here
	Widget* w1 = Manage(new Widget);
	w1->MoveTo(100, 100);
	context->Append(w1);

	Panel* f1 = Manage(new Panel);  // now f1 should be deleted automatically
	f1->MoveTo(240, 320);
	f1->Setup(w1);

	context->Append(f1);

	bool result = (
			(w1->container() == f1) &&
			(f1->container() == context)
	);

	RunLoop(win);

	Interface::Release();

	Terminate();

	ASSERT_TRUE(result);
}

/**
 * test widgets in different layers
 *
 * Expected result: 
 */
TEST_F(ContextTest1, Layer1)
{
    Init();

    GLFWwindow* win = CreateWindow("ContextManager - Layer1", 640, 480);

	Context* context = Manage (new Context);
	Interface::instance->SetCurrentContext(context);

    // TODO: add test code here
    Widget* w1 = Manage(new Widget);
    w1->MoveTo(200, 200);
    context->Append(w1);
        
    Widget* w2 = Manage(new Widget);
    w2->MoveTo(400, 200);
    context->Append(w2);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}

/**
 * test widgets in different layers
 *
 * Expected result: 
 */
TEST_F(ContextTest1, Layer2)
{
    Init();

    GLFWwindow* win = CreateWindow("ContextManager - Layer2", 640, 480);

	Context* context = Manage (new Context);
	Interface::instance->SetCurrentContext(context);

    // TODO: add test code here
    Widget* w1 = new Widget;
    w1->MoveTo(200, 200);
    context->Append(w1);

    Widget* w2 = new Widget;
    w2->MoveTo(400, 200);
    context->Append(w2);

    Widget* w3 = new Widget;
    context->Append(w3);

    delete w3; w3 = 0;

    RunLoop(win);

    delete w1;
    delete w2;

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}

/**
 * test widgets in different layers
 *
 * Expected result:
 */
TEST_F(ContextTest1, Layer3)
{
    Init();

    GLFWwindow* win = CreateWindow("ContextManager - Layer3", 640, 480);

	Context* context = Manage (new Context);
	Interface::instance->SetCurrentContext(context);

	Widget* w1 = Manage(new Widget);
	w1->MoveTo(100, 100);
	context->Append(w1);

	Widget* w2 = Manage(new Widget);
	w2->MoveTo(150, 150);
	context->Append(w2);

	Widget* w3 = Manage(new Widget);
	w3->MoveTo(200, 200);
	context->Append(w3);

	Widget* w4 = Manage(new Widget);
	w4->MoveTo(250, 250);
	context->Append(w4);

	delete w4;
	delete w3;
	delete w2;

	Widget* w5 = Manage(new Widget);
	w5->MoveTo(300, 300);
	context->Append(w5);

	Widget* w6 = Manage(new Widget);
	w6->MoveTo(350, 350);
	context->Append(w6);

	RunLoop(win);

	delete w1;
	delete w2;

	Interface::Release();

	Terminate();

	ASSERT_TRUE(true);
}
