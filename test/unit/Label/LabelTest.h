/* Test case for FontFace */

#ifndef _LABEL_TEST_H
#define _LABEL_TEST_H

#include <iostream>
#include <string>

#include <BIL/Application.hpp>
#include <BIL/Window.hpp>
#include <BIL/Label.hpp>

#include <cppunit/extensions/HelperMacros.h>

using namespace BIL;
using namespace std;

class LabelTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(LabelTest);

	CPPUNIT_TEST(show1);
	CPPUNIT_TEST(show2);
	CPPUNIT_TEST(checkfont1);
	CPPUNIT_TEST(checkfont2);

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
};

#endif  /* _LABEL_TEST_H */
