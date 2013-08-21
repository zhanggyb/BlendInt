// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include "LabelTest.h"

#include <BIL/Label.hpp>
#include <BIL/FontConfig.hpp>

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(LabelTest);

LabelTest::LabelTest ()
{

}

LabelTest::~LabelTest ()
{

}

void LabelTest::setUp ()
{
}

void LabelTest::tearDown ()
{
}

void LabelTest::show1 ()
{
    Application app;

    Window win(640, 480, "640 x 480 Window", NULL, NULL);

    app.setMainWindow(&win);
    app.initialize();

    Label label(L"仁义礼智信");
    label.set_parent(&win);
    label.set_pos(Coord2f(50.0, 50.0));
    label.set_background(Color(0x4A898B80));
    //label.setFont(Font("Droid Sans", 24));
    label.setTextColor (Color(0x80F0B0FF));

    app.run();
    CPPUNIT_ASSERT(true);
}

void LabelTest::show2 ()
{
	CPPUNIT_ASSERT(true);
}

void LabelTest::checkfont1 ()
{
    Application app;

    Window win(640, 480, "640 x 480 Window", NULL, NULL);

    app.setMainWindow(&win);
    app.initialize();

    Label label(L"Hello World!");
    label.set_parent(&win);
    label.set_pos(Coord2f(50.0, 50.0));
    label.set_background(Color(0x40, 0x40, 0xFF, 0x80));
    label.setFont(Font("Droid Sans", 24));

    app.run();
    CPPUNIT_ASSERT(true);
}
