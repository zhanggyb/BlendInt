/*
 * FontCache.h
 *
 *  Created on: 2013年7月16日
 *      Author: zhanggyb
 */

#ifndef _BIL_FONTCACHE_H_
#define _BIL_FONTCACHE_H_

#include <GL/glew.h>
#include <GL/gl.h>

#include <map>
#include <vector>

#include <BIL/Glyph.h>

using namespace std;

namespace BIL {

	/**
	 * @brief Class in charge of caching fonts
	 */
	class FontCache
	{
	public:

		enum CacheType
		{
			Default, Menu
		};

		static bool instance (CacheType cache = Default);

		static FontCache* getDefault (void)
		{
			return fontDefault;
		}

		bool initialize (void);

		bool isInitialized (void) const
		{
			return initialized;
		}

		void setCacheSize (unsigned int size)
		{
			cacheSize = size;
		}

	private:

		/**
		 * @brief private destructor
		 *
		 * Move destructor to private area to avoid this object be deleted
		 * in anywhere.
		 */
		virtual ~FontCache ();

		FontCache ();

		FontCache (const FontCache& orige);

		FontCache& operator = (const FontCache& orig);

	private:

		struct FontData
		{
			GLuint texture;
			GLuint displist;
		};

		static FontCache* fontDefault;

		static FontCache* fontMenu;	// TODO: more font configuration

		map<wchar_t, FontData> _fontdb;

		static bool initialized;

		static unsigned int cacheSize;

	};

} /* namespace BIL */
#endif /* FONTCACHE_H_ */
