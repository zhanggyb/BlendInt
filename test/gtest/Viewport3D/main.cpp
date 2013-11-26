/*
 * Unit test for Viewport3D
 */

#include <Common/Window.hpp>

#include <BlendInt/Viewport3D.hpp>
#include <BlendInt/Button.hpp>

#include <gtest/gtest.h>

// The fixture for testing class Foo.

using namespace BlendInt;

class Viewport3DTest: public testing::Test
{

protected:

	Viewport3DTest () {

	}

	virtual ~Viewport3DTest () {

	}

	virtual void SetUp () {
		// Code here
	}

	virtual void TearDown () {

	}

};

TEST_F(Viewport3DTest, CheckTimeOut)
{
    // TODO
}

int main (int argc, char* argv[])
{
    BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

    Init ();

    GLFWwindow* window = CreateWindow("Timer Test");

    // add test code here
    Viewport3D* view = new Viewport3D;

    view->Resize(800, 600);
    view->SetPosition(100, 100);

    Button* button = new Button("OK");
    button->SetPosition(1000, 400);

    Interface::instance()->bind(view);
    Interface::instance()->bind(button);

    RunLoop(window);

    Terminate();
	
	//return RUN_ALL_TESTS();
	return 0;
}

