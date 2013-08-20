/* Test case for FontFace */

#ifndef _VECTOR_TEST_H
#define _VECTOR_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace std;

class VectorTest: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(VectorTest);

    CPPUNIT_TEST(create1);
    CPPUNIT_TEST(create2);
    CPPUNIT_TEST(create3);
    CPPUNIT_TEST(create4);
    CPPUNIT_TEST(create5);
    CPPUNIT_TEST(create6);
    CPPUNIT_TEST(create7);

    CPPUNIT_TEST(checkdefault1);

    CPPUNIT_TEST_SUITE_END();

 public:

    VectorTest();

    virtual ~VectorTest();

    void setUp();

    void tearDown();

    void runTest ();

 private:

    void create1 ();
    void create2 ();
    void create3 ();
    void create4 ();
    void create5 ();
    void create6 ();
    void create7 ();
	void checkdefault1 ();
};

#endif  /* _VECTOR_TEST_H */
