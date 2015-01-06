#include "ThemeTest1.hpp"
#include <BlendInt/Core/Object.hpp>
#include <BlendInt/Gui/Context.hpp>

#include <iostream>

#include <list>
#include <set>

using namespace BlendInt;

ThemeTest1::ThemeTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ThemeTest1::~ThemeTest1()
{
	// TODO: add destructor code
}

TEST_F(ThemeTest1, Save1)
{
	Init ();
	GLFWwindow* window = CreateWindow("Theme - Save1", 640, 480);

	// TODO: add test code here
	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

	Context::theme->Save("theme.xml");

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}

/**
 * test new Object 
 *
 * Expected result: 
 */
TEST_F(ThemeTest1, Load1)
{
	Init ();
	GLFWwindow* window = CreateWindow("Theme - Load1", 640, 480);

	// TODO: add test code here
	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

	Context::theme->Load("theme.xml");

	RunLoop(window);

	Terminate();

	ASSERT_TRUE(true);
}
