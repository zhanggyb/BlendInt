/* Test case for FontFace */

#ifndef _GLYPH_TEST_H
#define _GLYPH_TEST_H

#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class GlyphTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(GlyphTest);

	CPPUNIT_TEST(create1);
	//CPPUNIT_TEST(create2);
	CPPUNIT_TEST(printtext1);
	CPPUNIT_TEST(printtext2);
	CPPUNIT_TEST(printtext3);
	CPPUNIT_TEST(printtext4);
	CPPUNIT_TEST(checkbox1);
	CPPUNIT_TEST(checkoutline1);
	CPPUNIT_TEST(checkkerning1);

	CPPUNIT_TEST_SUITE_END()
	;

public:

	GlyphTest ();

	virtual ~GlyphTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void create1 ();
	void create2 ();
	void printtext1 ();
	void printtext2 ();
	void printtext3 ();
	void printtext4 ();
	void checkbox1 ();
	void checkkerning1 ();
	void checkoutline1 ();

	static void cbError (int error, const char* description);

	static void cbWindowSize (GLFWwindow* window, int w, int h);

	static void cbKey (GLFWwindow* window, int key, int scancode, int action, int mods);

	static void cbMouseButton (GLFWwindow* window, int button, int action, int mods);

	static void cbCursorPos (GLFWwindow* window, double xpos, double ypos);

};

#endif  /* _GLYPH_TEST_H */
