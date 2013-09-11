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
#include <stdexcept>

#include <BIL/FontCache.hpp>

using namespace std;

namespace BIL {

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
		fontengine_ = new Freetype;
		fontengine_->open(font, dpi);
	}

	FontCache::~FontCache ()
	{
		if (fontengine_) {

			map<wchar_t, TextureFont*>::iterator it;
			for (it = texture_fonts_.begin(); it != texture_fonts_.end(); it++) {
				if (it->second) {
					delete it->second;
					it->second = 0;
				}
			}
			texture_fonts_.clear();

			initialized_ = false;

			delete fontengine_;
		}
	}

	bool FontCache::setup (void)
	{
		if (!fontengine_->valid()) {
			return false;
		}

		if (initialized_) {
			return false;
		}

		atlas_.generate(fontengine_, 32, 96);

		initialized_ = true;
		return true;
	}

	const Glyph& FontCache::queryGlyph (wchar_t charcode, bool create)
	{
		if (atlas_.contains(charcode)) {
			return atlas_.glyph(charcode);
		}

		map<wchar_t, TextureFont*>::iterator it;
		it = texture_fonts_.find(charcode);

		// if the glyph is not found and need to be created
		if (it == texture_fonts_.end()) {

			if (create) {

				TextureFont* new_font = new TextureFont;
				new_font->generate(fontengine_, charcode);
				texture_fonts_[charcode] = new_font;

			} else {
				// TODO: return an default font glyph to show unknown character
			}
		}

		return texture_fonts_[charcode]->glyph();
	}

	const GLuint FontCache::queryTexture (wchar_t charcode, bool create)
	{
		if (atlas_.contains(charcode)) {
			return atlas_.texture();
		}

		map<wchar_t, TextureFont*>::iterator it;
		it = texture_fonts_.find(charcode);

		// if the glyph is not found and need to be created
		if (it == texture_fonts_.end()) {

			if (create) {

				TextureFont* new_font = new TextureFont;
				new_font->generate(fontengine_, charcode);
				texture_fonts_[charcode] = new_font;

			} else {
				// TODO: return an default font glyph to show unknown character
			}
		}

		return texture_fonts_[charcode]->texture();
	}

#ifdef DEBUG
	void FontCache::printcount (void)
	{
		std::cout << "fonts in texture_fonts_: " << texture_fonts_.size() << std::endl;
	}
#endif

} /* namespace BIL */
