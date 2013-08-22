/* Test case for FontFace */

#ifndef _WIDGET_TEST_H
#define _WIDGET_TEST_H

#include <BIL/Application.hpp>
#include <BIL/Window.hpp>
#include <BIL/Widget.hpp>

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace BIL;
using namespace std;

class testWidget: public Widget
{
public:

	testWidget (int type = 0, Traceable* parent = NULL);

	virtual ~testWidget ()
	{
	}

protected:

	virtual void Render ();
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

};

#endif  /* _WIDGET_TEST_H */
