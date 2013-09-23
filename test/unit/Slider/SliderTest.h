/* Test case for FontFace */

#ifndef _SLIDER_TEST_H
#define _SLIDER_TEST_H

#include <cppunit/extensions/HelperMacros.h>

#include <BILO/Label.hpp>

#include <BILO/String.hpp>

class myLabel: public BILO::Label
{
public:
	myLabel (const BILO::String& label = "0");

	virtual ~myLabel();

	void print_value (int value);
};

class SliderTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(SliderTest);

	CPPUNIT_TEST(slider_move_test01);

	CPPUNIT_TEST_SUITE_END()
	;

public:

	SliderTest ();

	virtual ~SliderTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void slider_move_test01 ();

	static void cbError (int error, const char* description);

	static void cbWindowSize (GLFWwindow* window, int w, int h);

	static void cbKey (GLFWwindow* window, int key, int scancode, int action, int mods);

	static void cbMouseButton (GLFWwindow* window, int button, int action, int mods);

	static void cbCursorPos (GLFWwindow* window, double xpos, double ypos);

};

#endif  /* _SCROLLWIDGET_TEST_H */
