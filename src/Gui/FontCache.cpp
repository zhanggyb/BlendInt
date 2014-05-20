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

	map<FontTypeBase, RefPtr<FontCache> > FontCache::cache_db;

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

	int FontCache::default_texture_width = 512;
	int FontCache::default_texture_height = 512;

	RefPtr<FontCache> FontCache::Create (const FontTypeBase& data)
	{
		// Don't repeatedly create
		map<FontTypeBase, RefPtr<FontCache> >::const_iterator it;
		it = cache_db.find(data);

		if (it != cache_db.end()) {
			return it->second;
		}

		RefPtr<FontCache> cache(new FontCache(data));
		cache->set_name(data.name);
		cache->Initialize(data, 32, 95);
		cache_db[data] = cache;

		return cache;
	}

	bool FontCache::Release (const FontTypeBase& data)
	{
		map<FontTypeBase, RefPtr<FontCache> >::iterator it;
		it = cache_db.find(data);

		if (it == cache_db.end())
			return false;

		cache_db.erase(it);
		return true;
	}

	void FontCache::ReleaseAll ()
	{
		cache_db.clear();
	}

	size_t FontCache::GetCacheSize ()
	{
		return cache_db.size();
	}

	void FontCache::SetDefaultTextureSize (int width, int height)
	{
		default_texture_width = width;
		default_texture_height = height;
	}

	int FontCache::GetDefaultTextureWidth ()
	{
		return default_texture_width;
	}

	int FontCache::GetDefaultTextureHeight ()
	{
		return default_texture_height;
	}

	FontCache::FontCache(const FontTypeBase& data)
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
	
	const GlyphExt* FontCache::Query (const FontTypeBase& font_data, uint32_t charcode, bool create)
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
		int cell_y = std::max (
						(m_ft_face.face()->size->metrics.ascender -
										m_ft_face.face()->size->metrics.descender) >> 6,
						m_ft_face.face()->size->metrics.height >> 6);

		m_last->Bind();
		if(m_last->IsFull()) {
			DBG_PRINT_MSG("%s", "Atlas is full, create a new one");
			m_last->Reset();
			m_last.reset(new TextureAtlas2D);
			m_last->Generate(default_texture_width, default_texture_height, cell_x, cell_y);
			m_last->Bind();
		}

		FT_GlyphSlot slot = m_ft_face.face()->glyph;
		FT_UInt glyph_index = m_ft_face.GetCharIndex(charcode);

		if(m_ft_face.LoadGlyph(glyph_index, FT_LOAD_NO_BITMAP | FT_LOAD_FORCE_AUTOHINT)) {

			if(font_data.flag & FontStyleOutline) {

				FTStroker ft_stroker;
				ft_stroker.New(m_ft_lib);
				ft_stroker.Set((int)(font_data.thickness * 64), FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
				FTGlyph glyph;
				glyph.GetGlyph(m_ft_face);
				ft_stroker.GlyphStroke(glyph);
				glyph.ToBitmap(FT_RENDER_MODE_NORMAL);
				FT_BitmapGlyph bitmap_glyph = glyph.GetBitmapGlyph();

				if(m_last->Push(bitmap_glyph->bitmap.width, bitmap_glyph->bitmap.rows, bitmap_glyph->bitmap.buffer)) {
					GlyphExt glyph;
					SetGlyphData(glyph, slot, bitmap_glyph, m_last);
					m_extension[charcode] = glyph;
					ret = &m_extension[charcode];
				} else {
					DBG_PRINT_MSG("%s", "Fail to push character into texture atlas, maybe the texture size is too small");
					exit(EXIT_FAILURE);
				}

				glyph.Done();
				ft_stroker.Done();

			} else {

				m_ft_face.LoadGlyph(glyph_index, FT_LOAD_RENDER | FT_LOAD_NO_HINTING);

				if(m_last->Push(slot->bitmap.width, slot->bitmap.rows, slot->bitmap.buffer)) {
					GlyphExt glyph;
					SetGlyphData(glyph, slot, m_last);
					m_extension[charcode] = glyph;
					ret = &m_extension[charcode];
				} else {
					DBG_PRINT_MSG("%s", "Fail to push character into texture atlas, maybe the texture size is too small");
					exit(EXIT_FAILURE);
				}

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

		return ret;
	}

	FontCache::~FontCache()
	{
		m_extension.clear();
		m_preset.clear();

		glDeleteBuffers(1, &m_vbo);
		glDeleteVertexArrays(1, &m_vao);

		m_ft_face.Done();
		m_ft_lib.Done();
	}

	void FontCache::Initialize (const FontTypeBase& font_data, uint32_t char_code, int size)
	{
		m_preset.clear();
		m_start = char_code;
		m_size = size;
		m_preset.resize(size);

		int cell_x = m_ft_face.face()->size->metrics.max_advance >> 6;
		int cell_y = std::max (
						(m_ft_face.face()->size->metrics.ascender -
										m_ft_face.face()->size->metrics.descender) >> 6,
						m_ft_face.face()->size->metrics.height >> 6);

		// if outline, add large the cell size;

		FT_GlyphSlot slot = m_ft_face.face()->glyph;
		FT_UInt glyph_index = 0;
		FTStroker ft_stroker;
		FTGlyph glyph;
		FT_BitmapGlyph bitmap_glyph = 0;

		int i = 0;

		m_last.reset(new TextureAtlas2D);
		m_last->Generate(default_texture_width, default_texture_height, cell_x, cell_y);
		m_last->Bind();

		while (i < size) {

			glyph_index = m_ft_face.GetCharIndex(char_code + i);

			if(m_ft_face.LoadGlyph(glyph_index, FT_LOAD_NO_BITMAP | FT_LOAD_FORCE_AUTOHINT)) {

				if(font_data.flag & FontStyleOutline) {

					ft_stroker.New(m_ft_lib);
					ft_stroker.Set((int)(font_data.thickness * 64), FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
					glyph.GetGlyph(m_ft_face);
					ft_stroker.GlyphStroke(glyph);
					glyph.ToBitmap(FT_RENDER_MODE_NORMAL);
					bitmap_glyph = glyph.GetBitmapGlyph();

					if(m_last->Push(bitmap_glyph->bitmap.width, bitmap_glyph->bitmap.rows, bitmap_glyph->bitmap.buffer)) {
						SetGlyphData(m_preset[i], slot, bitmap_glyph, m_last);
					} else {
						DBG_PRINT_MSG("%s", "Fail to push character into texture atlas, maybe the texture size is too small");
						exit(EXIT_FAILURE);
					}

					glyph.Done();
					ft_stroker.Done();

				} else {

					m_ft_face.LoadGlyph(glyph_index, FT_LOAD_RENDER);

					if(m_last->Push(slot->bitmap.width, slot->bitmap.rows, slot->bitmap.buffer)) {
						SetGlyphData(m_preset[i], slot, m_last);
					} else {
						DBG_PRINT_MSG("%s", "Fail to push character into texture atlas, maybe the texture size is too small");
						exit(EXIT_FAILURE);
					}

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

	void FontCache::SetGlyphData(GlyphExt& glyph, FT_GlyphSlot slot, const RefPtr<TextureAtlas2D>& atlas)
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

	void FontCache::SetGlyphData (GlyphExt& glyph, FT_GlyphSlot slot,
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
	void FontCache::list (void)
	{
		map<FontTypeBase, RefPtr<FontCache> >::const_iterator it;
		for (it = cache_db.begin(); it != cache_db.end(); it++) {
			cout << it->second->name() << endl;
		}
	}
#endif

} /* namespace BlendInt */
