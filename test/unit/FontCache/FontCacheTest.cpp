// cpp

#include <iostream>
#include <string>

#include <BIL/FontCache.h>
#include <BIL/FontConfig.h>

#include "FontCacheTest.h"

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(FontCacheTest);

FontCacheTest::FontCacheTest ()
{

}

FontCacheTest::~FontCacheTest ()
{

}

void FontCacheTest::setUp ()
{
    bool ret = false;

    FontConfig::instance();

    FontConfig::getService()->initialize();

    ret = FontConfig::getService()->loadDefaultFontToMem(); // load default font to memory
    if (!ret) {
        // TODO: stop and show failure of this TestFixture
        CPPUNIT_FAIL("Fail to load default font to memory");
    }
}

void FontCacheTest::tearDown ()
{
    FontConfig::release();
}

void FontCacheTest::create1 ()
{
    FontCache* cache = FontCache::create();

    FontCache::release();

    CPPUNIT_ASSERT(cache != NULL);
}

void FontCacheTest::create2 ()
{
    FontCache* cache = FontCache::create(Font("Droid Sans"));

    FontCache::release(Font("Droid Sans"));

    CPPUNIT_ASSERT(cache != NULL);
}

void FontCacheTest::create3 ()
{
    FontCache* cache = FontCache::create(Font("Droid Sans"));

    bool result = FontCache::release(Font("Sans"));

    FontCache::release(Font("Droid Sans"));

    CPPUNIT_ASSERT(!result);
}

void FontCacheTest::create4 ()
{
    FontCache* cache = FontCache::getCache(Font("Droid Sans Mono"), true);

    if (cache == NULL) {
        CPPUNIT_FAIL ("Cannot create cache for Droid Sans Mono\n");
    }

    bool result = cache->initialize();

    if (result) {
        const Glyph* glyph = cache->query('A');
        if(glyph == NULL) {
            result = false;
        } else {
            cout << "Glyph Index: " << glyph->getGlyphIndex() << endl;
        }
    }

    CPPUNIT_ASSERT(result);
}
