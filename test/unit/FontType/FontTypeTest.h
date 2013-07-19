/* Test case for FontFace */

#ifndef _FONTTYPE_TEST_H
#define _FONTTYPE_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace std;

class FontTypeTest: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(FontTypeTest);

    CPPUNIT_TEST(create1);
    CPPUNIT_TEST(create2);

    CPPUNIT_TEST_SUITE_END();

 public:

    FontTypeTest();

    virtual ~FontTypeTest();

    void setUp();

    void tearDown();

    void runTest ();

 private:

    void create1 ();
    void create2 ();

};

#endif  /* _FONTTYPE_TEST_H */
