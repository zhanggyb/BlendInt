/*
 * Unit test for ClockWidget
 */

#include <Common/Window.hpp>

#include <BlendInt/ClockWidget.hpp>

//#include <gtest/gtest.h>
#include <boost/smart_ptr.hpp>

// The fixture for testing class Foo.

using namespace BlendInt;

/*
class ClockWidgetTest: public testing::Test
{

protected:

	ClockWidgetTest () {

	}

	virtual ~ClockWidgetTest () {

	}

	virtual void SetUp () {
		// Code here
	}

	virtual void TearDown () {

	}

};

TEST_F(ClockWidgetTest, CheckClockWidget1)
{
}
*/

int main (int argc, char* argv[])
{
    BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

    Init ();

    GLFWwindow* window = CreateWindow("Timer Test");

    // add test code here

	ClockWidget* tf = new ClockWidget;

	tf->SetPosition(200, 200);
    tf->Resize(400, 400);

	Interface::instance()->bind(tf);

    RunLoop(window);

    Terminate();
	
	//return RUN_ALL_TESTS(); return 0;
}
