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

#include <BIL/FontCache.hpp>

using namespace std;

namespace BIL {

	unsigned int FontCache::cacheSize = 1024;
	unsigned int FontCache::maxCaches = 32;

	map<Font, FontCache*> FontCache::cacheDB;
	map<Font, unsigned long> FontCache::cacheCountDB;

	FontCache* FontCache::create (const Font& font, unsigned int dpi,
	        bool force)
	{
		// Don't repeatedly create, cause memory leak
		map<Font, FontCache*>::const_iterator it;
		it = cacheDB.find(font);

		if (it != cacheDB.end()) {
			unsigned long count = cacheCountDB[font];
			cacheCountDB[font] = count + 1;

			it->second->set_dpi(dpi);
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
			wcout << "Remove " << q.top().first.family << " from cache DB."
			        << std::endl;

			delete cacheDB[font_of_cache];
			cacheDB.erase(font_of_cache);
			cacheCountDB.erase(font_of_cache);
		}

		FontCache * cache = new FontCache(font, dpi);

		cacheDB[font] = cache;
		unsigned long count = cacheCountDB[font];
		cacheCountDB[font] = count + 1;

		return cache;
	}

	FontCache* FontCache::getCache (const Font& font, unsigned int dpi)
	{
		map<Font, unsigned long>::const_iterator it;
		it = cacheCountDB.find(font);

		if (it == cacheCountDB.end()) {
			return NULL;
		} else {
			unsigned long count = cacheCountDB[font];
			cacheCountDB[font] = count + 1;
			cacheDB[font]->set_dpi(dpi);
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
			wcout << it->first.family << " of " << it->first.size
			        << " is used: " << it->second << endl;
		}
	}
#endif

	FontCache::FontCache (const Font& font, unsigned int dpi)
			: fontengine_(NULL), initialized_(false)
	{
		for (int i = 0; i < 128; i++) {
			ascii_db_[i] = NULL;
		}
		fontengine_ = new Freetype;
		fontengine_->open(font, dpi);
	}

	FontCache::~FontCache ()
	{
		if (fontengine_) {

			map<wchar_t, Glyph*>::iterator it;
			for (it = glyph_db_.begin(); it != glyph_db_.end(); it++) {
				if (it->second) {
					delete it->second;
					it->second = NULL;
				}
			}
			glyph_db_.clear();
			count_db_.clear();

			map<wchar_t, TextureFont*>::iterator tex_fonts_it;
			for (tex_fonts_it = texture_fonts_.begin(); tex_fonts_it != texture_fonts_.end(); tex_fonts_it++) {
				if (tex_fonts_it->second) {
					delete tex_fonts_it->second;
					tex_fonts_it->second = 0;
				}
			}
			texture_fonts_.clear();

			for (int i = 0; i < 128; i++) {
				if (ascii_db_[i]) {
					delete ascii_db_[i];
					ascii_db_[i] = NULL;
				}
			}
			initialized_ = false;

			delete fontengine_;
		}
	}

	bool FontCache::Initialize (void)
	{
		if (!fontengine_->valid()) {
			return false;
		}

		if (initialized_) {
			return false;
		}

		atlas_.generate(fontengine_, 32, 96);

		for (char i = 'A'; i < 'Z'; i++)
		{
			query (i, true);
			query (i+40, true);	// a-z
		}

		initialized_ = true;
		return true;
	}

	Glyph* FontCache::query (wchar_t charcode, bool create)
	{
		if (!fontengine_->valid()) {
			return NULL;
		}

		Glyph* glyph = NULL;

		if (charcode < 128) {
			glyph = ascii_db_[charcode];

			if (glyph) return glyph;

			if (create) {
				glyph = new Glyph(charcode, fontengine_);
				ascii_db_[charcode] = glyph;

				return glyph;
			} else {
				return NULL;
			}
		}

		map<wchar_t, Glyph*>::iterator it;
		it = glyph_db_.find(charcode);

		// if the glyph is not found and need to be created
		if (it == glyph_db_.end()) {

			if (create) {

				glyph = new Glyph(charcode, fontengine_);

				if (glyph_db_.size() >= cacheSize) {
					typedef std::pair<wchar_t, unsigned long> data_t;
					typedef std::priority_queue<data_t, std::deque<data_t>,
					        greater_second<data_t> > queue_t;
					queue_t q(count_db_.begin(), count_db_.end());

					wchar_t char_to_del = q.top().first;
					cout << "Remove " << q.top().first << " from cache."
					        << std::endl;

					delete glyph_db_[char_to_del];
					glyph_db_.erase(char_to_del);
					count_db_.erase(char_to_del);
				}

				glyph_db_[charcode] = glyph;

			}
		} else {
			glyph = glyph_db_[charcode];
		}

		if (glyph) {
			unsigned long count = count_db_[charcode];
			count_db_[charcode] = count + 1;
		}
		return glyph;
	}

#ifdef DEBUG
	void FontCache::printcount (void)
	{
		map<wchar_t, unsigned long>::iterator it;
		cout << endl;
		wchar_t ch;
		for (it = count_db_.begin(); it != count_db_.end(); it++) {
			ch = it->first;
			cout << "Character: ";
			cout << (unsigned long) ch << " ";
			cout << "Count: " << it->second << endl;
		}
	}
#endif

} /* namespace BIL */
