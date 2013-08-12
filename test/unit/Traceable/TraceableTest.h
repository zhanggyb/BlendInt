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
    CPPUNIT_TEST(create1);
    CPPUNIT_TEST(remove);

    CPPUNIT_TEST_SUITE_END();

 public:

    TraceableTest();

    virtual ~TraceableTest();

    void setUp();

    void tearDown();

    void runTest ();

 private:

    void checksolo1 ();
    void checksolo2 ();
    void checkparent1 ();
    void checkparent2 ();
    void checkparent3 ();
    void create1 ();
    void remove ();

};

#endif  /* _TRACEABLE_TEST_H */
