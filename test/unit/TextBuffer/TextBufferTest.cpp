// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include "TextBufferTest.h"

#include <BIL/TextBuffer.h>
#include <BIL/FontConfig.h>

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(TextBufferTest);

TextBufferTest::TextBufferTest ()
{

}

TextBufferTest::~TextBufferTest ()
{

}

void TextBufferTest::setUp ()
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

void TextBufferTest::tearDown ()
{
    FontConfig::release();

    glfwTerminate();
}

void TextBufferTest::showcharacter1 ()
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

	Font font("Sans", 16);

    TextBuffer buf (font);
    //TextBuffer buf;

    buf.setOrigin(Coord3f(100.0, 100.0, 0.0));
    buf.append(L"A");

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
		buf.render();

		glfwSwapBuffers(win);
		glfwPollEvents();
    }

	FontCache::releaseAll();

    CPPUNIT_ASSERT(true);
}

void TextBufferTest::showtextline1 ()
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

    TextBuffer buf(Font("Sans", 16));
    buf.setOrigin(Coord3f(100.0, 100.0, 0.0));
    buf.append(L"we have official packages");

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
		buf.render();

		glfwSwapBuffers(win);
		glfwPollEvents();
    }

	FontCache::releaseAll();

    CPPUNIT_ASSERT(true);
}

void TextBufferTest::showtextline2 ()
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

    TextBuffer buf(Font("Sans", 16));
    buf.setOrigin(Coord3f(100.0, 100.0, 0.0));
    buf.append(L"Our strong community is diverse and helpful");

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
		buf.render();

		glfwSwapBuffers(win);
		glfwPollEvents();
    }

	FontCache::releaseAll();

    CPPUNIT_ASSERT(true);
}

void TextBufferTest::showtextline3 ()
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

    TextBuffer buf(Font("Sans", 16));
    buf.setOrigin(Coord3f(100.0, 100.0, 0.0));
    buf.append(L"Arch Linux\n??????\n床前明月光");

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
		buf.render();

		glfwSwapBuffers(win);
		glfwPollEvents();
    }

	FontCache::releaseAll();

    CPPUNIT_ASSERT(true);
}

void TextBufferTest::showtextline4 ()
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

    TextBuffer buf(Font("Sans", 16));
    buf.setOrigin(Coord3f(100.0, 100.0, 0.0));
    buf.append(L"A simple, lightweight distribution\n??????\n疑是地上霜");

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
		buf.render();

		glfwSwapBuffers(win);
		glfwPollEvents();
    }

	FontCache::releaseAll();

    CPPUNIT_ASSERT(true);
}

void TextBufferTest::calculatebox1 ()
{
    TextBuffer buf(Font("Sans", 16));
    buf.append(L"ArchLinux");

	Vec2ui box = buf.calculateBox ();

	cout << "Text Box: " << box.vec.x << " " << box.vec.y << endl;

	CPPUNIT_ASSERT(true);
}
