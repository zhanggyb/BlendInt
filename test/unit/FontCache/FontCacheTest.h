/* Test case for FontFace */

#ifndef _FONTCACHE_TEST_H
#define _FONTCACHE_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace std;

class FontCacheTest: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(FontCacheTest);

    CPPUNIT_TEST(create1);
    CPPUNIT_TEST(create2);
    CPPUNIT_TEST(create3);

    CPPUNIT_TEST_SUITE_END();

 public:

    FontCacheTest();

    virtual ~FontCacheTest();

    void setUp();

    void tearDown();

    void runTest ();

 private:

    void create1 ();
    void create2 ();
    void create3 ();

};

#endif  /* _FONTCACHE_TEST_H */
