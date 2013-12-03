/*
 * Unit test for Layout
 */

#include <Common/Window.hpp>

#include <BlendInt/HorizontalLayout.hpp>
#include <BlendInt/VerticalLayout.hpp>
#include <BlendInt/ClockWidget.hpp>

#include <gtest/gtest.h>
#include <boost/smart_ptr.hpp>

// The fixture for testing class Foo.

using namespace BlendInt;

class LayoutTest: public testing::Test
{

protected:

	LayoutTest () {

	}

	virtual ~LayoutTest () {

	}

	virtual void SetUp () {
		// Code here
	}

	virtual void TearDown () {

	}

};

TEST_F(LayoutTest, CheckLayout1)
{
}

int main (int argc, char* argv[])
{
    BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

    Init ();

    GLFWwindow* window = CreateWindow("Timer Test");

    // add test code here

	HorizontalLayout* hlayout = new HorizontalLayout;
	hlayout->SetPosition(200, 200);
    
    ClockWidget* clock1 = new ClockWidget;
    ClockWidget* clock2 = new ClockWidget;
    clock2->set_background(Color(100, 200, 200, 245));

    hlayout->add(clock1);
    hlayout->add(clock2);

    //hlayout->Resize(200, 80);
    
    VerticalLayout* vlayout = new VerticalLayout;
    
    ClockWidget* clock3 = new ClockWidget;
    clock3->set_background(Color(150, 60, 180, 245));
    ClockWidget* clock4 = new ClockWidget;
    clock4->set_background(Color(190, 230, 20, 245));

    vlayout->add(clock3);
    vlayout->add(clock4);

    hlayout->add(vlayout);

	Interface::instance()->bind(hlayout);

    RunLoop(window);

    Terminate();
	
	//return RUN_ALL_TESTS(); return 0;
}
