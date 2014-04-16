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
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);

    // TODO: add test code here
    Widget* w1 = new Widget;
    w1->set_name("widget1");
    w1->SetPosition(200, 200);

    Widget* w2 = new Widget;
    w2->set_name("widget2");
    w2->SetPosition(400, 200);

    context->Add(w1);
    context->Add(w2);

#ifdef DEBUG
    context->PrintLayers();
#endif
    
    size_t index_size = context->index_size();

    RunLoop(win);

    delete w1;
    delete w2;

    Interface::Release();

    Terminate();

	ASSERT_TRUE(index_size == 2);
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
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);

    // TODO: add test code here
    Widget* w1 = new Widget;
    w1->set_name("widget1");
    w1->SetPosition(200, 200);

    Widget* w2 = new Widget;
    w2->set_name("widget2");
    w2->SetPosition(400, 200);

    Frame* f1 = new Frame;
    f1->set_name("frame1");
    f1->SetPosition(240, 320);
    f1->Add(w2);

    context->Add(w1);
    context->Add(f1);

#ifdef DEBUG
    context->PrintLayers();
#endif

    size_t cm_size = context->index_size();

    bool result = (
            (cm_size == 2) &&
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
    context->set_name("Context");
    Interface::instance->SetCurrentContext(context);

    // TODO: add test code here
    Widget* w1 = new Widget;
    w1->set_name("widget1");
    w1->SetPosition(200, 200);

    Widget* w2 = new Widget;
    w2->set_name("widget2");
    w2->SetPosition(400, 200);

    Frame* f1 = new Frame;
    f1->set_name("frame1");
    f1->SetPosition(240, 320);
    f1->Add(w2);

    context->Add(w2);
    context->Add(f1);

    w2->SetPosition(400, 200);

    f1->Add(w1);

#ifdef DEBUG
    context->PrintLayers();
#endif

    size_t cm_size = context->index_size();

    bool result = (
            (cm_size == 2) &&
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
	context->set_name("Context");
	Interface::instance->SetCurrentContext(context);

	// TODO: add test code here
	Widget* w1 = new Widget;
	w1->set_name("widget1");
	w1->SetPosition(200, 200);

	Frame* f1 = new Frame;
	f1->set_name("frame1");
	f1->SetPosition(240, 320);

	f1->Add(w1);

	context->Add(f1);

	delete f1;
	context->Add(w1);
	// now w1 should be in context manager as it's not marked as managed

#ifdef DEBUG
	context->PrintLayers();
#endif

	size_t cm_size = context->index_size();

	bool result = (
			(cm_size == 1) &&
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
	context->set_name("Context");
	Interface::instance->SetCurrentContext(context);

	// TODO: add test code here
	Widget* w1 = Manage(new Widget);
	w1->set_name("widget1");
	w1->SetPosition(100, 100);
	context->Add(w1);

	Frame* f1 = Manage(new Frame);  // now f1 should be deleted automatically
	f1->set_name("frame1");
	f1->SetPosition(240, 320);
	f1->Add(w1);

	context->Add(f1);

#ifdef DEBUG
	context->PrintLayers();
#endif

	size_t cm_size = context->index_size();

	bool result = (
			(cm_size == 1) &&
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
	context->set_name("Context");
	Interface::instance->SetCurrentContext(context);

    // TODO: add test code here
    Widget* w1 = Manage(new Widget);
    w1->set_name("widget1");
    w1->SetPosition(200, 200);
    context->Add(w1);
        
    Widget* w2 = Manage(new Widget);
    w2->set_name("widget2");
    w2->SetPosition(400, 200);
    context->Add(w2);
    w2->SetLayer(1);

#ifdef DEBUG
    context->PrintLayers();
#endif

	size_t index_size = context->index_size();
	size_t layer_size = context->layer_size();

	bool result = (
					index_size == 2 &&
					layer_size == 2);

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
TEST_F(ContextTest1, Layer2)
{
    Init();

    GLFWwindow* win = CreateWindow("ContextManager - Layer2", 640, 480);

	Context* context = Manage (new Context);
	context->set_name("Context");
	Interface::instance->SetCurrentContext(context);

    // TODO: add test code here
    Widget* w1 = new Widget;
    w1->set_name("widget1");
    w1->SetPosition(200, 200);
    context->Add(w1);

    Widget* w2 = new Widget;
    w2->set_name("widget2");
    w2->SetPosition(400, 200);
    w2->SetLayer(1);
    context->Add(w2);

    Widget* w3 = new Widget;
    w3->set_name("widget3");
    w3->SetLayer(2);
    context->Add(w3);

    delete w3; w3 = 0;

#ifdef DEBUG
    context->PrintLayers();
#endif

    size_t index_size = context->index_size();
	size_t layer_size = context->layer_size();

	bool result = (
					index_size == 2 &&
					layer_size == 2);

    RunLoop(win);

    delete w1;
    delete w2;

    Interface::Release();

    Terminate();

	ASSERT_TRUE(result);
}
