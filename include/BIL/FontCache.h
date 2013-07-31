/*
 * FontCache.h
 *
 *  Created on: 2013年7月16日
 *      Author: zhanggyb
 */

#ifndef _BIL_FONTCACHE_H_
#define _BIL_FONTCACHE_H_

#include <map>
#include <functional>
#include <wchar.h>

#include <BIL/FontEngine.h>
#include <BIL/Font.h>
#include <BIL/Glyph.h>

using namespace std;

namespace BIL {

	template<class T>
	struct greater_second : std::binary_function<T, T, bool>
	{
		inline bool operator() (const T& lhs, const T& rhs)
		{
			return lhs.second > rhs.second;
		}
	};

	/**
	 * @brief Class in charge of caching fonts
	 */
	class FontCache
	{
	public:	// static member functions

		/**
		 * @brief create and get the FontCache object
		 * @param font Font, default is regular "Sans"
		 * @param force if true, remove unused cache and create one
		 * @return cache object created, NULL for failure
		 */
		static FontCache* create (const Font& font = Font("Sans"), bool force = true);

		static FontCache* getCache (const Font& font = Font("Sans"), bool create = true);

		static bool release (const Font& font = Font("Sans"));

		static void releaseAll (void);

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

		/**
		 * @brief query the Glyph of a wchar
		 * @param charcode wchar character
		 * @param create create a glyph if not found
		 * @return
		 */
		const Glyph* query (wchar_t charcode, bool create = true);

		bool addCharacter (wchar_t charcode);

	private:

		FontCache (const Font& font, unsigned int dpi = 96);

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

		Font _font;

		unsigned int _dpi;

		FontEngine* _fontengine;

		map<wchar_t, Glyph*> _glyphDB;
		map<wchar_t, unsigned long> _countDB;

	private:	// static members

		static unsigned int cacheSize;

		static unsigned int maxCaches;

		static map<Font, FontCache*> cacheDB;
		static map<Font, unsigned long> cacheCountDB;

	};

} /* namespace BIL */
#endif /* FONTCACHE_H_ */
