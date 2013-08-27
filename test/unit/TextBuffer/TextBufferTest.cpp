// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include "TextBufferTest.h"

#include <BIL/TextBuffer.hpp>
#include <BIL/FontConfig.hpp>

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

void TextBufferTest::draw_grid(int width, int height)
{
	// Draw grid for debug
	const int small_step = 20;
	const int big_step = 100;

	glLineWidth(1);
	glEnable(GL_LINE_STIPPLE);

	glColor4f (1.0f, 1.0f, 1.0f, 0.1f);
	glLineStipple (1, 0xAAAA);
	for (int num = 1; num < width; num++)
	{
		int step = num * small_step;
		glBegin (GL_LINES);
			glVertex2i(0, step);
			glVertex2i(width, step);
		glEnd();

	}
	for (int num = 1; num < height; num++)
	{
		int step = num * small_step;
		glBegin (GL_LINES);
			glVertex2i(step, 0);
			glVertex2i(step, height);
		glEnd();
	}

	glColor4f (1.0f, 1.0f, 1.0f, 0.25f);
	glLineStipple (1, 0xAAAA);
	for (int num = 1; num < width; num++)
	{
		int step = num * big_step;
		glBegin (GL_LINES);
		glVertex2i(0, step);
		glVertex2i(width, step);
		glEnd();
	}

	for (int num = 1; num < height; num++)
	{
		int step = num * big_step;
		glBegin (GL_LINES);
			glVertex2i(step, 0);
			glVertex2i(step, height);
		glEnd();
	}

	glDisable(GL_LINE_STIPPLE);
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

	TextBuffer buf;
	//TextBuffer buf;

	buf.set_font(font);
	buf.set_origin(Coord3f(100.0, 100.0, 0.0));
	buf.Append(L"A");

	while (!glfwWindowShouldClose(win)) {

		int width, height;

		glfwGetWindowSize(win, &width, &height);

		glClearColor(0.40, 0.40, 0.45, 1.00);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor4f(1.00, 1.00, 1.00, 1.00);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// enable anti-alias
		glEnable(GL_BLEND);

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.f, (float) width, 0.f, (float) height, 100.f, -100.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		draw_grid (width, height);
		// Test buffer render
		buf.Render();

		glDisable(GL_BLEND);
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

	TextBuffer buf;
	buf.set_font(Font("Sans",16));
	buf.set_origin(Coord3f(100.0, 100.0, 0.0));
	buf.Append(L"we have official packages");

	while (!glfwWindowShouldClose(win)) {

		int width, height;

		glfwGetWindowSize(win, &width, &height);

		glClearColor(0.40, 0.40, 0.45, 1.00);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor4f(1.00, 1.00, 1.00, 1.00);

		// enable anti-alias
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.f, (float) width, 0.f, (float) height, 100.f, -100.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		draw_grid (width, height);

		// Test buffer render
		buf.Render();

		glDisable(GL_BLEND);

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

	TextBuffer buf;
	buf.set_font(Font("Sans", 16));
	buf.set_origin(Coord3f(100.0, 100.0, 0.0));
	buf.Append(L"Our strong community is diverse and helpful");

	while (!glfwWindowShouldClose(win)) {

		int width, height;

		glfwGetWindowSize(win, &width, &height);

		glClearColor(0.40, 0.40, 0.45, 1.00);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor4f(1.00, 1.00, 1.00, 1.00);

		// enable anti-alias
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.f, (float) width, 0.f, (float) height, 100.f, -100.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		draw_grid (width, height);

		// Test buffer render
		buf.Render();

		glDisable(GL_BLEND);
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

	TextBuffer buf;
	buf.set_font(Font("Sans", 16));
	buf.set_origin(Coord3f(100.0, 100.0, 0.0));
	buf.Append(L"Arch Linux\n??????\n床前明月光");

	while (!glfwWindowShouldClose(win)) {

		int width, height;

		glfwGetWindowSize(win, &width, &height);

		glClearColor(0.40, 0.40, 0.45, 1.00);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor4f(1.00, 1.00, 1.00, 1.00);

		// enable anti-alias
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.f, (float) width, 0.f, (float) height, 100.f, -100.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		draw_grid (width, height);

		// Test buffer render
		buf.Render();

		glDisable(GL_BLEND);
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

	TextBuffer buf;
	buf.set_font(Font("Sans", 16));
	buf.set_origin(Coord3f(100.0, 100.0, 0.0));
	buf.Append(L"A simple, lightweight distribution\n??????\n疑是地上霜");

	while (!glfwWindowShouldClose(win)) {

		int width, height;

		glfwGetWindowSize(win, &width, &height);

		glClearColor(0.40, 0.40, 0.45, 1.00);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor4f(1.00, 1.00, 1.00, 1.00);

		// enable anti-alias
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.f, (float) width, 0.f, (float) height, 100.f, -100.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		draw_grid (width, height);

		// Test buffer render
		buf.Render();

		glDisable(GL_BLEND);
		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	FontCache::releaseAll();

	CPPUNIT_ASSERT(true);
}

void TextBufferTest::multiple_buf_show1 ()
{
	GLFWwindow * win = glfwCreateWindow(640, 480,
			"TextBuffer Test -- Multiple buffer", NULL, NULL);

	if (!win) {
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

	String str1 = L"Hello World!";
	TextBuffer buf1(str1);
	buf1.set_origin(Coord3f(100.0, 100.0, 0.0));
	buf1.set_font(Font("Droid Sans Mono", 48));

	String str2 = L"Hello World!";
	TextBuffer buf2(str2);
	buf2.set_origin(Coord3f(100.0, 200.0, 0.0));
	buf2.set_font(Font("Source Code Pro", 48));

	String str3 = L"我爱北京天安门";
	TextBuffer buf3(str3);
	buf3.set_origin(Coord3f(100.0, 300.0, 0.0));
	buf3.set_font(Font("Sans", 48));

	while (!glfwWindowShouldClose(win)) {

		int width, height;

		glfwGetWindowSize(win, &width, &height);

		glClearColor(0.40, 0.40, 0.45, 1.00);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor4f(1.00, 1.00, 1.00, 1.00);

		// enable anti-alias
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.f, (float) width, 0.f, (float) height, 100.f, -100.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		draw_grid (width, height);

		// Test buffer render
		buf1.Render();

		buf2.Render();

		buf3.Render();

		glDisable(GL_BLEND);
		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	FontCache::list();

	FontCache::releaseAll();

	CPPUNIT_ASSERT(true);
}

void TextBufferTest::calculatebox1 ()
{
	TextBuffer buf;
	buf.set_font(Font("Sans", 16));
	buf.Append(L"ArchLinux");

	Size box = buf.CalculateOutlineBoxSize();

	cout << "Text Box: " << box.width() << " " << box.height() << endl;

	CPPUNIT_ASSERT(true);
}
