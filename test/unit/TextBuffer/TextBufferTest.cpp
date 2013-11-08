// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include "TextBufferTest.h"

#include <BlendInt/Interface.hpp>
#include <BlendInt/TextBuffer.hpp>
#include <BlendInt/FontConfig.hpp>

using namespace BlendInt;
using namespace std;

//CPPUNIT_TEST_SUITE_REGISTRATION(TextBufferTest);

TextBufferTest::TextBufferTest ()
{

}

TextBufferTest::~TextBufferTest ()
{

}

void TextBufferTest::setUp ()
{

}

void TextBufferTest::tearDown ()
{
#ifdef DEBUG
	int mapsize = AbstractWidget::map_size();

	if(mapsize > 0) {
		map<uint64_t, AbstractWidget*>::const_iterator it;
		for (it = AbstractWidget::get_map().begin(); it != AbstractWidget::get_map().end(); it++)
		{
			cout << "id: " << it->first << " was not deleted!" << endl;
		}
	}

	CPPUNIT_ASSERT(mapsize == 0);
#endif
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
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BlendInt", NULL, NULL);
	if (!window) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	glfwSetWindowSizeCallback(window, &cbWindowSize);
	glfwSetKeyCallback(window, &cbKey);
	glfwSetMouseButtonCallback(window, &cbMouseButton);
	glfwSetCursorPosCallback(window, &cbCursorPos);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* initialize BlendInt after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	Font font("Sans", 16);

	TextBuffer buf;
	//TextBuffer buf;

	buf.setFont(font);
	buf.setOrigin(Coord3f(100.0, 100.0, 0.0));
	buf.append(L"A");

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {

		int width, height;

		glfwGetWindowSize(window, &width, &height);

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
		buf.render();

		glDisable(GL_BLEND);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	/* release BlendInt */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void TextBufferTest::showtextline1 ()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BlendInt", NULL, NULL);
	if (!window) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	glfwSetWindowSizeCallback(window, &cbWindowSize);
	glfwSetKeyCallback(window, &cbKey);
	glfwSetMouseButtonCallback(window, &cbMouseButton);
	glfwSetCursorPosCallback(window, &cbCursorPos);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* initialize BlendInt after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	TextBuffer buf;
	buf.setFont(Font("Sans",16));
	buf.setOrigin(Coord3f(100.0, 100.0, 0.0));
	buf.append(L"we have official packages");

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {

		int width, height;

		glfwGetWindowSize(window, &width, &height);

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
		buf.render();

		glDisable(GL_BLEND);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	/* release BlendInt */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void TextBufferTest::showtextline2 ()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BlendInt", NULL, NULL);
	if (!window) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	glfwSetWindowSizeCallback(window, &cbWindowSize);
	glfwSetKeyCallback(window, &cbKey);
	glfwSetMouseButtonCallback(window, &cbMouseButton);
	glfwSetCursorPosCallback(window, &cbCursorPos);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* initialize BlendInt after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	TextBuffer buf;
	buf.setFont(Font("Sans", 16));
	buf.setOrigin(Coord3f(100.0, 100.0, 0.0));
	buf.append(L"Our strong community is diverse and helpful");

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {

		int width, height;

		glfwGetWindowSize(window, &width, &height);

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
		buf.render();

		glDisable(GL_BLEND);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	/* release BlendInt */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void TextBufferTest::showtextline3 ()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BlendInt", NULL, NULL);
	if (!window) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	glfwSetWindowSizeCallback(window, &cbWindowSize);
	glfwSetKeyCallback(window, &cbKey);
	glfwSetMouseButtonCallback(window, &cbMouseButton);
	glfwSetCursorPosCallback(window, &cbCursorPos);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* initialize BlendInt after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	TextBuffer buf;
	buf.setFont(Font("Sans", 16));
	buf.setOrigin(Coord3f(100.0, 100.0, 0.0));
	buf.append(L"Arch Linux\n??????\n床前明月光");

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {

		int width, height;

		glfwGetWindowSize(window, &width, &height);

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
		buf.render();

		glDisable(GL_BLEND);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	/* release BlendInt */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void TextBufferTest::showtextline4 ()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BlendInt", NULL, NULL);
	if (!window) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	glfwSetWindowSizeCallback(window, &cbWindowSize);
	glfwSetKeyCallback(window, &cbKey);
	glfwSetMouseButtonCallback(window, &cbMouseButton);
	glfwSetCursorPosCallback(window, &cbCursorPos);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* initialize BlendInt after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	TextBuffer buf;
	buf.setFont(Font("Sans", 16));
	buf.setOrigin(Coord3f(100.0, 100.0, 0.0));
	buf.append(L"A simple, lightweight distribution\n??????\n疑是地上霜");

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {

		int width, height;

		glfwGetWindowSize(window, &width, &height);

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
		buf.render();

		glDisable(GL_BLEND);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	/* release BlendInt */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void TextBufferTest::multiple_buf_show1 ()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BlendInt", NULL, NULL);
	if (!window) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	glfwSetWindowSizeCallback(window, &cbWindowSize);
	glfwSetKeyCallback(window, &cbKey);
	glfwSetMouseButtonCallback(window, &cbMouseButton);
	glfwSetCursorPosCallback(window, &cbCursorPos);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* initialize BlendInt after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	String str1 = L"Hello World!";
	TextBuffer buf1(str1);
	buf1.setOrigin(Coord3f(100.0, 100.0, 0.0));
	buf1.setFont(Font("Droid Sans Mono", 48));

	String str2 = L"Hello World!";
	TextBuffer buf2(str2);
	buf2.setOrigin(Coord3f(100.0, 200.0, 0.0));
	buf2.setFont(Font("Source Code Pro", 48));

	String str3 = L"我爱北京天安门";
	TextBuffer buf3(str3);
	buf3.setOrigin(Coord3f(100.0, 300.0, 0.0));
	buf3.setFont(Font("Sans", 48));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {

		int width, height;

		glfwGetWindowSize(window, &width, &height);

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
		buf1.render();

		buf2.render();

		buf3.render();

		glDisable(GL_BLEND);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	/* release BlendInt */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void TextBufferTest::calculatebox1 ()
{
	if (!FontConfig::initialize()) {
		CPPUNIT_ASSERT(false);
		return;
	}

	TextBuffer buf;
	buf.setFont(Font("Sans", 16));
	buf.append(L"ArchLinux");

	//Size box = buf.calculateOutlineBoxSize();

	//cout << "Text Box: " << box.width() << " " << box.height() << endl;

	FontConfig::release();
	CPPUNIT_ASSERT(true);
}

void TextBufferTest::calculatebox2 ()
{
	if (!FontConfig::initialize()) {
		CPPUNIT_ASSERT(false);
		return;
	}

	TextBuffer buf;
	buf.setFont(Font("Sans", 16));
	buf.append(L"greetings, yes");

	//Rect box = buf.calculateOutline();

	//cout << "Text Box: " << box.left() << " " << box.bottom() << " " << box.right() << " " << box.top() << endl;

	FontConfig::release();
	CPPUNIT_ASSERT(true);
}

void TextBufferTest::cbError (int error, const char* description)
{
	std::cerr << "Error: " << description
			<< " (error code: " << error << ")"
			<< std::endl;
}

void TextBufferTest::cbWindowSize (GLFWwindow* window, int w, int h)
{
	BlendInt::Interface::instance()->resizeEvent(w, h);
}

void TextBufferTest::cbKey (GLFWwindow* window, int key, int scancode, int action,
        int mods)
{
	BlendInt::Interface::instance()->keyEvent(key, scancode, action, mods);
}

void TextBufferTest::cbMouseButton (GLFWwindow* window, int button, int action,
        int mods)
{
	BlendInt::Interface::instance()->mouseButtonEvent(button, action, mods);
}

void TextBufferTest::cbCursorPos (GLFWwindow* window, double xpos, double ypos)
{
	BlendInt::Interface::instance()->cursorPosEvent(xpos, ypos);
}
