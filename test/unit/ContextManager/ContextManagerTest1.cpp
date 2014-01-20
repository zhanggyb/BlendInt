#include "ContextManagerTest1.hpp"
#include <BlendInt/ContextManager.hpp>
#include <BlendInt/Widget.hpp>
#include <BlendInt/Window.hpp>

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
    Window::Initialize();

	Window* win = Window::Create("ContextManager - Foo1", 1280, 800);

    if(!Interface::Initialize()) {
        Window::Release();
        ASSERT_TRUE(false);
    }

    Interface::Instance()->Resize(1280, 800);

    // TODO: add test code here
    Widget* w1 = new Widget;
    w1->set_name("widget1");
    w1->SetPosition(200, 200);

    Widget* w2 = new Widget;
    w2->set_name("widget2");
    w2->SetPosition(400, 200);

    w1->Register();
    w2->Register();

    ContextManager::Instance()->print();

    Window::Run();

    Interface::Release();

    Window::Release();

	ASSERT_TRUE(true);
}

/**
 * test widgets in different layers
 *
 * Expected result: 
 */
TEST_F(ContextManagerTest1, Layer1)
{
    Window::Initialize();

    Window* win = Window::Create("ContextManager - Layer1", 1280, 800);

    if(!Interface::Initialize()) {
        Window::Release();
        ASSERT_TRUE(false);
    }

    Interface::Instance()->Resize(1280, 800);

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

    ContextManager::Instance()->print();

    Window::Run();

    Interface::Release();

    Window::Release();

	ASSERT_TRUE(true);
}

/**
 * test widgets in different layers
 *
 * Expected result: 
 */
TEST_F(ContextManagerTest1, Layer2)
{
    Window::Initialize();

    Window* win = Window::Create("ContextManager - Layer2", 1280, 800);

    if(!Interface::Initialize()) {
        Window::Release();
        ASSERT_TRUE(false);
    }

    Interface::Instance()->Resize(1280, 800);

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

    ContextManager::Instance()->print();

    Window::Run();

    Interface::Release();

    Window::Release();

	ASSERT_TRUE(true);
}
