/*
 * FontCache.cpp
 *
 *  Created on: 2013年7月16日
 *      Author: zhanggyb
 */

#include <iostream>
#include <assert.h>

#include <BIL/FontCache.h>

using namespace std;

namespace BIL {

	unsigned int FontCache::cacheSize = 128;

	unsigned int FontCache::maxCaches = 32;

	map<Font, FontCache*> FontCache::cacheDB;

	FontCache* FontCache::create (const Font& font)
	{
		if(cacheDB.size() >= maxCaches) {
			// TODO: remove mostly unused cache
		}

		FontCache * cache = new FontCache(font);

		cacheDB[font] = cache;

		return cache;
	}

	FontCache* FontCache::getCache (const Font& font)
	{
		map<Font, FontCache*>::const_iterator it;
		it = cacheDB.find(font);

		if(it == cacheDB.end()) return NULL;

		return cacheDB[font];
	}

	bool FontCache::release (const Font& font)
	{
		map<Font, FontCache*>::iterator it;
		it = cacheDB.find(font);

		if(it == cacheDB.end()) return false;

		FontCache* cache = cacheDB[font];
		if(cache != NULL) {
			delete cache;
		}

		// now erase the key-value
		cacheDB.erase(it);

		return true;
	}

	FontCache::FontCache (const Font& font)
		: _fonttype(NULL)
	{

	}

	FontCache::~FontCache ()
	{
		if(_fonttype != NULL) {
			delete _fonttype;
			_fonttype = NULL;
		}
	}

} /* namespace BIL */
