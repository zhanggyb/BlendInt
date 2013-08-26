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
#include <BIL/Rect.hpp>

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
	label.set_pos(Point(50, 50));
	label.set_background(Color(0x4A898B80));
	//label.setFont(Font("Droid Sans", 24));
	label.setTextColor(Color(0x80F0B0FF));

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

	Window win(1024, 480, "1024 x 480 Window", NULL, NULL);

	app.setMainWindow(&win);
	app.initialize();

	wstring string(L"Hello World!");

	Label label(string);
	label.set_parent(&win);
	label.set_pos(Point(100, 100));
	label.set_background(Color(0x40, 0x40, 0xFF, 0x80));
	label.setFont(Font("Droid Sans", 100));

	app.run();
	CPPUNIT_ASSERT(true);
}

void LabelTest::checkfont2 ()
{
	Application app;

	Window win(640, 480, "640 x 480 Window", NULL, NULL);

	app.setMainWindow(&win);
	app.initialize();

	wstring string(L"Hello World!");

	Label instr1(wstring(L"box size"));
	instr1.set_parent(&win);
	instr1.setFont(Font("Droid Sans"));
	instr1.set_background(Color(0x40, 0x40, 0xFF, 0x00));
	instr1.set_pos(100, 200);

	Label label(string);
	label.set_parent(&win);
	label.set_pos(Point(100, 100));
	label.set_background(Color(0x40, 0x40, 0xFF, 0x80));
	label.setFont(Font("Droid Sans", 50));

	app.run();
	CPPUNIT_ASSERT(true);
}
