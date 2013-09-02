/* Test case for FontFace */

#ifndef _LAYOUT_TEST_H
#define _LAYOUT_TEST_H

#include <cppunit/extensions/HelperMacros.h>

class LayoutTest: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(LayoutTest);

	CPPUNIT_TEST(horizontal_layout1);

	CPPUNIT_TEST_SUITE_END()
	;

public:

	LayoutTest ();

	virtual ~LayoutTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void horizontal_layout1 ();

	static void cbError (int error, const char* description);

	static void cbWindowSize (GLFWwindow* window, int w, int h);

	static void cbKey (GLFWwindow* window, int key, int scancode, int action, int mods);

	static void cbMouseButton (GLFWwindow* window, int button, int action, int mods);

	static void cbCursorPos (GLFWwindow* window, double xpos, double ypos);

};

#endif  /* _LAYOUT_TEST_H */
