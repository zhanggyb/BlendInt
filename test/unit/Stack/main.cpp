/*
 * Unit test for StackedWidget
 */

#include <Common/Window.hpp>

#include <BlendInt/Gui/Stack.hpp>
#include <BlendInt/Gui/ClockWidget.hpp>

// #include <gtest/gtest.h>
#include <boost/smart_ptr.hpp>

// The fixture for testing class Foo.

using namespace BlendInt;

/*
class StackedWidgetTest: public testing::Test
{

protected:

	StackedWidgetTest () {

	}

	virtual ~StackedWidgetTest () {

	}

	virtual void SetUp () {
		// Code here
	}

	virtual void TearDown () {

	}

};

TEST_F(StackedWidgetTest, CheckStackedWidget1)
{
}
*/

int main (int argc, char* argv[])
{
    BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

    Init ();

    GLFWwindow* window = CreateWindow("Timer Test");

    // add test code here

    RunLoop(window);

    Terminate();
	
	//return RUN_ALL_TESTS(); return 0;
}
