/* Test case for FontFace */

#ifndef _TEXTUREATLAS_TEST_H
#define _TEXTUREATLAS_TEST_H

#include <iostream>
#include <string>

#include <BILO/Label.hpp>

#include <cppunit/extensions/HelperMacros.h>

using namespace BILO;
using namespace std;

class TextureAtlasTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(TextureAtlasTest);

	CPPUNIT_TEST(show1);
	CPPUNIT_TEST(show2);

	CPPUNIT_TEST_SUITE_END();

public:

	TextureAtlasTest ();

	virtual ~TextureAtlasTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void show1 ();

	void show2 ();

	static void cbError (int error, const char* description);

	static void cbWindowSize (GLFWwindow* window, int w, int h);

	static void cbKey (GLFWwindow* window, int key, int scancode, int action, int mods);

	static void cbMouseButton (GLFWwindow* window, int button, int action, int mods);

	static void cbCursorPos (GLFWwindow* window, double xpos, double ypos);

};

#endif  /* _TEXTUREATLAS_TEST_H */
