#include "ContextManagerTest1.hpp"
#include <BlendInt/Service/ContextManager.hpp>
#include <BlendInt/Gui/Widget.hpp>
#include <Common/Window.hpp>

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
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ContextManagerTest1, Foo1)
{
    Init();

	GLFWwindow* win = CreateWindow("ContextManager - Foo1");

    // TODO: add test code here
    Widget* w1 = new Widget;
    w1->set_name("widget1");
    w1->SetPosition(200, 200);

    Widget* w2 = new Widget;
    w2->set_name("widget2");
    w2->SetPosition(400, 200);

    w1->Register();
    w2->Register();

    ContextManager::instance->print();

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
TEST_F(ContextManagerTest1, Layer1)
{
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

    w1->Register();
    w2->Register();

    ContextManager::instance->print();

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
TEST_F(ContextManagerTest1, Layer2)
{
    Init();

    GLFWwindow* win = CreateWindow("ContextManager - Layer2");

    // TODO: add test code here
    Widget* w1 = new Widget;
    w1->set_name("widget1");
    w1->SetPosition(200, 200);

    Widget* w2 = new Widget;
    w2->set_name("widget2");
    w2->SetPosition(400, 200);

    w1->Register();
    w2->Register();

    w2->SetLayer(1);

    ContextManager::instance->print();

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
