// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <BIL/FontConfig.hpp>
#include <BIL/Glyph.hpp>
#include <iostream>
#include <string>

#include <BIL/Coord.hpp>

#include "GlyphTest.h"

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(GlyphTest);

GlyphTest::GlyphTest ()
{

}

GlyphTest::~GlyphTest ()
{

}

void GlyphTest::setUp ()
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

void GlyphTest::tearDown ()
{
	FontConfig::release();

	glfwTerminate();
}

void GlyphTest::create1 ()
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

	Glyph glyph (L'A', Font("Sans", 24), 96);

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

		glTranslatef (20, 20, 0);
		// Test buffer render
		glyph.Render();

		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	CPPUNIT_ASSERT(true);
}

void GlyphTest::create2 ()
{
	CPPUNIT_ASSERT(true);
}

void GlyphTest::printtext1 ()
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

	Glyph a(L'A', Font("Sans", 12), 96);
	Glyph b(L'B', Font("Sans", 12), 96);
	Glyph c(L'C', Font("Sans", 12), 96);
	Glyph d(L'd', Font("Sans", 12), 96);
	Glyph e(L'e', Font("Sans", 12), 96);
	Glyph f(L'F', Font("Sans", 12), 96);

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

		glTranslatef(20, 20, 0);	// where begin draw text

		a.Render();
		glTranslatef(a.metrics().horiAdvance, 0, 0);
		b.Render();
		glTranslatef(b.metrics().horiAdvance, 0, 0);
		c.Render();
		glTranslatef(c.metrics().horiAdvance, 0, 0);
		d.Render();
		glTranslatef(d.metrics().horiAdvance, 0, 0);
		e.Render();
		glTranslatef(e.metrics().horiAdvance, 0, 0);
		f.Render();

		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	CPPUNIT_ASSERT(true);
}

void GlyphTest::printtext2 ()
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

	Glyph a(L'A', Font("Sans", 12), 96);
	Glyph b(L'b', Font("Sans", 12), 96);
	Glyph c(L'仁', Font("Sans", 12), 96);
	Glyph d(L'D', Font("Sans", 12), 96);
	Glyph e(L'义', Font("Sans", 12), 96);
	Glyph f(L'f', Font("Sans", 12), 96);

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

		glTranslatef(20, 20, 0);	// where begin draw text

		a.Render();
		glTranslatef(a.metrics().horiAdvance, 0, 0);
		b.Render();
		glTranslatef(b.metrics().horiAdvance, 0, 0);
		c.Render();
		glTranslatef(c.metrics().horiAdvance, 0, 0);
		d.Render();
		glTranslatef(d.metrics().horiAdvance, 0, 0);
		e.Render();
		glTranslatef(e.metrics().horiAdvance, 0, 0);
		f.Render();

		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	CPPUNIT_ASSERT(true);
}

void GlyphTest::printtext3 ()
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

	Glyph a(L'a', Font("Sans", 16), 96);
	Glyph b(L'b', Font("Sans", 16), 96);
	Glyph c(L'c', Font("Sans", 16), 96);
	Glyph d(L'd', Font("Sans", 16), 96);
	Glyph e(L'e', Font("Sans", 16), 96);
	Glyph f(L'f', Font("Sans", 16), 96);
	Glyph g(L'g', Font("Sans", 16), 96);
	Glyph h(L'h', Font("Sans", 16), 96);
	Glyph i(L'i', Font("Sans", 16), 96);
	Glyph j(L'j', Font("Sans", 16), 96);

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

		glTranslatef(20, 20, 0);	// where begin draw text

		a.Render();
		glTranslatef(a.metrics().horiAdvance, 0, 0);
		b.Render();
		glTranslatef(b.metrics().horiAdvance, 0, 0);
		c.Render();
		glTranslatef(c.metrics().horiAdvance, 0, 0);
		d.Render();
		glTranslatef(d.metrics().horiAdvance, 0, 0);
		e.Render();
		glTranslatef(e.metrics().horiAdvance, 0, 0);
		f.Render();
		glTranslatef(f.metrics().horiAdvance, 0, 0);
		g.Render();
		glTranslatef(g.metrics().horiAdvance, 0, 0);
		h.Render();
		glTranslatef(h.metrics().horiAdvance, 0, 0);
		i.Render();
		glTranslatef(i.metrics().horiAdvance, 0, 0);
		j.Render();

		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	CPPUNIT_ASSERT(true);
}

void GlyphTest::printtext4 ()
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

	FontEngine font (FontConfig::getService()->getBuffer(),
			FontConfig::getService()->getBufferSize());

	Glyph a(L'p', Font("Sans", 12), 96, &font);
	Glyph b(L'r', Font("Sans", 12), 96, &font);
	Glyph c(L'i', Font("Sans", 12), 96, &font);
	Glyph d(L'n', Font("Sans", 12), 96, &font);
	Glyph e(L't', Font("Sans", 12), 96, &font);
	Glyph f(L't', Font("Sans", 12), 96, &font);
	Glyph g(L'e', Font("Sans", 12), 96, &font);
	Glyph h(L'x', Font("Sans", 12), 96, &font);
	Glyph i(L't', Font("Sans", 12), 96, &font);
	Glyph j(L'4', Font("Sans", 12), 96, &font);

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

		glTranslatef(20, 20, 0);	// where begin draw text

		a.Render();
		glTranslatef(a.metrics().horiAdvance, 0, 0);
		b.Render();
		glTranslatef(b.metrics().horiAdvance, 0, 0);
		c.Render();
		glTranslatef(c.metrics().horiAdvance, 0, 0);
		d.Render();
		glTranslatef(d.metrics().horiAdvance, 0, 0);
		e.Render();
		glTranslatef(e.metrics().horiAdvance, 0, 0);
		f.Render();
		glTranslatef(f.metrics().horiAdvance, 0, 0);
		g.Render();
		glTranslatef(g.metrics().horiAdvance, 0, 0);
		h.Render();
		glTranslatef(h.metrics().horiAdvance, 0, 0);
		i.Render();
		glTranslatef(i.metrics().horiAdvance, 0, 0);
		j.Render();

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	CPPUNIT_ASSERT(true);
}

void GlyphTest::checkbox1 ()
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

	Glyph a(L'A', Font("Sans", 12), 96);
	Glyph b(L'a', Font("Sans", 12), 96);

	FontConfig* fontserv = FontConfig::getService();
	FontEngine font(fontserv->getBuffer(), fontserv->getBufferSize(), 0, 12);

	cout << endl;
	cout << "Metrics of A: width: " << a.metrics().width << " height: "
		 << a.metrics().height << endl;
	cout << "Box of A: " << a.getBox().vec.x << ", " << a.getBox().vec.y << endl;
	cout << "Metrics of a: width: " << b.metrics().width << " height: "
		 << b.metrics().height << endl;
	cout << "Box of a: " << b.getBox().vec.x << ", " << b.getBox().vec.y << endl;

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

		glTranslatef(120, 120, 0);	// where begin draw text

		a.Render();
		glTranslatef(a.metrics().horiAdvance, 0, 0);
		b.Render();

		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	CPPUNIT_ASSERT(true);
}


void GlyphTest::checkkerning1 ()
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

	Glyph a(L'A', Font("Sans", 12), 96);
	Glyph b(L'b', Font("Sans", 12), 96);
	Glyph c(L'仁', Font("Sans", 12), 96);
	Glyph d(L'D', Font("Sans", 12), 96);
	Glyph e(L'义', Font("Sans", 12), 96);
	Glyph f(L'f', Font("Sans", 12), 96);

	FontConfig* fontserv = FontConfig::getService();
	FontEngine font(fontserv->getBuffer(), fontserv->getBufferSize(), 0, 12);

	Tuple2l kerning;

	cout << endl;
	kerning = font.getKerning(a,b);
	cout << "Kerning between A, b: " << kerning.vec.x << ", " << kerning.vec.y << endl;
	kerning = font.getKerning(b,c);
	cout << "Kerning between b, c: " << kerning.vec.x << ", " << kerning.vec.y << endl;
	kerning = font.getKerning(c,d);
	cout << "Kerning between c, D: " << kerning.vec.x << ", " << kerning.vec.y << endl;
	kerning = font.getKerning(d,e);
	cout << "Kerning between D, E: " << kerning.vec.x << ", " << kerning.vec.y << endl;
	kerning = font.getKerning(e,f);
	cout << "Kerning between E, f: " << kerning.vec.x << ", " << kerning.vec.y << endl;

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

		glTranslatef(20, 20, 0);	// where begin draw text

		a.Render();
		glTranslatef(a.metrics().horiAdvance, 0, 0);
		b.Render();
		glTranslatef(b.metrics().horiAdvance, 0, 0);
		c.Render();
		glTranslatef(c.metrics().horiAdvance, 0, 0);
		d.Render();
		glTranslatef(d.metrics().horiAdvance, 0, 0);
		e.Render();
		glTranslatef(e.metrics().horiAdvance, 0, 0);
		f.Render();

		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	CPPUNIT_ASSERT(true);
}

void GlyphTest::checkoutline1 ()
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

	Glyph a(L'A', Font("Sans", 12), 96);
	Glyph b(L'b', Font("Sans", 12), 96);
	Glyph c(L'仁', Font("Sans", 12), 96);
	Glyph d(L'D', Font("Sans", 12), 96);
	Glyph e(L'义', Font("Sans", 12), 96);
	Glyph f(L'f', Font("Sans", 12), 96);

	FontConfig* fontserv = FontConfig::getService();
	FontEngine font(fontserv->getBuffer(), fontserv->getBufferSize(), 0, 12);

	cout << endl;
	Rect rect = a.OutlineBox();
	cout << "Outline of a: " << rect.left() << " " << rect.bottom()
			<< " " << rect.right() << " " << rect.top() << endl;

	rect = b.OutlineBox();
	cout << "Outline of b: " << rect.left() << " " << rect.bottom()
			<< " " << rect.right() << " " << rect.top() << endl;

	rect = c.OutlineBox();
	cout << "Outline of c: " << rect.left() << " " << rect.bottom()
			<< " " << rect.right() << " " << rect.top() << endl;

	rect = d.OutlineBox();
	cout << "Outline of d: " << rect.left() << " " << rect.bottom()
			<< " " << rect.right() << " " << rect.top() << endl;

	rect = e.OutlineBox();
	cout << "Outline of e: " << rect.left() << " " << rect.bottom()
			<< " " << rect.right() << " " << rect.top() << endl;

	rect = f.OutlineBox();
	cout << "Outline of f: " << rect.left() << " " << rect.bottom()
			<< " " << rect.right() << " " << rect.top() << endl;

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

		glTranslatef(20, 20, 0);	// where begin draw text

		a.Render();
		glTranslatef(a.metrics().horiAdvance, 0, 0);
		b.Render();
		glTranslatef(b.metrics().horiAdvance, 0, 0);
		c.Render();
		glTranslatef(c.metrics().horiAdvance, 0, 0);
		d.Render();
		glTranslatef(d.metrics().horiAdvance, 0, 0);
		e.Render();
		glTranslatef(e.metrics().horiAdvance, 0, 0);
		f.Render();

		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	CPPUNIT_ASSERT(true);
}
