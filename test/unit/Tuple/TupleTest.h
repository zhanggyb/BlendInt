/* Test case for FontFace */

#ifndef _TUPLE_TEST_H
#define _TUPLE_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace std;

class TupleTest: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TupleTest);

    CPPUNIT_TEST(create1);
    CPPUNIT_TEST(create2);
    CPPUNIT_TEST(create3);
    CPPUNIT_TEST(create4);
    CPPUNIT_TEST(create5);
    CPPUNIT_TEST(checkdefault1);

    CPPUNIT_TEST_SUITE_END();

 public:

    TupleTest();

    virtual ~TupleTest();

    void setUp();

    void tearDown();

    void runTest ();

 private:

    void create1 ();
    void create2 ();
    void create3 ();
    void create4 ();
    void create5 ();
    	void checkdefault1 ();
};

#endif  /* _TUPLE_TEST_H */
