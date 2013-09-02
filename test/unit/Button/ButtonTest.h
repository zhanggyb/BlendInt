/* Test case for FontFace */

#ifndef _BUTTON_TEST_H
#define _BUTTON_TEST_H

#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class ButtonTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(ButtonTest);

	CPPUNIT_TEST(show1);
	CPPUNIT_TEST(toggle_button_show1);


	CPPUNIT_TEST_SUITE_END()
	;

public:

	ButtonTest ();

	virtual ~ButtonTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void show1 ();

	void toggle_button_show1 ();

	static void cbError (int error, const char* description);

	static void cbWindowSize (GLFWwindow* window, int w, int h);

	static void cbKey (GLFWwindow* window, int key, int scancode, int action, int mods);

	static void cbMouseButton (GLFWwindow* window, int button, int action, int mods);

	static void cbCursorPos (GLFWwindow* window, double xpos, double ypos);

};

#endif  /* _BUTTON_TEST_H */
