// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include <BIL/Interface.hpp>
#include <BIL/FontCache.hpp>
#include <BIL/FontConfig.hpp>
#include <BIL/String.hpp>

#include "FontCacheTest.h"

using namespace BILO;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(FontCacheTest);

FontCacheTest::FontCacheTest ()
{

}

FontCacheTest::~FontCacheTest ()
{

}

void FontCacheTest::setUp ()
{
	if(!FontConfig::initialize()) {
		CPPUNIT_FAIL("Cannot initialize FontConfig service\n");
	}
}

void FontCacheTest::tearDown ()
{
	FontConfig::release();
}

void FontCacheTest::create1 ()
{
	FontCache* cache = FontCache::create();

	FontCache::release();

	CPPUNIT_ASSERT(cache != NULL);
}

void FontCacheTest::create2 ()
{
	cout << endl << "reach here!" << endl;
	FontCache* cache = FontCache::create(Font("Droid Sans"));

	cout << "and reach here!" << endl;

	FontCache::release(Font("Droid Sans"));
	cout << "and finally reach here!" << endl;

	CPPUNIT_ASSERT(cache != NULL);
}

void FontCacheTest::create3 ()
{
	FontCache::create(Font("Droid Sans"));

	bool result = FontCache::release(Font("Sans"));

	FontCache::release(Font("Droid Sans"));

	CPPUNIT_ASSERT(!result);
}

void FontCacheTest::create4 ()
{
	FontCache* cache = FontCache::create(Font("Droid Sans Mono"));

	if (cache == NULL) {
		CPPUNIT_FAIL ("Cannot create cache for Droid Sans Mono\n");
	}

	cout << "Glyph width: " << cache->queryGlyph('A').bitmap_width << endl;

	FontCache::release (Font("Droid Sans Mono"));

	CPPUNIT_ASSERT(true);
}

void FontCacheTest::check1 ()
{
	FontCache* cache = FontCache::create(Font("Droid Sans Mono"));

	if(cache == NULL)
		CPPUNIT_FAIL ("Cannot create cache for Droid Sans Mono\n");

	bool result = true;

	if(result) {
		/*
		cache->queryGlyph(L'仁', true);
		cache->queryGlyph(L'义', true);
		cache->queryGlyph(L'礼', true);
		cache->queryGlyph(L'智', true);
		cache->queryGlyph(L'信', true);


		cache->queryGlyph(L'仁');
		cache->queryGlyph(L'仁');
		*/
		cache->queryGlyph('A');

#ifdef DEBUG
		cache->printcount();

		FontCache::list ();

#endif
	}

	FontCache::release (Font("Droid Sans Mono"));

#ifdef DEBUG
	FontCache::list();
#endif

	CPPUNIT_ASSERT (result);
}

void FontCacheTest::check2 ()
{
	FontCache* cache = FontCache::create(Font("Droid Sans Mono"));

	if(cache == NULL)
		CPPUNIT_FAIL ("Cannot create cache for Droid Sans Mono\n");

	bool result = true;

	if(result) {
#ifdef DEBUG
		cache->printcount();
		FontCache::list();
#endif
		cache->queryGlyph('A');
	}

	FontCache::release (Font("Droid Sans Mono"));

	CPPUNIT_ASSERT (result);
}

void FontCacheTest::check3 ()
{
	FontCache* cache = FontCache::create();
	//Glyph* glyph = NULL;

	if(cache == NULL)
		CPPUNIT_FAIL ("Cannot create cache for Droid Sans Mono\n");

	bool result = true;

	if(result) {
#ifdef DEBUG
		cache->printcount();
		FontCache::list();
#endif
		//glyph = cache->query('A');
	}

	FontCache::release ();

	CPPUNIT_ASSERT (result);
}

void FontCacheTest::check4 ()
{
	FontCache::create(Font("Sans"));
	FontCache::create(Font("Droid Sans"));
	FontCache::create(Font("Droid Serif"));
	FontCache::create(Font("Droid Sans Mono"));
	FontCache::create(Font("Droid Sans Thai"));

#ifdef DEBUG
		FontCache::list();
#endif

	FontCache::releaseAll ();
#ifdef DEBUG
		FontCache::list();
#endif

	CPPUNIT_ASSERT (true);
}

void FontCacheTest::check5 ()
{
	FontCache::setMaxCaches(6);
	FontCache::create(Font("Droid Sans"));
	FontCache::create(Font("Droid Serif"));
	FontCache::create(Font("Droid Sans Mono"));
	FontCache::create(Font("Droid Sans Thai"));
	FontCache::create(Font("Droid Sans Hebrew"));
	FontCache::create(Font("Droid Sans Fallback"));
	FontCache::create(Font("Droid Sans Japanese"));

#ifdef DEBUG
		FontCache::list();
#endif

	FontCache::releaseAll ();
#ifdef DEBUG
		FontCache::list();
#endif

		FontCache::setMaxCaches(32);

	CPPUNIT_ASSERT (true);
}

void FontCacheTest::check6 ()
{
	FontCache* cache = FontCache::create();

	if(cache == NULL)
		CPPUNIT_FAIL ("Cannot create cache for default font\n");

	bool result = true;

	if(result) {
		for(int i = 0; i < 10; i++)
		{
			//cache->queryGlyph(L'仁');
			cache->queryGlyph('A');
		}
		for(int i = 0; i < 5; i++)
		{
			//cache->queryGlyph(L'义');
			cache->queryGlyph('B');
		}
		for(int i = 0; i < 20; i++)
		{
			//cache->queryGlyph(L'智');
			cache->queryGlyph('C');
		}

#ifdef DEBUG
		cache->printcount();
#endif

	}

	FontCache::releaseAll ();

	CPPUNIT_ASSERT (true);
}

void FontCacheTest::check7 ()
{

	FontCache::setMaxCaches(6);
	FontCache* cache1 = FontCache::create(Font("Droid Sans"));
	FontCache* cache2 = FontCache::create(Font("Droid Sans"));

#ifdef DEBUG
		FontCache::list();
#endif

	FontCache::releaseAll ();

#ifdef DEBUG
		FontCache::list();
#endif

		FontCache::setMaxCaches(32);

	CPPUNIT_ASSERT (cache1 == cache2);
}

void FontCacheTest::check8 ()
{

	FontCache::setMaxCaches(6);
	FontCache* cache1 = FontCache::create(Font("Droid Sans"));
	bool result1 = true;
	FontCache* cache2 = FontCache::create(Font("Droid Sans", 12));
	bool result2 = true;

#ifdef DEBUG
		FontCache::list();
#endif

	FontCache::releaseAll ();

#ifdef DEBUG
		FontCache::list();
#endif

		FontCache::setMaxCaches(32);

	CPPUNIT_ASSERT ((cache1 != cache2) && result1 && result2);
}

void FontCacheTest::show1()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BIL", NULL, NULL);
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

	/* initialize BIL after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	FontCache* cache = FontCache::create(Font("Droid Sans Fallback", 24));

	if(cache == NULL)
		CPPUNIT_FAIL ("Cannot create cache for default font\n");

	bool result = true;
	if(!result) {
		CPPUNIT_FAIL("Cannot initialize font cache\n");
	}

	//cache->queryGlyph(L'信');
	cache->queryGlyph('D');

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {

		int width, height;

		glfwGetWindowSize(window, &width, &height);

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
		//glyph->Render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	/* release BIL */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void FontCacheTest::show_multiple_cache1()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BIL", NULL, NULL);
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

	/* initialize BIL after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	FontCache* cache1 = FontCache::create(Font("Lucida Grande", 48));

	FontCache* cache2 = FontCache::create(Font("Sans", 48));
	//FontCache* cache2 = FontCache::create(Font("Droid Sans Mono", 48));

	FontCache* cache3 = FontCache::create(Font("DejaVu Serif", 48));

	FontCache* cache4 = FontCache::create(Font("STXingkai", 48));
	//FontCache* cache4 = FontCache::create(Font("Bitstream Vera Sans", 48));

	String str1("Hello World! (cache1)");
	String str2(L"花间一壶酒，独酌无相亲。");
	//String str2("Hello World! (cache2)");
	String str3("Hello World! (cache3)");
	String str4(L"举杯邀明月，对影成三人。");
	//String str4("Hello World! (cache4)");
	String::const_iterator it;

#ifdef DEBUG
	FontCache::list();
#endif

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {

		int width, height;

		glfwGetWindowSize(window, &width, &height);

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
		glPushMatrix();
		glTranslatef(100.0f, 100.0f, 0.0f);
		for (it = str1.begin(); it != str1.end(); it++)
		{
			cache1->queryGlyph(*it);
			//glyph->Render();
			//glTranslatef(glyph->metrics().horiAdvance, 0, 0);
		}
		glPopMatrix();

		glPushMatrix();
		glTranslatef(100.0f, 200.0f, 0.0f);
		for (it = str2.begin(); it != str2.end(); it++)
		{
			cache2->queryGlyph(*it);
			//glyph->Render();
			//glTranslatef(glyph->metrics().horiAdvance, 0, 0);
		}
		glPopMatrix();

		glPushMatrix();
		glTranslatef(100.0f, 300.0f, 0.0f);
		for (it = str3.begin(); it != str3.end(); it++)
		{
			cache3->queryGlyph(*it);
			//glyph->Render();
			//glTranslatef(glyph->metrics().horiAdvance, 0, 0);
		}
		glPopMatrix();

		glPushMatrix();
		glTranslatef(100.0f, 400.0f, 0.0f);
		for (it = str4.begin(); it != str4.end(); it++)
		{
			cache4->queryGlyph(*it);
			//glyph->Render();
			//glTranslatef(glyph->metrics().horiAdvance, 0, 0);
		}
		glPopMatrix();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	/* release BIL */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void FontCacheTest::test_font_not_exist1()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BIL", NULL, NULL);
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

	/* initialize BIL after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	FontCache* cache1 = FontCache::create(Font("Lucida Grande", 48));

	FontCache* cache2 = FontCache::create(Font("Sans", 48));
	//FontCache* cache2 = FontCache::create(Font("Droid Sans Mono", 48));

	FontCache* cache3 = FontCache::create(Font("DejaVu Serif", 48));

	FontCache* cache4 = FontCache::create(Font("Droid Sans", 48));
	//FontCache* cache4 = FontCache::create(Font("Bitstream Vera Sans", 48));

	String str1("Hello World! (cache1)");
	String str2(L"花间一壶酒，独酌无相亲。");
	//String str2("Hello World! (cache2)");
	String str3("Hello World! (cache3)");
	String str4(L"举杯邀明月，对影成三人。");
	//String str4("Hello World! (cache4)");
	String::const_iterator it;

#ifdef DEBUG
	FontCache::list();
#endif

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {

		int width, height;

		glfwGetWindowSize(window, &width, &height);

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
		glPushMatrix();
		glTranslatef(100.0f, 100.0f, 0.0f);
		for (it = str1.begin(); it != str1.end(); it++)
		{
			cache1->queryGlyph(*it);
			//glyph->Render();
			//glTranslatef(glyph->metrics().horiAdvance, 0, 0);
		}
		glPopMatrix();

		glPushMatrix();
		glTranslatef(100.0f, 200.0f, 0.0f);
		for (it = str2.begin(); it != str2.end(); it++)
		{
			cache2->queryGlyph(*it);
			//glyph->Render();
			//glTranslatef(glyph->metrics().horiAdvance, 0, 0);
		}
		glPopMatrix();

		glPushMatrix();
		glTranslatef(100.0f, 300.0f, 0.0f);
		for (it = str3.begin(); it != str3.end(); it++)
		{
			cache3->queryGlyph(*it);
			//glyph->Render();
			//glTranslatef(glyph->metrics().horiAdvance, 0, 0);
		}
		glPopMatrix();

		glPushMatrix();
		glTranslatef(100.0f, 400.0f, 0.0f);
		for (it = str4.begin(); it != str4.end(); it++)
		{
			cache4->queryGlyph(*it);
			//glyph->Render();
			//glTranslatef(glyph->metrics().horiAdvance, 0, 0);
		}
		glPopMatrix();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	/* release BIL */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void FontCacheTest::cbError (int error, const char* description)
{
	std::cerr << "Error: " << description
			<< " (error code: " << error << ")"
			<< std::endl;
}

void FontCacheTest::cbWindowSize (GLFWwindow* window, int w, int h)
{
	BILO::Interface::instance()->resizeEvent(w, h);
}

void FontCacheTest::cbKey (GLFWwindow* window, int key, int scancode, int action,
        int mods)
{
	BILO::Interface::instance()->keyEvent(key, scancode, action, mods);
}

void FontCacheTest::cbMouseButton (GLFWwindow* window, int button, int action,
        int mods)
{
	BILO::Interface::instance()->mouseButtonEvent(button, action, mods);
}

void FontCacheTest::cbCursorPos (GLFWwindow* window, double xpos, double ypos)
{
	BILO::Interface::instance()->cursorPosEvent(xpos, ypos);
}
