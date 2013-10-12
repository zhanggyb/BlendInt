/* Test case for FontFace */

#ifndef _CPPEVENTS_TEST_H
#define _CPPEVENTS_TEST_H

#include <cppunit/extensions/HelperMacros.h>

#include <BlendInt/Button.hpp>
#include <BlendInt/Label.hpp>

using namespace std;

class CppEventsTest: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(CppEventsTest);

	CPPUNIT_TEST(connect1);

	CPPUNIT_TEST_SUITE_END();

public:

	CppEventsTest ();

	virtual ~CppEventsTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void connect1 ();

	Cpp::ConnectionScope scope_;

	static void cbError (int error, const char* description);

	static void cbWindowSize (GLFWwindow* window, int w, int h);

	static void cbKey (GLFWwindow* window, int key, int scancode, int action, int mods);

	static void cbMouseButton (GLFWwindow* window, int button, int action, int mods);

	static void cbCursorPos (GLFWwindow* window, double xpos, double ypos);
};

#endif  /* _CPPEVENTS_TEST_H */
