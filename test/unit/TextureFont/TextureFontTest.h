/* Test case for FontFace */

#ifndef _TEXTUREFONT_TEST_H
#define _TEXTUREFONT_TEST_H

#include <iostream>
#include <string>

#include <BlendInt/Label.hpp>

#include <cppunit/extensions/HelperMacros.h>
#include <BlendInt/GLSLProgram.hpp>

using namespace BlendInt;
using namespace std;

class TextureFontTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(TextureFontTest);

	CPPUNIT_TEST(show1);
	CPPUNIT_TEST(show2);

	CPPUNIT_TEST_SUITE_END();

public:

	TextureFontTest ();

	virtual ~TextureFontTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	struct point {
		GLfloat x;
		GLfloat y;
		GLfloat s;
		GLfloat t;
	};

	void show1 ();
	void show2 ();

	void render_text (const char* text, float x, float y);

	static void cbError (int error, const char* description);

	static void cbWindowSize (GLFWwindow* window, int w, int h);

	static void cbKey (GLFWwindow* window, int key, int scancode, int action, int mods);

	static void cbMouseButton (GLFWwindow* window, int button, int action, int mods);

	static void cbCursorPos (GLFWwindow* window, double xpos, double ypos);

};

#endif  /* _TEXTUREFONT_TEST_H */
