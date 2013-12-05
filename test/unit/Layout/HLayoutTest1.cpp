
#include "HLayoutTest1.hpp"

#include <BlendInt/Interface.hpp>
#include <BlendInt/HorizontalLayout.hpp>
#include <BlendInt/ClockWidget.hpp>

#include <boost/smart_ptr.hpp>

using namespace BlendInt;

TEST_F(HLayoutTest1, CheckHLayout1)
{
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

    Interface::instance()->bind(hlayout);

    RunLoop(window);

    Terminate();

    ASSERT_TRUE(true);
}

