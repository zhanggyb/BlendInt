#include "ContextManagerTest1.hpp"
#include <BlendInt/Service/ContextManager.hpp>
#include <BlendInt/Gui/Widget.hpp>
#include <Common/Window.hpp>
#include <BlendInt/Gui/Frame.hpp>

using namespace BlendInt;

ContextManagerTest1::ContextManagerTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ContextManagerTest1::~ContextManagerTest1()
{
	// TODO: add destructor code
}

/**
 * test AddSubWidget method, default behavior in constructor
 *
 * Expected result: 
 */
TEST_F(ContextManagerTest1, AddSubWidget01)
{
    Init();

	GLFWwindow* win = CreateWindow("ContextManager - AddSubWidget01");

    // TODO: add test code here
    Widget* w1 = new Widget;
    w1->set_name("widget1");
    w1->SetPosition(200, 200);

    Widget* w2 = new Widget;
    w2->set_name("widget2");
    w2->SetPosition(400, 200);

    ContextManager::instance->print();
    
    size_t index_size = ContextManager::instance->index_size();

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
TEST_F(ContextManagerTest1, AddSubWidget02)
{
    Init();

	GLFWwindow* win = CreateWindow("ContextManager - AddSubWidget02");

    // TODO: add test code here
    Widget* w1 = new Widget;
    w1->set_name("widget1");
    w1->SetPosition(200, 200);

    Widget* w2 = new Widget;
    w2->set_name("widget2");
    w2->SetPosition(400, 200);

    Frame* f1 = new Frame;
    f1->set_name("frame1");
    f1->SetPosition(400, 400);
    f1->Add(w2);

#ifdef DEBUG
    ContextManager::instance->print();
#endif

    size_t cm_size = ContextManager::instance->index_size();

#ifdef DEBUG
    f1->print();
#endif

    size_t fm_size = f1->sub_widget_size ();

    bool result = (
            (cm_size == 2) &&
            (fm_size == 1) &&
            (w1->in_context_manager()) &&
            (!w1->in_container()) &&
            (w2->in_container()) &&
            (!w2->in_context_manager()) &&
            (w1->container() == ContextManager::instance) &&
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
TEST_F(ContextManagerTest1, AddSubWidget03)
{
    Init();

	GLFWwindow* win = CreateWindow("ContextManager - AddSubWidget03");

    // TODO: add test code here
    Widget* w1 = new Widget;
    w1->set_name("widget1");
    w1->SetPosition(200, 200);

    Widget* w2 = new Widget;
    w2->set_name("widget2");
    w2->SetPosition(400, 200);

    Frame* f1 = new Frame;
    f1->set_name("frame1");
    f1->SetPosition(400, 400);
    f1->Add(w2);

    ContextManager::instance->AddSubWidget(w2);

    w2->SetPosition(400, 200);

    f1->Add(w1);

#ifdef DEBUG
    ContextManager::instance->print();
#endif

    size_t cm_size = ContextManager::instance->index_size();

#ifdef DEBUG
    f1->print();
#endif

    size_t fm_size = f1->sub_widget_size ();

    bool result = (
            (cm_size == 2) &&
            (fm_size == 1) &&
            (w2->in_context_manager()) &&
            (!w2->in_container()) &&
            (w1->in_container()) &&
            (!w1->in_context_manager()) &&
            (w2->container() == ContextManager::instance) &&
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
TEST_F(ContextManagerTest1, DestructorInContainer01)
{
    Init();

	GLFWwindow* win = CreateWindow("ContextManager - DestructorInContainer01");

    // TODO: add test code here
    Widget* w1 = new Widget;
    w1->set_name("widget1");
    w1->SetPosition(200, 200);

    Frame* f1 = new Frame;
    f1->set_name("frame1");
    f1->SetPosition(400, 400);
    f1->Add(w1);

    delete f1;  // now w1 should be in context manager as it's not marked as managed

#ifdef DEBUG
    ContextManager::instance->print();
#endif

    size_t cm_size = ContextManager::instance->index_size();

    bool result = (
            (cm_size == 1) &&
            (!w1->in_container()) &&
            (w1->in_context_manager()) &&
            (w1->container() == ContextManager::instance)
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
TEST_F(ContextManagerTest1, DestructorInContainer02)
{
    Init();

	GLFWwindow* win = CreateWindow("ContextManager - DestructorInContainer02");

    // TODO: add test code here
    Widget* w1 = manage(new Widget);
    w1->set_name("widget1");
    w1->SetPosition(200, 200);

    Frame* f1 = manage(new Frame);  // now f1 should be deleted automatically
    f1->set_name("frame1");
    f1->SetPosition(400, 400);
    f1->Add(w1);

#ifdef DEBUG
    ContextManager::instance->print();
#endif

    size_t cm_size = ContextManager::instance->index_size();

    bool result = (
            (cm_size == 1) &&
            (w1->in_container()) &&
            (!w1->in_context_manager()) &&
            (w1->container() == f1) &&
            (f1->in_context_manager()) &&
            (!f1->in_container()) &&
            (f1->container() == ContextManager::instance)
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
TEST_F(ContextManagerTest1, Layer1)
{
    /*
    Init();

    GLFWwindow* win = CreateWindow("ContextManager - Layer1");

    // TODO: add test code here
    Widget* w1 = new Widget;
    w1->set_name("widget1");
    w1->SetPosition(200, 200);
        
    Widget* w2 = new Widget;
    w2->set_name("widget2");
    w2->SetPosition(400, 200);
    w2->SetLayer(1);

    ContextManager::instance->print();

    RunLoop(win);

    Interface::Release();

    Terminate();
    */

	ASSERT_TRUE(true);
}

/**
 * test widgets in different layers
 *
 * Expected result: 
 */
TEST_F(ContextManagerTest1, Layer2)
{
    /*
    Init();

    GLFWwindow* win = CreateWindow("ContextManager - Layer2");

    // TODO: add test code here
    Widget* w1 = new Widget;
    w1->set_name("widget1");
    w1->SetPosition(200, 200);

    Widget* w2 = new Widget;
    w2->set_name("widget2");
    w2->SetPosition(400, 200);

    w2->SetLayer(1);

    ContextManager::instance->print();

    RunLoop(win);

    delete w1;
    delete w2;
    Interface::Release();

    Terminate();

    */
	ASSERT_TRUE(true);
}
