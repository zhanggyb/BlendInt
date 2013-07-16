/*
 * FontCache.cpp
 *
 *  Created on: 2013年7月16日
 *      Author: zhanggyb
 */

#include <assert.h>

#include <BIL/FontCache.h>

namespace BIL {

	FontCache::FontCache (size_t w, size_t h, size_t d)
		: _atlas(w, h, d)
	{
		// TODO Auto-generated constructor stub

		// _fonts 

		_cache = wcsdup(L" ");
	}

	FontCache::~FontCache ()
	{
		// TODO Auto-generated destructor stub
		if(_cache) {
			free (_cache);
		}
	}

	wchar_t* FontCache::wcsdup (const wchar_t *str)
	{
		wchar_t * result;
		assert (str);
		result =(wchar_t *) malloc ((wcslen (str) + 1) * sizeof (wchar_t));
		wcscpy (result, str);
		return result;
	}

} /* namespace BIL */
