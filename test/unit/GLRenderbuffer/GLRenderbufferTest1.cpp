#include "GLRenderbufferTest1.hpp"
#include <BlendInt/OpenGL/GLRenderbuffer.hpp>

using namespace BlendInt;

GLRenderbufferTest1::GLRenderbufferTest1()
: testing::Test()
{
	// TODO: add constructor code
}

GLRenderbufferTest1::~GLRenderbufferTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(GLRenderbufferTest1, OffScreenTest1)
{
	Init ();

    GLFWwindow* win = CreateWindow ("GLRenderbuffer - OffScreenTest1");

    // TODO: add test code here

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}
