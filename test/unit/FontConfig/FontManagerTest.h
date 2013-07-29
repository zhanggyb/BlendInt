/* Test case for FontFace */

#ifndef _FONTMANAGER_TEST_H
#define _FONTMANAGER_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace std;

class FontManagerTest: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(FontManagerTest);

    CPPUNIT_TEST(initialize1);
    CPPUNIT_TEST(loadfont1);
    CPPUNIT_TEST(loadfont2);

    CPPUNIT_TEST_SUITE_END();

 public:

    FontManagerTest();

    virtual ~FontManagerTest();

    void setUp();

    void tearDown();

    void runTest ();

 private:

    void initialize1 ();
    void loadfont1 ();
    void loadfont2 ();

};

#endif  /* _FONTMANAGER_TEST_H */
