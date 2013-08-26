/* Test case for FontFace */

#ifndef _STRING_TEST_H
#define _STRING_TEST_H

#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class StringTest: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(StringTest);

    CPPUNIT_TEST(create1);
    CPPUNIT_TEST(create2);
    CPPUNIT_TEST(create3);
    CPPUNIT_TEST(create4);
    CPPUNIT_TEST(create5);
    CPPUNIT_TEST_SUITE_END();

 public:

    StringTest();

    virtual ~StringTest();

    void setUp();

    void tearDown();

    void runTest ();

 private:

    void create1 ();
    void create2 ();
    void create3 ();
    void create4 ();
    void create5 ();
};

#endif  /* _STRING_TEST_H */
