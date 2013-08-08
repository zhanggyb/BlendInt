// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include <BIL/Theme.h>
#include <BIL/FontConfig.h>
#include <BIL/Label.h>

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
	int ret = glfwInit();

	if (ret != GL_TRUE) {
		CPPUNIT_FAIL("Cannot initialize glfw\n");
	}

	FontConfig::instance();
	bool fontinit = FontConfig::getService()->initialize();
	if (!fontinit) {
		CPPUNIT_FAIL("Cannot initialize FontManager\n");
	}
	fontinit = FontConfig::getService()->loadDefaultFontToMem();
	if (!fontinit) {
		CPPUNIT_FAIL("Cannot load default font\n");
	}
}

void ThemeTest::tearDown ()
{
	FontConfig::release();

	glfwTerminate();
}

void ThemeTest::initialize1 ()
{
	float r = 0.0;

	Theme* theme = Theme::instance();

	if (theme != NULL)
		theme->initialize();

	//_themeUI.wcol_tool.outline = RGBAf(0.098, 0.098, 0.098);
	r = theme->getThemeUI()->wcol_tool.outline.rgba.r;

	if (theme != NULL) {
		Theme::release();
	}

	cout << "Red: " << r << endl;
	CPPUNIT_ASSERT((r - 0.098) < 0.000001);
}

void ThemeTest::initialize2 ()
{
	GLFWwindow * win = glfwCreateWindow(1024, 768, "Theme Test", NULL,
	NULL);

	if (win == NULL) {
		CPPUNIT_FAIL("Cannot create glfw window\n");
	}

	glfwMakeContextCurrent(win);

	// Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to initilize GLEW" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	Theme* theme = Theme::instance();

	if (theme != NULL)
		theme->initialize();

	//_themeUI.wcol_tool.outline = RGBAf(0.098, 0.098, 0.098);
	RGBAf color = theme->getThemeUI()->wcol_menu_item.item;
	RGBAf textcolor = theme->getThemeUI()->wcol_menu_item.text;

	Label label(L"Text in Label");
	label.setPos(Coord2i(50, 50));
	label.setBackground(color);
	label.setFont(Font("Droid Sans", 12));
	label.setTextColor(textcolor);

	while (!glfwWindowShouldClose(win)) {

		int width, height;

		glfwGetWindowSize(win, &width, &height);

		glClearColor(0.40, 0.40, 0.45, 1.00);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor4f(1.00, 1.00, 1.00, 1.00);

		// enable anti-alias
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.f, (float) width, 0.f, (float) height, 100.f, -100.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Test buffer render
		label.render();

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	Theme::release();

	FontCache::releaseAll();

	CPPUNIT_ASSERT(true);
}
