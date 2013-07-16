/*
 * FontCache.h
 *
 *  Created on: 2013年7月16日
 *      Author: zhanggyb
 */

#ifndef _BIL_FONTCACHE_H_
#define _BIL_FONTCACHE_H_

#include <vector>
#include <BIL/TextureAtlas.h>
#include <wchar.h>

 using namespace std;

namespace BIL {

	/**
	 * @brief Class in charge of caching fonts
	 *
	 * A class to wrap font-manager in freetype-gl
	 */
	class FontCache
	{
	public:

		/**
		 * @brief Constructor
		 * @param w width of the underlying atlas
		 * @param h height of the underlying atlas
		 * @param d depth of the underlying atlas
		 */
		FontCache (size_t w, size_t h, size_t d);

		virtual ~FontCache ();

	private:

		wchar_t* wcsdup (const wchar_t *str);

		TextureAtlas _atlas;

		// vector<TextureFont> _fonts;

		wchar_t * _cache;

	};

} /* namespace BIL */
#endif /* FONTCACHE_H_ */
