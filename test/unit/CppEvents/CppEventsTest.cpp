// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include "CppEventsTest.h"

#include <BIL/Application.hpp>
#include <BIL/Window.hpp>
#include <BIL/Label.hpp>
#include <BIL/Button.hpp>
#include <BIL/ToggleButton.hpp>

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(CppEventsTest);

CppEventsTest::CppEventsTest ()
{

}

CppEventsTest::~CppEventsTest ()
{

}

void CppEventsTest::setUp ()
{
}

void CppEventsTest::tearDown ()
{
}

void CppEventsTest::connect1 ()
{
	Application app;

	Window win(640, 480, "640 x 480 Window", NULL, NULL);

	app.setMainWindow(&win);
	app.initialize();

	Button button(L"Button Test");
	button.set_font(Font("Droid Sans"));
	button.setParent(&win);
	button.set_round_box_type(RoundBoxAll);
	button.set_pos(Point(50, 50));

	Label label(L"text will be changed");
	label.setParent(&win);
	label.setFont(Font("Droid Sans"));
	label.set_pos(Point(100, 100));

	scope_.connect(button.clicked(), &label, &Label::accept);

	app.run();

	CPPUNIT_ASSERT(true);
}
