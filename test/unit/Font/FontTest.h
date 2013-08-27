/* Test case for FontFace */

#ifndef _FONT_TEST_H
#define _FONT_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace std;

class FontTest: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(FontTest);

	CPPUNIT_TEST(check1);
	CPPUNIT_TEST(check2);
	CPPUNIT_TEST(check3);
	CPPUNIT_TEST(check4);
	CPPUNIT_TEST(check5);
	CPPUNIT_TEST(check6);
	CPPUNIT_TEST(check7);

	CPPUNIT_TEST_SUITE_END();

public:

	FontTest ();

	virtual ~FontTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void check1 ();
	void check2 ();
	void check3 ();
	void check4 ();
	void check5 ();
	void check6 ();
	void check7 ();
};

#endif  /* _FONT_TEST_H */
