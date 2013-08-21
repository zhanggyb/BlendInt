// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include "AbstractButtonTest.h"

#include <BIL/Application.hpp>
#include <BIL/Window.hpp>
#include <BIL/Label.hpp>
#include <BIL/Button.hpp>

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(AbstractButtonTest);

AbstractButtonTest::AbstractButtonTest ()
{

}

AbstractButtonTest::~AbstractButtonTest ()
{

}

void AbstractButtonTest::setUp ()
{
}

void AbstractButtonTest::tearDown ()
{
}

void AbstractButtonTest::show1 ()
{
	Application app;

	Window win(640, 480, "640 x 480 Window", NULL, NULL);

	app.setMainWindow(&win);
	app.initialize();

	Button button(L"The Default Value");
	button.set_parent(&win);
	button.set_pos(Coord2f(50.0, 50.0));
	button.set_background(RGBAf(1.0, 0.9, 1.0, 0.9));
	button.set_corner_radius(10.0);

	app.run();

	CPPUNIT_ASSERT(true);
}
