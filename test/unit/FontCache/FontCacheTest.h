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

    CPPUNIT_TEST(initialize1);

    CPPUNIT_TEST_SUITE_END();

 public:

    FontCacheTest();

    virtual ~FontCacheTest();

    void setUp();

    void tearDown();

    void runTest ();

 private:

    void initialize1 ();

};

#endif  /* _FONTCACHE_TEST_H */
