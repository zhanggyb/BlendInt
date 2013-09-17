/* Test case for FontFace */

#ifndef _SCROLLWIDGET_TEST_H
#define _SCROLLWIDGET_TEST_H

#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class ScrollWidgetTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(ScrollWidgetTest);

	CPPUNIT_TEST(scrollbar_test01);

	CPPUNIT_TEST_SUITE_END()
	;

public:

	ScrollWidgetTest ();

	virtual ~ScrollWidgetTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void scrollbar_test01 ();

	static void cbError (int error, const char* description);

	static void cbWindowSize (GLFWwindow* window, int w, int h);

	static void cbKey (GLFWwindow* window, int key, int scancode, int action, int mods);

	static void cbMouseButton (GLFWwindow* window, int button, int action, int mods);

	static void cbCursorPos (GLFWwindow* window, double xpos, double ypos);

};

#endif  /* _SCROLLWIDGET_TEST_H */
