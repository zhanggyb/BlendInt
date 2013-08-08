/* Test case for FontFace */

#ifndef _THEME_TEST_H
#define _THEME_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace std;

class ThemeTest: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ThemeTest);

    CPPUNIT_TEST(initialize1);
    CPPUNIT_TEST(initialize2);

    CPPUNIT_TEST_SUITE_END();

 public:

    ThemeTest();

    virtual ~ThemeTest();

    void setUp();

    void tearDown();

    void runTest ();

 private:

    void initialize1 ();

    void initialize2 ();

};

#endif  /* _THEME_TEST_H */
