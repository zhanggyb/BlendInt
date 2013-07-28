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

	FontCache* FontCache::fontDefault = NULL;

	bool FontCache::initialized = false;

	unsigned int FontCache::cacheSize = 128;

	bool FontCache::instance (CacheType cache)
	{
		switch(cache)
		{
			case Default:
				if(fontDefault != NULL) {
					cerr << "Error: Cache for default font should be generated only once" << endl;
					return false;
				} else {
					fontDefault = new FontCache;
				}
				break;
			default:
				break;
		}

		return true;
	}

	bool FontCache::initialize (void)
	{
		if (initialized)
			return false;

		initialized = true;
		return true;
	}

	FontCache::FontCache ()
	{

	}

	FontCache::~FontCache ()
	{
		// TODO Auto-generated destructor stub
		/*
		 if(_cache) {
		 free (_cache);
		 }
		 */
		initialized = false;
	}

} /* namespace BIL */
