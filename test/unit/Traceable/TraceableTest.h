/* Test case for FontFace */

#ifndef _TRACEABLE_TEST_H
#define _TRACEABLE_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace std;

class TraceableTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(TraceableTest);

	CPPUNIT_TEST(checksolo1);
	CPPUNIT_TEST(checksolo2);
	CPPUNIT_TEST(checkparent1);
	CPPUNIT_TEST(checkparent2);
	CPPUNIT_TEST(checkparent3);
	CPPUNIT_TEST(checkparent4);
	CPPUNIT_TEST(checkparent5);
	CPPUNIT_TEST(checkparent6);
	CPPUNIT_TEST(checkparent7);
	CPPUNIT_TEST(checkparent8);
	CPPUNIT_TEST(checkparent9);
	CPPUNIT_TEST(checkparent10);

	CPPUNIT_TEST(solotest1);
	CPPUNIT_TEST(solotest2);

	CPPUNIT_TEST_SUITE_END()
	;

public:

	TraceableTest ();

	virtual ~TraceableTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void checksolo1 ();
	void checksolo2 ();
	void checkparent1 ();
	void checkparent2 ();
	void checkparent3 ();
	void checkparent4 ();
	void checkparent5 ();
	void checkparent6 ();
	void checkparent7 ();
	void checkparent8 ();
	void checkparent9 ();
	void checkparent10 ();
	void checkparent11 ();
	void checkparent12 ();

	void solotest1 ();
	void solotest2 ();

};

#endif  /* _TRACEABLE_TEST_H */
