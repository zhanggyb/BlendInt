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

#include <cassert>

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Gui/FontCache.hpp>

namespace BlendInt {

	/*
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

        m_last.reset(new GlyphAtlas);
        m_last->Generate(default_texture_width, default_texture_height, cell_x, cell_y);
        m_last->bind();

        while (i < size) {

            glyph_index = m_ft_face.GetCharIndex(char_code + i);

            if(m_ft_face.LoadGlyph(glyph_index, FT_LOAD_NO_BITMAP | FT_LOAD_FORCE_AUTOHINT)) {

                int x = 0;
                int y = 0;

                if(font_data.flag & FontStyleOutline) {

                    ft_stroker.New(m_ft_lib);
                    ft_stroker.Set((int)(font_data.thickness * 64), FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);
                    glyph.GetGlyph(m_ft_face);
                    ft_stroker.GlyphStroke(glyph);
                    glyph.ToBitmap(FT_RENDER_MODE_NORMAL);
                    bitmap_glyph = glyph.GetBitmapGlyph();

                    if(m_last->Push(bitmap_glyph->bitmap.width,
                                    bitmap_glyph->bitmap.rows,
                                    bitmap_glyph->bitmap.buffer,
                                    &x, &y)) {
                        SetGlyphData(m_preset[i], x, y, slot, bitmap_glyph, m_last);
                    } else {
                        DBG_PRINT_MSG("%s",
                                      "Fail to push character into texture"
                                      " atlas, maybe the texture size is too"
                                      " small or the bitmap size is larger"
                                      " than cell size.");
                        exit(EXIT_FAILURE);
                    }

                    glyph.Done();
                    ft_stroker.Done();

                } else {

                    m_ft_face.LoadGlyph(glyph_index, FT_LOAD_RENDER);

                    if(m_last->Push(slot->bitmap.width, slot->bitmap.rows, slot->bitmap.buffer, &x, &y)) {
                        SetGlyphData(m_preset[i], x, y, slot, m_last);
                    } else {
                        DBG_PRINT_MSG("%s",
                                      "Fail to push character into texture"
                                      " atlas, maybe the texture size is too"
                                      " small or the bitmap size is larger"
                                      " than cell size.");
                        exit(EXIT_FAILURE);
                    }

                }

                if(!m_last->MoveNext()) {
                    DBG_PRINT_MSG("%s", "one texture is full, create a new one");
                    m_last->reset();

                    m_last.reset(new GlyphAtlas);
                    m_last->Generate(default_texture_width, default_texture_height, cell_x, cell_y);
                    m_last->bind();
                }

            } else {
                DBG_PRINT_MSG("%s", "Fail to initialize character into texture atlas");
                exit(EXIT_FAILURE);
            }

            i++;
        }

        m_last->reset();

    }
    */

    map<FcChar32, RefPtr<FontCache> > FontCache::kCacheDB;

    FcChar32 FontCache::kDefaultFontHash = 0;

    RefPtr<FontCache> FontCache::Create (const Fc::Pattern& pattern)
    {
    	RefPtr<FontCache> cache;

    	FcChar32 hash_id = pattern.hash();
    	if(kCacheDB.count(hash_id)) {
    		cache = kCacheDB[hash_id];
    	} else {
    		cache.reset(new FontCache(pattern));
    		kCacheDB[hash_id] = cache;
    	}

    	return cache;
    }

    bool FontCache::Release (const Fc::Pattern& pattern)
    {
    	FcChar32 hash_id = pattern.hash();

    	return kCacheDB.erase(hash_id);
    }

    void FontCache::ReleaseAll ()
    {
    	kCacheDB.clear();
    }

    size_t FontCache::GetCacheSize ()
    {
    	return kCacheDB.size();
    }

    FontCache::FontCache (const Fc::Pattern& pattern)
    : pattern_(pattern)
    {
    	FcValue file;
    	FcValue size;
    	FcValue dpi;

    	FcResult result = pattern_.get(FC_FILE, 0, &file);
    	if(result) {
			fprintf(stderr, "ERROR: Fail to get font file");
			exit(EXIT_FAILURE);
    	}
    	DBG_PRINT_MSG("file: %s", file.u.s);

    	result = pattern_.get(FC_SIZE, 0, &size);
    	if(result) {
			fprintf(stderr, "ERROR: Fail to get font size");
			exit(EXIT_FAILURE);
    	}

    	result = pattern_.get(FC_DPI, 0, &dpi);
    	if(result) {
			fprintf(stderr, "ERROR: Fail to get font dpi");
			exit(EXIT_FAILURE);
    	}

    	// DBG_PRINT_MSG("size: %d, dpi: %u", size.u.i, (unsigned int)dpi.u.d);

    	library_.Init();
    	face_.New(library_, (const char*)file.u.s);
    	face_.set_char_size(size.u.i << 6, 0, (unsigned int)dpi.u.d, 0);

    	texture_atlas_.reset(new TextureAtlas2DExt);
    	texture_atlas_->Generate(500, 32);
    }

    FontCache::~FontCache ()
    {
    	texture_atlas_.destroy();
    	glyph_data_.clear();
    	 pattern_.destroy();
    	face_.Done();
    	library_.Done();
    }

	const GlyphMetrics* FontCache::Query (uint32_t charcode, bool create)
	{
		typedef std::map<uint32_t, GlyphMetrics>::iterator iterator_type;

		iterator_type it = glyph_data_.find(charcode);

		if(it != glyph_data_.end()) {
			return &(it->second);
		}

		face_.load_char(charcode, FT_LOAD_RENDER);
		FT_GlyphSlot g = face_.face()->glyph;

		GlyphMetrics glyph;
		glyph.bitmap_left = g->bitmap_left;
		glyph.bitmap_top = g->bitmap_top;
		glyph.bitmap_width = g->bitmap.width;
		glyph.bitmap_height = g->bitmap.rows;
		glyph.advance_x = g->advance.x >> 6;
		glyph.advance_y = g->advance.y >> 6;

		texture_atlas_->bind();
		texture_atlas_->Upload(g->bitmap.width,
				g->bitmap.rows,
				g->bitmap.buffer,
				&(glyph.offset_u),
				&(glyph.offset_v));
		texture_atlas_->reset();


		std::pair<iterator_type, bool> result = glyph_data_.insert(std::pair<uint32_t, GlyphMetrics>(charcode, glyph));

		return &(result.first->second);
	}

} /* namespace BlendInt */
