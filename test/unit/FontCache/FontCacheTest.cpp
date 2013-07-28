// cpp

#include <BIL/FontCache.h>
#include <iostream>
#include <string>

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

}

void FontCacheTest::tearDown ()
{

}

void FontCacheTest::initialize1 ()
{
	bool result = FontCache::instance(FontCache::Default);

	FontCache * def = FontCache::getDefault();

	def->initialize();

	//delete def;

	CPPUNIT_ASSERT(result);
}

