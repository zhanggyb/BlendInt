/* Test case for FontFace */

#ifndef _CHILDRENlIST_TEST_H
#define _CHILDRENlIST_TEST_H

#include <cppunit/extensions/HelperMacros.h>

using namespace BILO;
using namespace std;

class ChildrenListTest: public CppUnit::TestFixture
{

	CPPUNIT_TEST_SUITE(ChildrenListTest);

	CPPUNIT_TEST(check1);
	CPPUNIT_TEST(check2);
	CPPUNIT_TEST(check3);
	CPPUNIT_TEST(check4);
	CPPUNIT_TEST(check5);
	CPPUNIT_TEST(check6);
	CPPUNIT_TEST(check7);
	CPPUNIT_TEST(check8);
	CPPUNIT_TEST(check9);

	CPPUNIT_TEST_SUITE_END()
	;

public:

	ChildrenListTest ();

	virtual ~ChildrenListTest ();

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
	void check8 ();
	void check9 ();

};

#endif  /* _CHILDRENlIST_TEST_H */
