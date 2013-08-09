/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <iostream>
#include <assert.h>

#include <deque>
#include <queue>
#include <utility>

#include <BIL/FontCache.h>

using namespace std;

namespace BIL {

	unsigned int FontCache::cacheSize = 1024;
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
			return it->second;
		}

		if (cacheDB.size() >= maxCaches) {

			if (!force)
				return NULL;

			// Remove mostly unused cache
			typedef std::pair<Font, unsigned long> data_t;
			typedef std::priority_queue<data_t, std::deque<data_t>,
										greater_second<data_t> > queue_t;
			queue_t q(cacheCountDB.begin(), cacheCountDB.end());

			Font font_of_cache = q.top().first;
			cout << "Remove " << q.top().first.family << " from cache DB."
				 << std::endl;

			delete cacheDB[font_of_cache];
			cacheDB.erase(font_of_cache);
			cacheCountDB.erase(font_of_cache);
		}

		FontCache * cache = new FontCache(font);

		cacheDB[font] = cache;
		unsigned long count = cacheCountDB[font];
		cacheCountDB[font] = count + 1;

		return cache;
	}

	FontCache* FontCache::getCache (const Font& font)
	{
		map<Font, unsigned long>::const_iterator it;
		it = cacheCountDB.find(font);

		if (it == cacheCountDB.end()) {
			return NULL;
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

		FontCache* cache = it->second;
		if (cache != NULL) {
			delete cache;
		}

		// now erase the key-value
		cacheDB.erase(it);
		cacheCountDB.erase(font);

		return true;
	}

	void FontCache::releaseAll (void)
	{
		map<Font, FontCache*>::iterator it;

		for (it = cacheDB.begin(); it != cacheDB.end(); it++) {
			delete it->second;
		}
		cacheDB.clear();
		cacheCountDB.clear();
	}

#ifdef DEBUG
	void FontCache::list (void)
	{
		map<Font, unsigned long>::const_iterator it;
		cout << endl;
		for (it = cacheCountDB.begin(); it != cacheCountDB.end(); it++) {
			cout << it->first.family << " of " << it->first.size
				 << " is used: " << it->second << endl;
		}
	}
#endif

	FontCache::FontCache (const Font& font, unsigned int dpi)
		: _font(font), _dpi(dpi), _fontengine(NULL), _initialized(false)
	{
		for (unsigned char i = 0; i < 128; i++) {
			_asciiDB[i] = NULL;
		}
		_fontengine = new FontEngine(_font, _dpi);
	}

	FontCache::~FontCache ()
	{
		if (_fontengine->isValid()) {

			map<wchar_t, Glyph*>::iterator it;
			for (it = _glyphDB.begin(); it != _glyphDB.end(); it++) {
				delete it->second;
				it->second = NULL;
			}
			_glyphDB.clear();
			_countDB.clear();

			for (unsigned char i = 0; i < 128; i++) {
				if (_asciiDB[i] != NULL) {
					delete _asciiDB[i];
					_asciiDB[i] = NULL;
				}
			}
			_initialized = false;
		}
		if (_fontengine != NULL) {
			delete _fontengine;
			_fontengine = NULL;
		}
	}

	bool FontCache::initialize (void)
	{
		if (!_fontengine->isValid()) {
			return false;
		}

		if(_initialized) {
			return false;
		}

		for (unsigned char i = 0; i < 128; i++) {
			query(i, true);
		}

		_initialized = true;
		return true;
	}

	Glyph* FontCache::query (wchar_t charcode, bool create)
	{
		if (!_fontengine->isValid()) {
			return NULL;
		}

		Glyph* glyph = NULL;

		if (charcode < 128) {
			glyph = _asciiDB[charcode];
			if ((glyph == NULL) && create) {
				glyph = new Glyph(charcode, _font, _dpi, _fontengine);
				_asciiDB[charcode] = glyph;
			}

			return glyph;
		}

		map<wchar_t, Glyph*>::iterator it;
		it = _glyphDB.find(charcode);

		// if the glyph is not found and need to be created
		if (it == _glyphDB.end()) {

			if (create) {

				glyph = new Glyph(charcode, _font, _dpi, _fontengine);

				if (_glyphDB.size() >= cacheSize) {
					typedef std::pair<wchar_t, unsigned long> data_t;
					typedef std::priority_queue<data_t, std::deque<data_t>,
												greater_second<data_t> > queue_t;
					queue_t q(_countDB.begin(), _countDB.end());

					wchar_t char_to_del = q.top().first;
					cout << "Remove " << q.top().first << " from cache."
						 << std::endl;

					delete _glyphDB[char_to_del];
					_glyphDB.erase(char_to_del);
					_countDB.erase(char_to_del);
				}

				_glyphDB[charcode] = glyph;

			}
		} else {
			glyph = _glyphDB[charcode];
		}

		if(glyph != NULL) {
			unsigned long count = _countDB[charcode];
			_countDB[charcode] = count + 1;
		}
		return glyph;
	}

#ifdef DEBUG
	void FontCache::printcount (void)
	{
		map<wchar_t, unsigned long>::iterator it;
		cout << endl;
		for (it = _countDB.begin(); it != _countDB.end(); it++) {
			wchar_t ch = it->first;
			cout << "Character: ";
			cout << (unsigned long) ch << " ";
			cout << "Count: " << it->second << endl;
		}
	}
#endif

} /* namespace BIL */
