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

void LabelTest::tearDown ()
{
	FontConfig::release();

	glfwTerminate();
}

void LabelTest::show1 ()
{
	GLFWwindow * win = glfwCreateWindow(640, 480, "TextBuffer Test", NULL,
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

	Label label(L"仁义礼智信");
	label.set_pos(Coord2f(50.0, 50.0));
	label.set_background(Color(0x4A898B80));
	//label.setFont(Font("Droid Sans", 24));
	label.setTextColor (Color(0x80F0B0FF));

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
		label.Render();

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	FontCache::releaseAll();

	CPPUNIT_ASSERT(true);
}

void LabelTest::show2 ()
{
	GLFWwindow * win = glfwCreateWindow(640, 480, "TextBuffer Test", NULL,
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

		glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();

		glPushMatrix();

		glTranslatef(100.0, 100.0, 0.0);

		glBegin(GL_QUADS);
			glColor3f (0.f, 1.0f, 1.0f);
			glVertex2f(0.0, 0.0);
			glVertex2f(100.0, 0.0);
			glVertex2f(100.0, 100.0);
			glVertex2f(0.0, 100.0);
			//glVertex3f(0.0, 0.0, 0.0);
		glEnd();

		glPopMatrix();

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	FontCache::releaseAll();

	CPPUNIT_ASSERT(true);
}

void LabelTest::checkfont1 ()
{
	GLFWwindow * win = glfwCreateWindow(640, 480, "TextBuffer Test", NULL,
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

	Label label(L"Hello World!");
	label.set_pos(Coord2f(50.0, 50.0));
	label.set_background(Color(0x40, 0x40, 0xFF, 0x80));
	label.setFont(Font("Droid Sans", 24));

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
		label.Render();

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	FontCache::releaseAll();

	CPPUNIT_ASSERT(true);
}
