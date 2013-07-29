/*
 * FontCache.h
 *
 *  Created on: 2013年7月16日
 *      Author: zhanggyb
 */

#ifndef _BIL_FONTCACHE_H_
#define _BIL_FONTCACHE_H_

#include <map>
#include <wchar.h>

#include <BIL/FontType.h>
#include <BIL/Font.h>
#include <BIL/Glyph.h>

using namespace std;

namespace BIL {

	/**
	 * @brief Class in charge of caching fonts
	 */
	class FontCache
	{
	public:	// static member functions

		/**
		 * @brief create and get the FontCache object
		 * @param font Font, default is regular "Sans"
		 * @return cache object created, NULL for failure
		 */
		static FontCache* create (const Font& font = Font("Sans"));

		static FontCache* getCache (const Font& font = Font("Sans"));

		static bool release (const Font& font = Font("Sans"));

		static void setCacheSize (unsigned int size)
		{
			cacheSize = size;
		}

	public:

		/**
		 * @brief Initialize glyph database
		 * @return true for success, false for failure
		 */
		bool initialize (void);

		Glyph* query (wchar_t charcode);

		bool addCharacter (wchar_t charcode);

	private:

		FontCache (const Font& font);

		/**
		 * @brief private destructor
		 *
		 * Move destructor to private area to avoid this object be deleted
		 * in anywhere.
		 */
		virtual ~FontCache ();

	private:	// member functions disabled

		FontCache ();

		FontCache (const FontCache& orig);

		FontCache& operator = (const FontCache& orig);

	private:	// member variables

		FontType* _fonttype;

		map<wchar_t, Glyph*> _glyphdb;

	private:	// static members

		static unsigned int cacheSize;

		static unsigned int maxCaches;

		static map<Font, FontCache*> cacheDB;

	};

} /* namespace BIL */
#endif /* FONTCACHE_H_ */
