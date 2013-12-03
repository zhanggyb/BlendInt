/*
 * Unit test for TabFrame
 */

#include <Common/Window.hpp>

#include <BlendInt/TabFrame.hpp>
#include <BlendInt/ClockWidget.hpp>

// #include <gtest/gtest.h>
#include <boost/smart_ptr.hpp>

// The fixture for testing class Foo.

using namespace BlendInt;

/*
class TabFrameTest: public testing::Test
{

protected:

	TabFrameTest () {

	}

	virtual ~TabFrameTest () {

	}

	virtual void SetUp () {
		// Code here
	}

	virtual void TearDown () {

	}

};

TEST_F(TabFrameTest, CheckTabFrame1)
{
}
*/

int main (int argc, char* argv[])
{
    BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

    Init ();

    GLFWwindow* window = CreateWindow("Timer Test");

    // add test code here

	TabFrame* tf = new TabFrame;

	tf->SetPosition(200, 200);

    ClockWidget* clock1 = new ClockWidget;
    ClockWidget* clock2 = new ClockWidget;
    clock2->set_background(Color(255, 200, 210, 245));

    tf->Add("Button1", clock1);
    tf->Add("Button2", clock2);

	Interface::instance()->bind(tf);

    RunLoop(window);

    Terminate();
	
	//return RUN_ALL_TESTS(); return 0;
}
