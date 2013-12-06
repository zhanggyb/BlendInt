
#include "HLayoutTest1.hpp"

#include <BlendInt/Interface.hpp>
#include <BlendInt/HorizontalLayout.hpp>
#include <BlendInt/ClockWidget.hpp>

#include <boost/smart_ptr.hpp>

using namespace BlendInt;

// test the default Add() method
TEST_F(HLayoutTest1, Add1)
{
    Init ();
    GLFWwindow* window = CreateWindow("Timer Test");
	
    // add test code here

	HorizontalLayout* hlayout = new HorizontalLayout;
	hlayout->SetPosition(200, 200);
    
    ClockWidget* clock1 = new ClockWidget;
    ClockWidget* clock2 = new ClockWidget;
    clock2->set_background(Color(100, 200, 200, 245));

    hlayout->Add(clock1);
    hlayout->Add(clock2);

    Interface::instance()->bind(hlayout);

    RunLoop(window);

    Terminate();

    ASSERT_TRUE(true);
}

TEST_F(HLayoutTest1, Add2)
{
    Init ();
    GLFWwindow* window = CreateWindow("Timer Test");
	
    // add test code here

	HorizontalLayout* hlayout = new HorizontalLayout;
	hlayout->SetPosition(200, 200);
    
    ClockWidget* clock1 = new ClockWidget;
    ClockWidget* clock2 = new ClockWidget;
    clock2->set_background(Color(100, 200, 200, 245));

	clock1->Resize(100, 100);
	clock2->Resize(100, 100);
	
	hlayout->Resize(400, 400);
	
    hlayout->Add(clock1);
    hlayout->Add(clock2);

    Interface::instance()->bind(hlayout);

    RunLoop(window);

    Terminate();

    ASSERT_TRUE(true);
}

TEST_F(HLayoutTest1, Resize1)
{
    Init ();
    GLFWwindow* window = CreateWindow("Timer Test");
	
    // add test code here

	HorizontalLayout* hlayout = new HorizontalLayout;
	hlayout->SetPosition(200, 200);
    
    ClockWidget* clock1 = new ClockWidget;
    ClockWidget* clock2 = new ClockWidget;
    clock2->set_background(Color(100, 200, 200, 245));

    hlayout->Add(clock1);
    hlayout->Add(clock2);

    Interface::instance()->bind(hlayout);

	hlayout->Resize(300, 150);
	
    RunLoop(window);

    Terminate();

    ASSERT_TRUE(true);
}

TEST_F(HLayoutTest1, Resize2)
{
    Init ();
    GLFWwindow* window = CreateWindow("Timer Test");
	
    // add test code here

	HorizontalLayout* hlayout = new HorizontalLayout;
	hlayout->SetPosition(200, 200);
    
    ClockWidget* clock1 = new ClockWidget;
    ClockWidget* clock2 = new ClockWidget;
    clock2->set_background(Color(100, 200, 200, 245));

    hlayout->Add(clock1);
    hlayout->Add(clock2);

    Interface::instance()->bind(hlayout);

    RunLoop(window);

    Terminate();

    ASSERT_TRUE(true);
}
