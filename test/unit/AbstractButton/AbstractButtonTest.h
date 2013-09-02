/* Test case for FontFace */

#ifndef _ABSTRACTBUTTON_TEST_H
#define _ABSTRACTBUTTON_TEST_H

#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class AbstractButtonTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(AbstractButtonTest);

	CPPUNIT_TEST(show1);

	CPPUNIT_TEST_SUITE_END()
	;

public:

	AbstractButtonTest ();

	virtual ~AbstractButtonTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void show1 ();

	static void cbError (int error, const char* description);

	static void cbWindowSize (GLFWwindow* window, int w, int h);

	static void cbKey (GLFWwindow* window, int key, int scancode, int action, int mods);

	static void cbMouseButton (GLFWwindow* window, int button, int action, int mods);

	static void cbCursorPos (GLFWwindow* window, double xpos, double ypos);
};

#endif  /* _ABSTRACTBUTTON_TEST_H */
