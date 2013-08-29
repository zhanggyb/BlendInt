/* Test case for FontFace */

#ifndef _LAYOUT_TEST_H
#define _LAYOUT_TEST_H

#include <iostream>
#include <string>

#include <BIL/Application.hpp>
#include <BIL/Window.hpp>

#include <cppunit/extensions/HelperMacros.h>

using namespace BIL;
using namespace std;

class LayoutTest: public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(LayoutTest);

	CPPUNIT_TEST(horizontal_layout1);

	CPPUNIT_TEST_SUITE_END()
	;

public:

	LayoutTest ();

	virtual ~LayoutTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void horizontal_layout1 ();

};

#endif  /* _LAYOUT_TEST_H */
