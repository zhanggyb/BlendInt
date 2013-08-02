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

#include <boost/array.hpp>

#include <BIL/FontEngine.h>
#include <BIL/Font.h>
#include <BIL/Glyph.h>

using namespace std;
using namespace boost;

namespace BIL {

	template<class T>
	struct greater_second: std::binary_function<T, T, bool>
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
	public:
		// static member functions

		/**
		 * @brief create or get the FontCache object
		 * @param font Font, default is regular "Sans"
		 * @param force if true, remove unused cache and create one
		 * @return cache object created, NULL for failure
		 */
		static FontCache* create (const Font& font = Font("Sans"), bool force =
		        true);

		static FontCache* getCache (const Font& font = Font("Sans"));

		static bool release (const Font& font = Font("Sans"));

		static void releaseAll (void);

		static void setCacheSize (unsigned int size)
		{
			cacheSize = size;
		}

		static void setMaxCaches (unsigned int size)
		{
			maxCaches = size;
		}


#ifdef DEBUG
		static void list (void);
#endif

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
		 * @return pointer to a Glyph object
		 *
		 * @warning Do not delete the pointer get from this funciton
		 */
		Glyph* query (wchar_t charcode, bool create = true);

		int getHeight (void)
		{
			if(_fontengine == NULL) return 0;
			
			return _fontengine->getHeight();
		}

		int getAscender (void)
		{
			if(_fontengine == NULL) return 0;
			return _fontengine->getAscender();
		}

		int getDescender (void)
		{
			if(_fontengine == NULL) return 0;
			return _fontengine->getDescender();
		}

		int getMaxAdvance (void)
		{
			if(_fontengine == NULL) return 0;
			return _fontengine->getMaxAdvance();
		}

#ifdef DEBUG
		void printcount (void);
#endif

	private:

		/**
		 * @brief Default constructor
		 * @param font Font type
		 * @param dpi the DPI to be used
		 */
		FontCache (const Font& font, unsigned int dpi = 96);

		/**
		 * @brief private destructor
		 *
		 * Move destructor to private area to avoid this object be deleted
		 * in anywhere.
		 */
		virtual ~FontCache ();

	private:
		// member functions disabled

		FontCache ();

		FontCache (const FontCache& orig);

		FontCache& operator = (const FontCache& orig);

	private:
		// member variables

		Font _font;

		unsigned int _dpi;

		FontEngine* _fontengine;

		array<Glyph*, 128> _asciiDB;    // use arrary to store ascii for speed

		map<wchar_t, Glyph*> _glyphDB;

		map<wchar_t, unsigned long> _countDB;

	private:
		// static members

		static unsigned int cacheSize;

		static unsigned int maxCaches;

		static map<Font, FontCache*> cacheDB;
		static map<Font, unsigned long> cacheCountDB;

	};

} /* namespace BIL */
#endif /* FONTCACHE_H_ */
