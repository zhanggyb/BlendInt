/* Test case for FontFace */

#ifndef _FONTCACHE_TEST_H
#define _FONTCACHE_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace std;

class FontCacheTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(FontCacheTest);

	CPPUNIT_TEST(create1);
	CPPUNIT_TEST(create2);
	CPPUNIT_TEST(create3);
	CPPUNIT_TEST(create4);

	CPPUNIT_TEST(check1);
	CPPUNIT_TEST(check2);
	CPPUNIT_TEST(check3);
	CPPUNIT_TEST(check4);
	CPPUNIT_TEST(check5);
	CPPUNIT_TEST(check6);
	CPPUNIT_TEST(check7);
	CPPUNIT_TEST(check8);
	CPPUNIT_TEST(show1);

	CPPUNIT_TEST_SUITE_END()
	;

public:

	FontCacheTest ();

	virtual ~FontCacheTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void create1 ();
	void create2 ();
	void create3 ();
	void create4 ();

	void check1 ();
	void check2 ();
	void check3 ();
	void check4 ();
	void check5 ();
	void check6 ();
	void check7 ();
	void check8 ();
	void show1 ();

};

#endif  /* _FONTCACHE_TEST_H */
