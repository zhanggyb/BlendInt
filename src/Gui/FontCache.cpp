/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <iostream>
#include <assert.h>

#include <deque>
#include <queue>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <iterator>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#ifdef USE_FONTCONFIG
#include <BlendInt/Core/FontConfig.hpp>
#endif

#include <BlendInt/Gui/FontCache.hpp>
#include <BlendInt/Service/ShaderManager.hpp>
#include <BlendInt/Types.hpp>

using namespace std;

namespace BlendInt {

	unsigned int FontCache::maxCaches = 32;

	map<FontFileInfo, RefPtr<FontCache> > FontCache::cacheDB;
	map<FontFileInfo, unsigned long> FontCache::cacheCountDB;

	bool operator < (const FontFileInfo& src, const FontFileInfo& dist)
	{
		if(src.file < dist.file) {
			return true;
		} else if(src.file > dist.file) {
			return false;
		}

		if(src.size < dist.size) {
			return true;
		} else if(src.size > dist.size) {
			return false;
		}

		if(src.dpi < dist.dpi) {
			return true;
		} else if(src.dpi > dist.dpi) {
			return false;
		}

		if(src.bold < dist.bold) {
			return true;
		} else if (src.bold > dist.bold) {
			return false;
		}

		if(src.italic < dist.italic) {
			return true;
		} else if (src.italic > dist.italic) {
			return false;
		}

		return false;
	}

	bool operator == (const FontFileInfo& src, const FontFileInfo& dist)
	{
		// use memcmp?
		return (src.file == dist.file &&
				src.size == dist.size &&
				src.dpi == dist.dpi &&
				src.bold == dist.bold &&
				src.italic == dist.italic);
	}

	/*
	FontCache* FontCache::create (const Font& font, unsigned int dpi,
	        bool force)
	{
		FontFileInfo key;

#ifdef USE_FONTCONFIG
		FontConfig* fontconfig = FontConfig::instance();
		key.file = fontconfig->getFontPath(font);
#else

#ifdef __APPLE__
		key.file = font.family;
#endif

#endif
		key.size = font.size;
		key.dpi = dpi;

		// Don't repeatedly create, cause memory leak
		map<FontFileInfo, FontCache*>::const_iterator it;
		it = cacheDB.find(key);

		if (it != cacheDB.end()) {
			unsigned long count = cacheCountDB[key];
			cacheCountDB[key] = count + 1;

			it->second->set_dpi(dpi);
			return it->second;
		}

		if (cacheDB.size() >= maxCaches) {

			if (!force)
				return NULL;

			// Remove mostly unused cache
			typedef std::pair<FontFileInfo, unsigned long> data_t;
			typedef std::priority_queue<data_t, std::deque<data_t>,
			        greater_second<data_t> > queue_t;
			queue_t q(cacheCountDB.begin(), cacheCountDB.end());

			FontFileInfo font_of_cache = q.top().first;
			//wcout << "Remove " << q.top().first.family << " from cache DB."
			//        << std::endl;

			delete cacheDB[font_of_cache];
			cacheDB.erase(font_of_cache);
			cacheCountDB.erase(font_of_cache);
		}

		FontCache * cache = new FontCache(font, dpi);

		cacheDB[key] = cache;
		unsigned long count = cacheCountDB[key];
		cacheCountDB[key] = count + 1;

		return cache;
	}
	*/

	RefPtr<FontCache> FontCache::Create (const std::string& file, unsigned int size, unsigned int dpi, bool bold, bool italic,
	        bool force)
	{
		FontFileInfo key;

#ifdef USE_FONTCONFIG
		FontConfig* fontconfig = FontConfig::instance();
		key.file = fontconfig->getFontPath(file, size, bold, italic);
#else

#ifdef __APPLE__
		key.file = font.family;
#endif

#endif
		key.size = size;
		key.dpi = dpi;
		key.bold = bold;
		key.italic = italic;

		// Don't repeatedly create, cause memory leak
		map<FontFileInfo, RefPtr<FontCache> >::const_iterator it;
		it = cacheDB.find(key);

		if (it != cacheDB.end()) {
			unsigned long count = cacheCountDB[key];
			cacheCountDB[key] = count + 1;

			it->second->set_dpi(dpi);
			return it->second;
		}

		if (cacheDB.size() >= maxCaches) {

			//if (!force)
				//return NULL;

			// Remove mostly unused cache
			typedef std::pair<FontFileInfo, unsigned long> data_t;
			typedef std::priority_queue<data_t, std::deque<data_t>,
			        greater_second<data_t> > queue_t;
			queue_t q(cacheCountDB.begin(), cacheCountDB.end());

			FontFileInfo font_of_cache = q.top().first;
			//wcout << "Remove " << q.top().first.family << " from cache DB."
			//        << std::endl;

			// delete cacheDB[font_of_cache];
			cacheDB.erase(font_of_cache);
			cacheCountDB.erase(font_of_cache);
		}

		RefPtr<FontCache> cache(new FontCache(key, dpi));

		cacheDB[key] = cache;
		unsigned long count = cacheCountDB[key];
		cacheCountDB[key] = count + 1;

		return cache;
	}

	/*
	FontCache* FontCache::getCache (const Font& font, unsigned int dpi)
	{
		FontFileInfo key;

#ifdef USE_FONTCONFIG
		FontConfig* fontconfig = FontConfig::instance();
		key.file = fontconfig->getFontPath(font);
#else

#ifdef __APPLE__
		key.file = font.family;
#endif

#endif
		key.size = font.size;
		key.dpi = dpi;


		map<FontFileInfo, unsigned long>::const_iterator it;
		it = cacheCountDB.find(key);

		if (it == cacheCountDB.end()) {
			return NULL;
		} else {
			unsigned long count = cacheCountDB[key];
			cacheCountDB[key] = count + 1;
			cacheDB[key]->set_dpi(dpi);
			return cacheDB[key];
		}
	}
	*/

	/*
	bool FontCache::release (const Font& font, unsigned int dpi)
	{
		FontFileInfo key;

#ifdef USE_FONTCONFIG
		FontConfig* fontconfig = FontConfig::instance();
		key.file = fontconfig->getFontPath(font);
#else

#ifdef __APPLE__
		key.file = font.family;
#endif

#endif
		key.size = font.size;
		key.dpi = dpi;

		map<FontFileInfo, FontCache*>::iterator it;
		it = cacheDB.find(key);

		if (it == cacheDB.end())
			return false;

		FontCache* cache = it->second;
		if (cache != NULL) {
			delete cache;
		}

		// now erase the key-value
		cacheDB.erase(it);
		cacheCountDB.erase(key);

		return true;
	}
	*/

	bool FontCache::Release (const FontFileInfo& key)
	{
		map<FontFileInfo, RefPtr<FontCache> >::iterator it;
		it = cacheDB.find(key);

		if (it == cacheDB.end())
			return false;

		/*
		FontCache* cache = it->second;
		if (cache != NULL) {
			delete cache;
		}
		*/

		// now erase the key-value
		cacheDB.erase(it);
		cacheCountDB.erase(key);

		return true;
	}

	void FontCache::releaseAll (void)
	{
		/*
		map<FontFileInfo, RefPtr<FontCache> >::iterator it;

		for (it = cacheDB.begin(); it != cacheDB.end(); it++) {
			delete it->second;
		}
		*/
		cacheDB.clear();
		cacheCountDB.clear();
	}

#ifdef DEBUG
	void FontCache::list (void)
	{
		map<FontFileInfo, unsigned long>::const_iterator it;
		cout << endl;
		for (it = cacheCountDB.begin(); it != cacheCountDB.end(); it++) {
			cout << it->first.file << " of " << it->first.size
			        << " is used: " << it->second << endl;
		}
	}
#endif

	/*
	FontCache::FontCache (const Font& font, unsigned int dpi)
			: Object(), m_vao(0), m_freetype(0)
	{
		std::string filepath;

#ifdef USE_FONTCONFIG
		FontConfig* fontconfig = FontConfig::instance();
		filepath = fontconfig->getFontPath(font);
#else

#ifdef __APPLE__
		filepath = font.family;
#endif

#endif

		m_freetype = new Freetype;
		m_freetype->Open(filepath, font.size, dpi);

		if(!setup()) {
			DBG_PRINT_MSG("%s", "Fail to setup FontCache");
		}
	}

	FontCache::FontCache (const FontFileInfo& key, unsigned int dpi)
			: Object(), m_vao(0), m_freetype(0)
	{
		std::string filepath;

#ifdef USE_FONTCONFIG
		FontConfig* fontconfig = FontConfig::instance();
		filepath = fontconfig->getFontPath(key.file, key.size, key.bold, key.italic);
#else

#ifdef __APPLE__
		filepath = font.family;
#endif

#endif

		m_freetype = new Freetype;
		m_freetype->Open(filepath, key.size, dpi);

		if(!setup()) {
			DBG_PRINT_MSG("%s", "Fail to setup FontCache");
		}
	}
	*/

	FontCache::FontCache (const FontFileInfo& key, unsigned int dpi)
			: Object(), m_vao(0)
	{
		std::string filepath;

#ifdef USE_FONTCONFIG
		FontConfig* fontconfig = FontConfig::instance();
		filepath = fontconfig->getFontPath(key.file, key.size, key.bold, key.italic);
#else

#ifdef __APPLE__
		filepath = font.family;
#endif

#endif

		m_freetype.Open(filepath, key.size, dpi);

		if(!setup()) {
			DBG_PRINT_MSG("%s", "Fail to setup FontCache");
		}
	}


	FontCache::~FontCache ()
	{
		map<wchar_t, TextureGlyph*>::iterator it;
		for (it = m_texture_fonts.begin(); it != m_texture_fonts.end(); it++) {
			if (it->second) {
				delete it->second;
				it->second = 0;
			}
		}
		m_texture_fonts.clear();

		m_freetype.Close();

		glDeleteVertexArrays(1, &m_vao);
	}

	bool FontCache::setup (void)
	{
		if (!m_freetype.valid()) {
			return false;
		}

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		m_atlas.Generate(&m_freetype, 32, 96);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		return true;
	}

	const Glyph& FontCache::query (wchar_t charcode, bool create)
	{
		if (m_atlas.Contain(charcode)) {
			return m_atlas.glyph(charcode);
		}

		map<wchar_t, TextureGlyph*>::iterator it;
		it = m_texture_fonts.find(charcode);

		// if the glyph is not found and need to be created
		if (it == m_texture_fonts.end()) {

			if (create) {

				TextureGlyph* new_font = new TextureGlyph;
				new_font->generate(&m_freetype, charcode);
				m_texture_fonts[charcode] = new_font;

			} else {
				// TODO: return an default font glyph to show unknown character
			}
		}

		return m_texture_fonts[charcode]->glyph();

	}

//	const GLuint FontCache::queryTexture (wchar_t charcode, bool create)
//	{
		//if (atlas_.contains(charcode)) {
//			return atlas_.texture();
		//}

		/*
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
		*/
//	}

//	unsigned int FontCache::queryWidth (wchar_t charcode, bool create)
//	{
		//if (atlas_.contains(charcode)) {
//			return atlas_.width();
		//}

		/*
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

		return texture_fonts_[charcode]->width();
		*/
//	}

//	unsigned int FontCache::queryHeight (wchar_t charcode, bool create)
//	{
//		if (atlas_.contains(charcode)) {
//			return atlas_.height();
//		}

		/*
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

		return texture_fonts_[charcode]->height();
		*/
//	}

	Rect FontCache::get_text_outline (const String& string)
	{
		if(!m_freetype.valid()) {
			return Rect();
		}
		String::const_iterator it;
		// String::const_iterator next;
		// Tuple2l kerning;
		int xmin = 0;
		int ymin = 0;
		int xmax = 0;
		int ymax = 0;

		for (it = string.begin(); it != string.end(); it++)
		{
			xmax = query(*it).advance_x + xmax;
			ymin = std::min(static_cast<int>(query(*it).bitmap_top - query(*it).bitmap_height), ymin);
			ymax = std::max(static_cast<int>(query(*it).bitmap_top), ymax);
		}

		return Rect(Point(xmin, ymin), Point(xmax, ymax));
	}

	unsigned int FontCache::GetTextWidth (const String& string, size_t length, size_t start)
	{
		unsigned int width = 0;

		if(!m_freetype.valid()) {
			return width;
		}

		String::const_iterator it = string.begin();
		std::advance(it, start);
		size_t i = 0;

		for (; (it != string.end()) && (i < length); it++, i++)
		{
			width = query(*it).advance_x + width;
		}

		return width;
	}


#ifdef DEBUG
	void FontCache::printcount (void)
	{
		//std::cout << "fonts in texture_fonts_: " << texture_fonts_.size() << std::endl;
	}
#endif

} /* namespace BlendInt */

