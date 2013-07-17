/* Test case for FontFace */

#ifndef _FONTFACE_TEST_H
#define _FONTFACE_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace std;

class FontFaceTest: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(FontFaceTest);

    CPPUNIT_TEST(create1);

    CPPUNIT_TEST_SUITE_END();

 public:

    FontFaceTest();

    virtual ~FontFaceTest();

    void setUp();

    void tearDown();

    void runTest ();

 private:

    void create1 ();

};

#endif  /* _FONTFACE_TEST_H */
