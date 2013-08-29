// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include <BIL/Label.hpp>
#include <BIL/Application.hpp>
#include <BIL/Window.hpp>
#include <BIL/Theme.hpp>

#include "ThemeTest.h"

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(ThemeTest);

ThemeTest::ThemeTest ()
{

}

ThemeTest::~ThemeTest ()
{

}

void ThemeTest::setUp ()
{
}

void ThemeTest::tearDown ()
{
}

void ThemeTest::initialize1 ()
{
	unsigned char r = 0x00;

	Theme* theme = Theme::instance();

	if (theme != NULL)
		theme->initialize();

	//_themeUI.wcol_tool.outline = RGBAf(0.098, 0.098, 0.098);
	r = theme->themeUI()->wcol_tool.outline.r();

	if (theme != NULL) {
		Theme::release();
	}

	cout << "Red: " << r << endl;
	CPPUNIT_ASSERT(r == 0x19);
}

void ThemeTest::initialize2 ()
{
    Application app;

    Window win(640, 480, "640 x 480 Window", NULL, NULL);

    app.setMainWindow(&win);
    app.initialize();

    Theme* theme = Theme::instance();
	//_themeUI.wcol_tool.outline = RGBAf(0.098, 0.098, 0.098);
	Color bg_color = theme->themeUI()->wcol_menu_item.item;
	Color textcolor = theme->themeUI()->wcol_menu_item.text;

	Label label(L"Text in Label");
	label.setParent(&win);
	label.set_pos(Point(50, 50));
	label.set_background(bg_color);
	label.setFont(Font("Droid Sans", 12));
	label.setTextColor(textcolor);

    app.run();

	CPPUNIT_ASSERT(true);
}
