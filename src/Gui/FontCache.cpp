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
#else
#include <boost/filesystem.hpp>
#endif

#include <BlendInt/Gui/FontCache.hpp>
#include <BlendInt/Service/ShaderManager.hpp>
#include <BlendInt/Types.hpp>

using namespace std;

namespace BlendInt {

	unsigned int FontCache::maxCaches = 32;

	map<FontFileInfo, RefPtr<FontCache> > FontCache::cacheDB;
	map<FontFileInfo, unsigned long> FontCache::cacheCountDB;

	map<FontTypeBase, RefPtr<FontCacheExt> > FontCacheExt::cache_db;

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

	bool operator < (const FontTypeBase& src, const FontTypeBase& dist)
	{
		if(src.name < dist.name) {
			return true;
		} else if(src.name > dist.name) {
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

		if(src.flag < dist.flag) {
			return true;
		} else if (src.flag > dist.flag) {
			return false;
		}

		if(src.thickness < dist.thickness) {
			return true;
		} else if (src.thickness > dist.thickness) {
			return false;
		}

		return false;
	}

	bool operator == (const FontTypeBase& src, const FontTypeBase& dist)
	{
		// use memcmp?
		return (src.name == dist.name &&
				src.size == dist.size &&
				src.dpi == dist.dpi &&
				src.flag == dist.flag &&
				src.thickness == dist.thickness);
	}

	int FontCacheExt::default_texture_width = 512;
	int FontCacheExt::default_texture_height = 512;

	RefPtr<FontCacheExt> FontCacheExt::Create (const FontTypeBase& data)
	{
		// Don't repeatedly create
		map<FontTypeBase, RefPtr<FontCacheExt> >::const_iterator it;
		it = cache_db.find(data);

		if (it != cache_db.end()) {
			return it->second;
		}

		RefPtr<FontCacheExt> cache(new FontCacheExt(data));
		cache->set_name(data.name);
		cache->Initialize(data, 32, 95);
		cache_db[data] = cache;

		return cache;
	}

	bool FontCacheExt::Release (const FontTypeBase& data)
	{
		map<FontTypeBase, RefPtr<FontCacheExt> >::iterator it;
		it = cache_db.find(data);

		if (it == cache_db.end())
			return false;

		cache_db.erase(it);
		return true;
	}

	void FontCacheExt::ReleaseAll ()
	{
		cache_db.clear();
	}

	size_t FontCacheExt::GetCacheSize ()
	{
		return cache_db.size();
	}

	void FontCacheExt::SetDefaultTextureSize (int width, int height)
	{
		default_texture_width = width;
		default_texture_height = height;
	}

	int FontCacheExt::GetDefaultTextureWidth ()
	{
		return default_texture_width;
	}

	int FontCacheExt::GetDefaultTextureHeight ()
	{
		return default_texture_height;
	}

	FontCacheExt::FontCacheExt(const FontTypeBase& data)
	: m_vao(0), m_vbo(0), m_start(32), m_size(95)
	{
#ifdef USE_FONTCONFIG
		FontConfig* fontconfig = FontConfig::instance();
		bool bold = data.flag & FontStyleBold;
		bool italic = data.flag & FontStyleItalic;

		std::string filepath = fontconfig->getFontPath(data.name, data.size, bold, italic);
		if(filepath.empty()) {

			DBG_PRINT_MSG("Warning: the font family: %s does not exist", data.name.c_str());

#ifdef __LINUX__
			filepath = fontconfig->getFontPath("Sans", data.size, bold, italic);
#else
#ifdef __APPLE__
			filepath = fontconfig->getFontPath("Sans-Serif", data.size, bold, italic);
#endif
#endif
			assert(!filepath.empty());
		} else {
			DBG_PRINT_MSG("filepath: %s", filepath.c_str());
		}
#else

#endif

		if(m_ft_lib.Initialize()) {
			m_ft_lib.SetLcdFilter(FT_LCD_FILTER_DEFAULT);
			if(m_ft_face.New(m_ft_lib, filepath.data())) {
				m_ft_face.SetCharSize(data.size << 6, 0, data.dpi, 0);
			}
		}

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	
	const GlyphExt* FontCacheExt::Query (const FontTypeBase& font_data, wchar_t charcode, bool create)
	{
		int index = charcode - m_start;

		if(index >= 0 && index < m_size) {
			return &m_preset[index];
		}

		if(m_extension.count(charcode)) {
			return &m_extension[charcode];
		}

		const GlyphExt* ret = 0;
		int cell_x = m_ft_face.face()->size->metrics.max_advance >> 6;
		int cell_y = (m_ft_face.face()->size->metrics.ascender - m_ft_face.face()->size->metrics.descender) >> 6;

		m_last->Bind();
		if(m_last->IsFull()) {
			DBG_PRINT_MSG("%s", "Atlas is full, create a new one");
			m_last->Reset();
			m_last.reset(new TextureAtlas2D);
			m_last->Generate(default_texture_width, default_texture_height, cell_x, cell_y);
			m_last->Bind();
		}

		m_ft_face.LoadChar(charcode, FT_LOAD_RENDER);

		FT_GlyphSlot slot = m_ft_face.face()->glyph;

		if (m_last->Push(slot->bitmap.width, slot->bitmap.rows,
						slot->bitmap.buffer)) {

			GlyphExt glyph;
			SetGlyphData(glyph, slot, m_last);

			m_extension[charcode] = glyph;

			ret = &m_extension[charcode];
		} else {
			DBG_PRINT_MSG("%s", "Fail to push character into texture atlas, maybe the texture is too small");
			exit(EXIT_FAILURE);
		}

		if(!m_last->MoveNext()) {
			DBG_PRINT_MSG("%s", "one texture is full, create a new one");
			m_last->Reset();

			m_last.reset(new TextureAtlas2D);
			m_last->Generate(default_texture_width, default_texture_height, cell_x, cell_y);
		} else {
			m_last->Reset();
		}

		return ret;
	}

	FontCacheExt::~FontCacheExt()
	{
		m_extension.clear();
		m_preset.clear();

		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);

		m_ft_face.Done();
		m_ft_lib.Done();
	}

	void FontCacheExt::Initialize (const FontTypeBase& font_data, wchar_t char_code, int size)
	{
		m_preset.clear();
		m_start = char_code;
		m_size = size;
		m_preset.resize(size);

		int cell_x = m_ft_face.face()->size->metrics.max_advance >> 6;
		int cell_y = (m_ft_face.face()->size->metrics.ascender - m_ft_face.face()->size->metrics.descender) >> 6;

		// if outline, add large the cell size;

		FT_GlyphSlot slot = m_ft_face.face()->glyph;
		FT_UInt glyph_index = 0;

		int i = 0;

		m_last.reset(new TextureAtlas2D);
		m_last->Generate(default_texture_width, default_texture_height, cell_x, cell_y);
		m_last->Bind();

		while (i < size) {

			glyph_index = m_ft_face.GetCharIndex(char_code + i);

			if(m_ft_face.LoadGlyph(glyph_index, FT_LOAD_NO_BITMAP | FT_LOAD_FORCE_AUTOHINT)) {

				/*
					FTStroker ft_stroker;
					ft_stroker.New(m_ft_lib);
					ft_stroker.Set((int)(0.5 * 64), FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
					FTGlyph glyph;
					glyph.GetGlyph(m_ft_face);
					ft_stroker.GlyphStroke(glyph);
					glyph.ToBitmap(FT_RENDER_MODE_NORMAL);
					FT_BitmapGlyph bitmap_glyph = glyph.GetBitmapGlyph();

					if(atlas->Push(bitmap_glyph->bitmap.width, bitmap_glyph->bitmap.rows, bitmap_glyph->bitmap.buffer)) {
						SetGlyphData(m_preset[i], slot, bitmap_glyph, ox, oy, atlas);
						i++;
						glyph.Done();
						ft_stroker.Done();
					} else {
						DBG_PRINT_MSG("%s", "one texture is full, create a new one");
						glyph.Done();
						ft_stroker.Done();
						break;
					}
				 */
				m_ft_face.LoadGlyph(glyph_index, FT_LOAD_RENDER | FT_LOAD_NO_HINTING);

				if(m_last->Push(slot->bitmap.width, slot->bitmap.rows, slot->bitmap.buffer)) {
					SetGlyphData(m_preset[i], slot, m_last);
				} else {
					DBG_PRINT_MSG("%s", "Fail to push character into texture atlas, maybe the texture size is too small");
					exit(EXIT_FAILURE);
				}

				if(!m_last->MoveNext()) {
					DBG_PRINT_MSG("%s", "one texture is full, create a new one");
					m_last->Reset();

					m_last.reset(new TextureAtlas2D);
					m_last->Generate(default_texture_width, default_texture_height, cell_x, cell_y);
					m_last->Bind();
				}

			} else {
				DBG_PRINT_MSG("%s", "Fail to initialize character into texture atlas");
				exit(EXIT_FAILURE);
			}

			i++;
		}

		m_last->Reset();

	}

	void FontCacheExt::SetGlyphData(GlyphExt& glyph, FT_GlyphSlot slot, const RefPtr<TextureAtlas2D>& atlas)
	{
		float ox = atlas->xoffset() / (float)default_texture_width;
		float oy = atlas->yoffset() / (float)default_texture_height;

		glyph.bitmap_width = slot->bitmap.width;
		glyph.bitmap_height = slot->bitmap.rows;
		glyph.bitmap_left = slot->bitmap_left;
		glyph.bitmap_top = slot->bitmap_top;
		glyph.advance_x = slot->advance.x >> 6;
		glyph.advance_y = slot->advance.y >> 6;

		glyph.vertices[0].x = slot->bitmap_left;
		glyph.vertices[0].y = slot->bitmap_top	- slot->bitmap.rows;
		glyph.vertices[0].s = ox;
		glyph.vertices[0].t = oy + slot->bitmap.rows / (float)default_texture_height;

		glyph.vertices[1].x = slot->bitmap_left + slot->bitmap.width;
		glyph.vertices[1].y = slot->bitmap_top	- slot->bitmap.rows;
		glyph.vertices[1].s = ox + slot->bitmap.width / (float)default_texture_width;
		glyph.vertices[1].t = oy + slot->bitmap.rows / (float)default_texture_height;

		glyph.vertices[2].x = slot->bitmap_left;
		glyph.vertices[2].y = slot->bitmap_top;
		glyph.vertices[2].s = ox;
		glyph.vertices[2].t = oy;

		glyph.vertices[3].x = slot->bitmap_left + slot->bitmap.width;
		glyph.vertices[3].y = slot->bitmap_top;
		glyph.vertices[3].s = ox + slot->bitmap.width / (float)default_texture_width;
		glyph.vertices[3].t = oy;

		glyph.texture_atlas = atlas;
	}

	void FontCacheExt::SetGlyphData (GlyphExt& glyph, FT_GlyphSlot slot,
					FT_BitmapGlyph bitmap_glyph, const RefPtr<TextureAtlas2D>& atlas)
	{
		float ox = atlas->xoffset() / (float)default_texture_width;
		float oy = atlas->yoffset() / (float)default_texture_height;

		glyph.bitmap_width = bitmap_glyph->bitmap.width;
		glyph.bitmap_height = bitmap_glyph->bitmap.rows;
		glyph.bitmap_left = bitmap_glyph->left;
		glyph.bitmap_top = bitmap_glyph->top;
		glyph.advance_x = slot->advance.x >> 6;
		glyph.advance_y = slot->advance.y >> 6;

		glyph.vertices[0].x = bitmap_glyph->left;
		glyph.vertices[0].y = bitmap_glyph->top	- bitmap_glyph->bitmap.rows;
		glyph.vertices[0].s = ox;
		glyph.vertices[0].t = oy + bitmap_glyph->bitmap.rows / (float)default_texture_height;

		glyph.vertices[1].x = bitmap_glyph->left + bitmap_glyph->bitmap.width;
		glyph.vertices[1].y = bitmap_glyph->top	- bitmap_glyph->bitmap.rows;
		glyph.vertices[1].s = ox + bitmap_glyph->bitmap.width / (float)default_texture_width;
		glyph.vertices[1].t = oy + bitmap_glyph->bitmap.rows / (float)default_texture_height;

		glyph.vertices[2].x = bitmap_glyph->left;
		glyph.vertices[2].y = bitmap_glyph->top;
		glyph.vertices[2].s = ox;
		glyph.vertices[2].t = oy;

		glyph.vertices[3].x = bitmap_glyph->left + bitmap_glyph->bitmap.width;
		glyph.vertices[3].y = bitmap_glyph->top;
		glyph.vertices[3].s = ox + bitmap_glyph->bitmap.width / (float)default_texture_width;
		glyph.vertices[3].t = oy;

		glyph.texture_atlas = atlas;

	}

#ifdef DEBUG
	void FontCacheExt::list (void)
	{
		map<FontTypeBase, RefPtr<FontCacheExt> >::const_iterator it;
		for (it = cache_db.begin(); it != cache_db.end(); it++) {
			cout << it->second->name() << endl;
		}
	}
#endif

	// ---------------------------------------------------

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

		if(!Setup()) {
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

		glDeleteBuffers(1, &m_vbo);

		glDeleteVertexArrays(1, &m_vao);
	}

	bool FontCache::Setup (void)
	{
		if (!m_freetype.valid()) {
			return false;
		}

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		m_atlas.Generate(m_freetype, 32, 96);

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
				new_font->Load(m_freetype, charcode);
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

	Rect FontCache::GetTextOutline (const String& string)
	{
		if(!m_freetype.valid()) {
			return Rect();
		}

		String::const_iterator it;
		// String::const_iterator next;
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

	size_t FontCache::GetTextWidth (const String& string, size_t length, size_t start)
	{
		size_t width = 0;

		if(!m_freetype.valid()) {
			return width;
		}

		assert(start <= string.length() && length <= string.length());

		String::const_iterator it = string.begin();
		std::advance(it, start);
		size_t i = 0;

		while(it != string.end() && (i < length)) {
			width += query(*it).advance_x;
			it++;
			i++;
		}

		return width;
	}

	size_t FontCache::GetReverseTextWidth (const String& string, size_t length, size_t start)
	{
		size_t width = 0;

		if(!m_freetype.valid()) {
			return width;
		}

		assert(start <= string.length() && length <= string.length());

		String::const_reverse_iterator it = string.rbegin();
		std::advance(it, start);
		size_t i = 0;

		while (it != string.rend() && (i < length)) {
			width += query(*it).advance_x;
			it++;
			i++;
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
