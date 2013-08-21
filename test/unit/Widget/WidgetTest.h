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

class myWidget1: public Widget
{
public:

	myWidget1 (Traceable* parent = NULL);

	virtual ~myWidget1 ();

protected:

	virtual void Render (void);
};

class myWidget2: public Widget
{
public:

	myWidget2 (Traceable* parent = NULL);

	virtual ~myWidget2 ();

protected:

	virtual void Render (void);
};

class WidgetTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(WidgetTest);

	CPPUNIT_TEST(mywidget1_show1);
	CPPUNIT_TEST(mywidget2_show1);

	CPPUNIT_TEST_SUITE_END()
	;

public:

	WidgetTest ();

	virtual ~WidgetTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void mywidget1_show1 ();

	void mywidget2_show1 ();

};

#endif  /* _WIDGET_TEST_H */
