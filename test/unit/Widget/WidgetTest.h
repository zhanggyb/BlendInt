/* Test case for FontFace */

#ifndef _WIDGET_TEST_H
#define _WIDGET_TEST_H

#include <BIL/Widget.hpp>

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace BIL;
using namespace std;

class testWidget: public Widget
{
public:

	testWidget (int type = 0, Drawable* parent = 0);

	virtual ~testWidget ()
	{
	}

protected:

	virtual void render ();
};

class WidgetTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(WidgetTest);

	CPPUNIT_TEST(widget_default_show);
	CPPUNIT_TEST(testwidget_show0);

	CPPUNIT_TEST(testwidget_show1);
	CPPUNIT_TEST(testwidget_show2);
	CPPUNIT_TEST(testwidget_show3);

	CPPUNIT_TEST_SUITE_END()
	;

public:

	WidgetTest ();

	virtual ~WidgetTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void widget_default_show ();

	void testwidget_show0 ();

	void testwidget_show1 ();

	void testwidget_show2 ();

	void testwidget_show3 ();

	static void cbError (int error, const char* description);

	static void cbWindowSize (GLFWwindow* window, int w, int h);

	static void cbKey (GLFWwindow* window, int key, int scancode, int action, int mods);

	static void cbMouseButton (GLFWwindow* window, int button, int action, int mods);

	static void cbCursorPos (GLFWwindow* window, double xpos, double ypos);

};

#endif  /* _WIDGET_TEST_H */
