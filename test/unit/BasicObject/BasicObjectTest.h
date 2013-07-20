/* Test case for FontFace */

#ifndef _BASICOBJECT_TEST_H
#define _BASICOBJECT_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace std;

class BasicObjectTest: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(BasicObjectTest);

    CPPUNIT_TEST(create1);
    CPPUNIT_TEST(remove);

    CPPUNIT_TEST_SUITE_END();

 public:

    BasicObjectTest();

    virtual ~BasicObjectTest();

    void setUp();

    void tearDown();

    void runTest ();

 private:

    void create1 ();
    void remove ();

};

#endif  /* _BASICOBJECT_TEST_H */
