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

    FontCache* gFontCache = NULL;

    bool FontCache::initialized = false;
    unsigned int FontCache::cacheSize = 512;

    FontCache* FontCache::instance (void)
    {
        if (gFontCache != NULL) {
            cerr << "Error: FontCache should generate only on instance"
                 << endl;
            return NULL;
        }

        FontCache* fc = new FontCache;
        return fc;
    }

    bool FontCache::initialize (void)
    {
        if (initialized) return false;

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
