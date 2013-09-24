/* Test case for FontFace */

#ifndef _FONTCONFIG_TEST_H
#define _FONTCONFIG_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace std;

class FontConfigTest: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(FontConfigTest);

#ifdef DEBUG
    CPPUNIT_TEST(initialize1);
    CPPUNIT_TEST(loadfont1);
    CPPUNIT_TEST(loadfont2);
#endif

    CPPUNIT_TEST_SUITE_END();

 public:

    FontConfigTest();

    virtual ~FontConfigTest();

    void setUp();

    void tearDown();

    void runTest ();

 private:

#ifdef DEBUG
    void initialize1 ();
    void loadfont1 ();
    void loadfont2 ();
#endif

};

#endif  /* _FONTCONFIG_TEST_H */
