/* Test case for FontFace */

#ifndef _CONTEXTMANAGER_TEST_H
#define _CONTEXTMANAGER_TEST_H

#include <iostream>
#include <string>

#include <BlendInt/Label.hpp>

#include <cppunit/extensions/HelperMacros.h>

using namespace BlendInt;
using namespace std;

class ContextManagerTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(ContextManagerTest);

	CPPUNIT_TEST(check_layer_0_0);
	CPPUNIT_TEST(check_layer_0_1);
	CPPUNIT_TEST(check_layer_0_2);
	CPPUNIT_TEST(check_layer_0_3);
	CPPUNIT_TEST(check_layer_0_4);

	CPPUNIT_TEST_SUITE_END()
	;

public:

	ContextManagerTest ();

	virtual ~ContextManagerTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void check_layer_0_0 ();
	void check_layer_0_1 ();
	void check_layer_0_2 ();
	void check_layer_0_3 ();
	void check_layer_0_4 ();	// check set_z()

	static void cbError (int error, const char* description);

	static void cbWindowSize (GLFWwindow* window, int w, int h);

	static void cbKey (GLFWwindow* window, int key, int scancode, int action, int mods);

	static void cbMouseButton (GLFWwindow* window, int button, int action, int mods);

	static void cbCursorPos (GLFWwindow* window, double xpos, double ypos);

};

#endif  /* _CONTEXTMANAGER_TEST_H */
