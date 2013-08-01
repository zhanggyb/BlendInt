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
	FontCache* cache = FontCache::create(Font("Droid Sans Mono"));

	if (cache == NULL) {
		CPPUNIT_FAIL ("Cannot create cache for Droid Sans Mono\n");
	}

	bool result = cache->initialize();

	if (result) {
		Glyph* glyph = cache->query('A');
		if(glyph == NULL) {
			result = false;
		} else {
			cout << "Glyph Index: " << glyph->getGlyphIndex() << endl;
		}
	}

	FontCache::release (Font("Droid Sans Mono"));

	CPPUNIT_ASSERT(result);
}

void FontCacheTest::check1 ()
{
	FontCache* cache = FontCache::create(Font("Droid Sans Mono"));

	if(cache == NULL)
		CPPUNIT_FAIL ("Cannot create cache for Droid Sans Mono\n");

	bool result = cache->initialize();

	if(result) {
		cache->query(L'仁', true);
		cache->query(L'义', true);
		cache->query(L'礼', true);
		cache->query(L'智', true);
		cache->query(L'信', true);


		cache->query(L'仁');
		cache->query(L'仁');

#ifdef DEBUG
		cache->printcount();

		FontCache::list ();

#endif
	}

	FontCache::release (Font("Droid Sans Mono"));

	CPPUNIT_ASSERT (result);
}

void FontCacheTest::check2 ()
{
	FontCache* cache = FontCache::create(Font("Droid Sans Mono"), true);
	Glyph* glyph = NULL;

	if(cache == NULL)
		CPPUNIT_FAIL ("Cannot create cache for Droid Sans Mono\n");

	bool result = cache->initialize();

	if(result) {
#ifdef DEBUG
		cache->printcount();
		FontCache::list();
#endif
		glyph = cache->query('A');
	}

	FontCache::release (Font("Droid Sans Mono"));

	CPPUNIT_ASSERT (glyph != NULL && result);
}

void FontCacheTest::check3 ()
{
	FontCache* cache = FontCache::create();
	//Glyph* glyph = NULL;

	if(cache == NULL)
		CPPUNIT_FAIL ("Cannot create cache for Droid Sans Mono\n");

	bool result = cache->initialize();

	if(result) {
#ifdef DEBUG
		cache->printcount();
		FontCache::list();
#endif
		//glyph = cache->query('A');
	}

	FontCache::release ();

	CPPUNIT_ASSERT (result);
}

void FontCacheTest::check4 ()
{
	FontCache::create(Font("Sans"));
	FontCache::create(Font("Droid Sans"));
	FontCache::create(Font("Droid Serif"));
	FontCache::create(Font("Droid Sans Mono"));
	FontCache::create(Font("Droid Sans Thai"));

#ifdef DEBUG
		FontCache::list();
#endif

	FontCache::releaseAll ();
#ifdef DEBUG
		FontCache::list();
#endif

	CPPUNIT_ASSERT (true);
}

void FontCacheTest::check5 ()
{
	FontCache::setMaxCaches(6);
	FontCache::create(Font("Droid Sans"));
	FontCache::create(Font("Droid Serif"));
	FontCache::create(Font("Droid Sans Mono"));
	FontCache::create(Font("Droid Sans Thai"));
	FontCache::create(Font("Droid Sans Hebrew"));
	FontCache::create(Font("Droid Sans Fallback"));
	FontCache::create(Font("Droid Sans Japanese"));

#ifdef DEBUG
		FontCache::list();
#endif

	FontCache::releaseAll ();
#ifdef DEBUG
		FontCache::list();
#endif

		FontCache::setMaxCaches(32);

	CPPUNIT_ASSERT (true);
}

void FontCacheTest::check6 ()
{
	FontCache* cache = FontCache::create();
	Glyph* glyph = NULL;

	if(cache == NULL)
		CPPUNIT_FAIL ("Cannot create cache for default font\n");

	bool result = cache->initialize();

	if(result) {

		for(unsigned char i = 0; i < 128; i++)
		{
			glyph = cache->query(i);
			cout << (int)glyph->getGlyphIndex() << endl;
		}

		for(int i = 0; i < 10; i++)
		{
			glyph = cache->query(L'仁');
		}
		for(int i = 0; i < 5; i++)
		{
			glyph = cache->query(L'义');
		}
		for(int i = 0; i < 20; i++)
		{
			glyph = cache->query(L'智');
		}

#ifdef DEBUG
		cache->printcount();
#endif

	}

	FontCache::releaseAll ();

	CPPUNIT_ASSERT (true);
}
