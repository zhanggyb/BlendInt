/* Test case for FontFace */

#ifndef _LABEL_TEST_H
#define _LABEL_TEST_H

#include <iostream>
#include <string>

#include <BIL/Label.hpp>

#include <cppunit/extensions/HelperMacros.h>

using namespace BILO;
using namespace std;

class LabelTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(LabelTest);

	CPPUNIT_TEST(show1);
	CPPUNIT_TEST(show2);
	CPPUNIT_TEST(checkfont1);
	CPPUNIT_TEST(checkfont2);
	//CPPUNIT_TEST(multiline1);	// multiline not supported

	CPPUNIT_TEST_SUITE_END()
	;

public:

	LabelTest ();

	virtual ~LabelTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void show1 ();
	void show2 ();

	void checkfont1 ();
	void checkfont2 ();

	void calculatebox1 ();
	void multiline1 ();

	static void cbError (int error, const char* description);

	static void cbWindowSize (GLFWwindow* window, int w, int h);

	static void cbKey (GLFWwindow* window, int key, int scancode, int action, int mods);

	static void cbMouseButton (GLFWwindow* window, int button, int action, int mods);

	static void cbCursorPos (GLFWwindow* window, double xpos, double ypos);

};

#endif  /* _LABEL_TEST_H */
