/*
 * FontCache.cpp
 *
 *  Created on: 2013年7月16日
 *      Author: zhanggyb
 */

#include <iostream>
#include <assert.h>

#include <deque>
#include <queue>
#include <utility>

#include <BIL/FontCache.h>

using namespace std;

namespace BIL {

	unsigned int FontCache::cacheSize = 128;

	unsigned int FontCache::maxCaches = 32;

	map<Font, FontCache*> FontCache::cacheDB;
	map<Font, unsigned long> FontCache::cacheCountDB;

	FontCache* FontCache::create (const Font& font, bool force)
	{
		// Don't repeatedly create, cause memory leak

		map<Font, FontCache*>::const_iterator it;
		it = cacheDB.find(font);

		if (it != cacheDB.end()) {
			unsigned long count = cacheCountDB[font];
			cacheCountDB[font] = count + 1;
			return cacheDB[font];
		}

		if (cacheDB.size() >= maxCaches) {

			if(!force) return NULL;

			// Remove mostly unused cache
			typedef std::pair<Font, unsigned long> data_t;
			typedef std::priority_queue<data_t, std::deque<data_t>,
			        greater_second<data_t> > queue_t;
			queue_t q(cacheCountDB.begin(), cacheCountDB.end());

			Font font_of_cache = q.top().first;
			cout << "Remove " << q.top().first.family << " from cache DB."
			        << std::endl;

			delete cacheDB[font_of_cache]; cacheDB.erase(font_of_cache);
			cacheCountDB.erase(font_of_cache);
		}

		FontCache * cache = new FontCache(font);

		cacheDB[font] = cache;

		return cache;
	}

	FontCache* FontCache::getCache (const Font& font, bool create)
	{
		map<Font, FontCache*>::const_iterator it;
		it = cacheDB.find(font);

		if ((it == cacheDB.end()) && create) {
			FontCache* cache = FontCache::create();
			return cache;
		} else {
			unsigned long count = cacheCountDB[font];
			cacheCountDB[font] = count + 1;
			return cacheDB[font];
		}
	}

	bool FontCache::release (const Font& font)
	{
		map<Font, FontCache*>::iterator it;
		it = cacheDB.find(font);

		if (it == cacheDB.end())
			return false;

		FontCache* cache = cacheDB[font];
		if (cache != NULL) {
			delete cache;
		}

		// now erase the key-value
		cacheDB.erase(it);
		cacheCountDB.erase(font);

		return true;
	}

	FontCache::FontCache (const Font& font, unsigned int dpi)
			: _font(font), _dpi(dpi), _fontengine(NULL)
	{

	}

	FontCache::~FontCache ()
	{
		if (_fontengine != NULL) {
			delete _fontengine;
			_fontengine = NULL;
		}
		map<wchar_t, Glyph*>::iterator it;
		for(it = _glyphDB.begin(); it != _glyphDB.end(); it++)
		{
			delete it->second;
			it->second = NULL;
		}
		_glyphDB.clear();
		_countDB.clear();
	}

	const Glyph* FontCache::query (wchar_t charcode, bool create)
	{
		map<wchar_t, Glyph*>::iterator it;
		it = _glyphDB.find(charcode);

		Glyph* glyph = NULL;

		// if the glyph is not found and need to be created
		if ((it == _glyphDB.end()) && create) {

			glyph = new Glyph(charcode, _font, _dpi, _fontengine);

			if (_glyphDB.size() >= cacheSize) {
				typedef std::pair<wchar_t, unsigned long> data_t;
				typedef std::priority_queue<data_t, std::deque<data_t>,
				        greater_second<data_t> > queue_t;
				queue_t q(_countDB.begin(), _countDB.end());

				wchar_t char_to_del = q.top().first;
				cout << "Remove " << q.top().first << " from cache."
				        << std::endl;

				delete _glyphDB[char_to_del]; _glyphDB.erase(char_to_del);
				_countDB.erase(char_to_del);
			}

			_glyphDB[charcode] = glyph;

		} else {
			glyph = _glyphDB[charcode];
		}

		unsigned long count = _countDB[charcode];
		_countDB[charcode] = count + 1;

		return glyph;
	}

} /* namespace BIL */
