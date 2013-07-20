/*
 * TextureFont.cpp
 *
 *  Created on: 2013年7月16日
 *      Author: zhanggyb
 */

#include <math.h>
#include <assert.h>

#include <BIL/FontType.h>
#include <BIL/FontManager.h>
#include <BIL/TextureFont.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H


namespace BIL {

    TextureFont::Glyph::Glyph ()
        : charcode(0),
          id (0),
          width(0),
          height(0),
          offset_x(0),
          offset_y(0),
          advance_x(0.0),
          advance_y(0.0),
          s0(0.0),
          t0(0.0),
          s1(0.0),
          t1(0.0),
          outline_type(0),
          outline_thickness(0.0)
    {
    }

    TextureFont::Glyph::~Glyph()
    {
        kerning.clear();
    }

    TextureFont::Glyph::Glyph(const Glyph& orig)
    {
        charcode = orig.charcode;
        id = orig.id;
        width = orig.width;
        height = orig.height;
        offset_x = orig.offset_x;
        offset_y = orig.offset_y;
        advance_x = orig.advance_x;
        advance_y = orig.advance_y;
        s0 = orig.s0;
        t0 = orig.t0;
        s1 = orig.s1;
        t1 = orig.t1;
        kerning = orig.kerning;
        outline_type = orig.outline_type;
        outline_thickness = orig.outline_thickness;
    }

    TextureFont::Glyph& TextureFont::Glyph::operator = (const Glyph& orig)
    {
        charcode = orig.charcode;
        id = orig.id;
        width = orig.width;
        height = orig.height;
        offset_x = orig.offset_x;
        offset_y = orig.offset_y;
        advance_x = orig.advance_x;
        advance_y = orig.advance_y;
        s0 = orig.s0;
        t0 = orig.t0;
        s1 = orig.s1;
        t1 = orig.t1;
        kerning = orig.kerning;
        outline_type = orig.outline_type;
        outline_thickness = orig.outline_thickness;

        return *this;
    }

    TextureFont::TextureFont (TextureAtlas* atlas, const string& filename,
                              const float size)
        : _atlas(atlas), _height(0), _ascender(0), _descender(0),
          _filename(filename), _size(size), _outline_type(0),
          _outline_thickness(0), _hinting(1), _kerning(1), _filtering(1),
          _underline_thickness(0.0), _underline_position(0.0), _linegap (0.0)

    {
        FT_Size_Metrics metrics;

        // TODO Auto-generated constructor stub
        _lcd_weights[0] = 0x10;
        _lcd_weights[1] = 0x40;
        _lcd_weights[2] = 0x70;
        _lcd_weights[3] = 0x40;
        _lcd_weights[4] = 0x10;

        // Get font metrics at high resoltion
        FontType face (_filename, _size*100);

        if (!face.isValid()) return;

        // 64 * 64 because of 26.6 encoding AND the transform matrix used
        // in FontType::FontType( hres = 64)
        _underline_position = face.getFontFace()->underline_position
            / (float)(64.0f * 64.0f) * _size;

        _underline_position = round (_underline_position);
        if (_underline_position > -2) {
            _underline_position = -2.0;
        }

        _underline_thickness = face.getFontFace()->underline_thickness
            / (float)(64.0f * 64.0f) * _size;

        _underline_thickness = round(_underline_thickness);
        if(_underline_thickness < 1) {
            _underline_thickness = 1.0;
        }

        metrics = face.getFontFace()->size->metrics;
        _ascender = (metrics.ascender >> 6) / 100.0;
        _descender = (metrics.descender >> 6) / 100.0;

        _height = (metrics.height >> 6) / 100.0;
        _linegap = _height - _ascender + _descender;

        // -1 is a special glyph
        // getGlyph (-1);
    }

    TextureFont::~TextureFont ()
    {
        // TODO Auto-generated destructor stub
    }

    float TextureFont::getGlyphKerning (const Glyph& glyph,
                                        const wchar_t charcode)
    {
        size_t i;

        for(i = 0; i < glyph.kerning.size(); ++i)
        {
            Kerning kerning = glyph.kerning[i];
            if(kerning.charcode == charcode) {
                return kerning.kerning;
            }
        }

        return 0.0;
    }

    // TODO: use iterator of the _glyphs
    void TextureFont::generateKerning (void)
    {
        size_t i, j;

        FT_UInt glyph_index, prev_index;
        FT_Vector kerning;

        // Glyph glyph, prev_glyph; // wrong

        vector<Glyph>::iterator it;
        vector<Glyph>::iterator prev_it;

        FontType face(_filename, _size);

        if(! face.isValid()) return;

        // start from 1
        for (it = _glyphs.begin(), it++; it !=  _glyphs.end(); it++)
        {
            glyph_index = face.getCharIndex (it->charcode);
            it->kerning.clear();

            for (prev_it = _glyphs.begin(), prev_it++;
                 prev_it != _glyphs.end(); prev_it++)
            {
                prev_index = face.getCharIndex(prev_it->charcode);
                face.getKerning (prev_index,
                                 glyph_index,
                                 FT_KERNING_UNFITTED,
                                 &kerning);
                if(kerning.x) {
                    Kerning k;
                    k.charcode = prev_it->charcode;
                    k.kerning = kerning.x / (float) (64.0f * 64.0f);
                    it->kerning.push_back(k);
                }
            }
        }

    }

    size_t TextureFont::loadGlyphs (const wchar_t* chs)
    {
        size_t i, x, y, width, height, depth, w, h;

        FT_UInt glyph_index;
        FT_Glyph ft_glyph;
        FT_GlyphSlot slot;
        FT_Bitmap ft_bitmap;

        assert(_atlas);

        width = _atlas->getWidth();
        height = _atlas->getHeight();
        depth = _atlas->getDepth();

        FontType face(_filename, _size);
        if (!face.isValid()) {
            return wcslen(chs);
        }

        // load each glyph
        for (i = 0; i < wcslen(chs); ++i)
        {
            FT_Int32 flags = 0;
            int ft_bitmap_width = 0;
            int ft_bitmap_rows = 0;
            int ft_bitmap_pitch = 0;
            int ft_glyph_top = 0;
            int ft_glyph_left = 0;

            glyph_index = face.getCharIndex (chs[i]);
            // WARNING: We use texture-atlas depth to guess if user wants
            // LCD subpixel rendering
            if(_outline_type > 0) {
                flags |= FT_LOAD_NO_BITMAP;
            } else {
                flags |= FT_LOAD_RENDER;
            }

            if (_hinting) {
                flags |= FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT;
            } else {
                flags |= FT_LOAD_FORCE_AUTOHINT;
            }

            if(depth == 3) {
                face.setLcdFilter(FT_LCD_FILTER_LIGHT);
                flags |= FT_LOAD_TARGET_LCD;
                if(_filtering) {
                    face.setLcdFilterWeights(_lcd_weights);
                }
            }

            if (! face.loadGlyph(glyph_index, flags)) {
                return wcslen(chs) - i;
            }

            if(_outline_type == 0) {
                slot = face.getFontFace()->glyph;
                ft_bitmap = slot->bitmap;
                ft_bitmap_width = slot->bitmap.width;
                ft_bitmap_rows = slot->bitmap.rows;
                ft_bitmap_pitch = slot->bitmap.pitch;
                ft_glyph_top = slot->bitmap_top;
                ft_glyph_left = slot->bitmap_left;
            } else {
                FT_Stroker stroker;
                FT_BitmapGlyph ft_bitmap_glyph;
                stroker = face.getStroker();
                FT_Stroker_Set(stroker,
                               (int)(_outline_thickness * 64),
                               FT_STROKER_LINECAP_ROUND,
                               FT_STROKER_LINEJOIN_ROUND,
                               0);

            }

        }

        return 0;
    }

    TextureFont::Glyph* TextureFont::getGlyph (wchar_t ch)
    {
        assert(_atlas);

        size_t i;
        wchar_t buffer[2] = {0, 0};

        // Check if charcode has been already loaded
        for(i = 0; i < _glyphs.size(); i++)
        {
            if((_glyphs[i].charcode == ch) &&
               ((ch == (wchar_t)(-1)) ||
                ((_glyphs[i].outline_type == _outline_type) &&
                 (_glyphs[i].outline_thickness == _outline_thickness))))
            {
                return &(_glyphs[i]);
            }
        }

        // charcode -1 is special: it is used for line drawing (overline,
        // underline, strikethrough) and background.
        if(ch == (wchar_t)(-1))
        {
            size_t width = _atlas->getWidth();
            size_t height = _atlas->getHeight();

            Tuple4i region = _atlas->getRegion(5, 5);

            TextureFont::Glyph glyph;

            static unsigned char data[4 * 4 * 3] =
                {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                 -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
            if(region.rect.sx < 0) {
                cerr << "Texture atlas is full" << endl;
                return NULL;
            }

            _atlas->setRegion(region.rect.sx, region.rect.sy, 4, 4, data, 0);
            glyph.charcode = (wchar_t)(-1);
            glyph.s0 = (region.rect.sx + 2)/(float)width;
            glyph.t0 = (region.rect.sy + 2)/(float)height;
            glyph.s1 = (region.rect.sx + 3)/(float)width;
            glyph.t1 = (region.rect.sy + 3)/(float)height;
            _glyphs.push_back(glyph);
            return &(_glyphs.back());
        }

        // Glyph has not been already loaded
        buffer[0] = ch;

        // if (loadGlyphs(buffer) == 0) {
        //    return &(_glyphs.back());
        // }

        return NULL;
    }

} /* namespace BIL */
