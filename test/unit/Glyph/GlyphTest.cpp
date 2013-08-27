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

	FontEngine fe(Font("Sans", 24), 96);
	Glyph glyph (L'A', &fe);
	cout << endl;
	cout << "Metrics of a: width: " << glyph.metrics().width << " "
			<< " height: " << glyph.metrics().height << " "
			<< "bearingX: " << glyph.metrics().horiBearingX << " "
		 << "bearingY: " << glyph.metrics().horiBearingY << " "
		 << endl;


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

	FontEngine fe(Font("Sans", 12), 96);
	Glyph a(L'A', &fe);
	Glyph b(L'B', &fe);
	Glyph c(L'C', &fe);
	Glyph d(L'd', &fe);
	Glyph e(L'e', &fe);
	Glyph f(L'F', &fe);

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

	FontEngine fe (Font("Sans", 12), 96);
	Glyph a(L'A', &fe);
	Glyph b(L'b', &fe);
	Glyph c(L'仁', &fe);
	Glyph d(L'D', &fe);
	Glyph e(L'义', &fe);
	Glyph f(L'f', &fe);

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

	FontEngine fe(Font("Sans", 16), 96);
	Glyph a(L'a', &fe);
	Glyph b(L'b', &fe);
	Glyph c(L'c', &fe);
	Glyph d(L'd', &fe);
	Glyph e(L'e', &fe);
	Glyph f(L'f', &fe);
	Glyph g(L'g', &fe);
	Glyph h(L'h', &fe);
	Glyph i(L'i', &fe);
	Glyph j(L'j', &fe);

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

	Glyph a(L'p', &font);
	Glyph b(L'r', &font);
	Glyph c(L'i', &font);
	Glyph d(L'n', &font);
	Glyph e(L't', &font);
	Glyph f(L't', &font);
	Glyph g(L'e', &font);
	Glyph h(L'x', &font);
	Glyph i(L't', &font);
	Glyph j(L'4', &font);

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

	FontEngine fe (Font("Sans", 12), 96);
	Glyph a(L'A', &fe);
	Glyph b(L'a', &fe);

	FontConfig* fontserv = FontConfig::getService();
	FontEngine font(fontserv->getBuffer(), fontserv->getBufferSize(), 0, 12);

	Rect cbox = a.OutlineBox();
	cout << endl;
	cout << "Outline Box: " << cbox.x() << " " << cbox.y() << " " << cbox.width() << " " << cbox.height() << endl;
	cout << "Metrics of A: width: " << a.metrics().width << " height: "
		 << a.metrics().height << endl;
	cbox = b.OutlineBox();
	cout << "Outline Box: " << cbox.x() << " " << cbox.y() << " " << cbox.width() << " " << cbox.height() << endl;
	cout << "Metrics of a: width: " << b.metrics().width << " height: "
		 << b.metrics().height << endl;

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

	FontEngine fe(Font("Sans", 12), 96);
	Glyph a(L'A', &fe);
	Glyph b(L'b', &fe);
	Glyph c(L'仁',  &fe);
	Glyph d(L'D', &fe);
	Glyph e(L'义',  &fe);
	Glyph f(L'f', &fe);

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

	FontEngine fe(Font("Sans", 12), 96);
	Glyph a(L'A', &fe);
	Glyph b(L'b', &fe);
	Glyph c(L'仁',  &fe);
	Glyph d(L'D', &fe);
	Glyph e(L'义',  &fe);
	Glyph f(L'f', &fe);

	FontConfig* fontserv = FontConfig::getService();
	FontEngine font(fontserv->getBuffer(), fontserv->getBufferSize(), 0, 12);

	cout << endl;
	Rect cbox = a.OutlineBox();
	cout << "Outline Box: " << cbox.x() << " " << cbox.y() << " " << cbox.width() << " " << cbox.height() << endl;
	cout << "Metrics of a: width: " << a.metrics().width << " "
			<< " height: " << a.metrics().height << " "
			<< "bearingX: " << a.metrics().horiBearingX << " "
		 << "bearingY: " << a.metrics().horiBearingY << " "
		 << endl;

	cbox = b.OutlineBox();
	cout << "Outline Box: " << cbox.x() << " " << cbox.y() << " " << cbox.width() << " " << cbox.height() << endl;
	cout << "Metrics of b: width: " << b.metrics().width << " "
			<< " height: " << b.metrics().height << " "
			<< "bearingX: " << b.metrics().horiBearingX << " "
		 << "bearingY: " << b.metrics().horiBearingY << " "
		 << endl;

	cbox = c.OutlineBox();
	cout << "Outline Box: " << cbox.x() << " " << cbox.y() << " " << cbox.width() << " " << cbox.height() << endl;
	cout << "Metrics of c: width: " << c.metrics().width << " "
			<< " height: " << c.metrics().height << " "
			<< "bearingX: " << c.metrics().horiBearingX << " "
		 << "bearingY: " << c.metrics().horiBearingY << " "
		 << endl;

	cbox = d.OutlineBox();
	cout << "Outline Box: " << cbox.x() << " " << cbox.y() << " " << cbox.width() << " " << cbox.height() << endl;
	cout << "Metrics of d: width: " << d.metrics().width << " "
			<< " height: " << d.metrics().height << " "
			<< "bearingX: " << d.metrics().horiBearingX << " "
		 << "bearingY: " << d.metrics().horiBearingY << " "
		 << endl;

	cbox = e.OutlineBox();
	cout << "Outline Box: " << cbox.x() << " " << cbox.y() << " " << cbox.width() << " " << cbox.height() << endl;
	cout << "Metrics of e: width: " << e.metrics().width << " "
			<< " height: " << e.metrics().height << " "
			<< "bearingX: " << e.metrics().horiBearingX << " "
		 << "bearingY: " << e.metrics().horiBearingY << " "
		 << endl;

	cbox = f.OutlineBox();
	cout << "Outline Box: " << cbox.x() << " " << cbox.y() << " " << cbox.width() << " " << cbox.height() << endl;
	cout << "Metrics of f: width: " << f.metrics().width << " "
			<< " height: " << f.metrics().height << " "
			<< "bearingX: " << f.metrics().horiBearingX << " "
		 << "bearingY: " << f.metrics().horiBearingY << " "
		 << endl;

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
