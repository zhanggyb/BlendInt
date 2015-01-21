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

#include <cassert>

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Font/FcConfig.hpp>

#include <BlendInt/Gui/Font.hpp>

namespace BlendInt {

	Font::Font ()
	: Object()
	{
		cache_ = FontCache::kCacheDB[FontCache::kDefaultFontHash];
	}

	Font::Font (const FcChar8* family, double size, int weight,
	        int slant)
	: Object()
	{
		Fc::Pattern p;
		p.add(FC_FAMILY, family);
		p.add(FC_SIZE, size);
		p.add(FC_WEIGHT, weight);
		p.add(FC_SLANT, slant);

		Fc::Config::substitute(0, p, FcMatchPattern);
		p.default_substitute();

		FcResult result;
		Fc::Pattern match = Fc::Config::match(0, p, &result);

		assert(match);

		if(result != FcResultMatch) {
			DBG_PRINT_MSG("Warning: %s", "the default font was not found");
		}

		cache_ = FontCache::Create(match);
	}

	Font::~Font ()
	{
	}

	void Font::SetFamily (const FcChar8* family)
	{
		Fc::Pattern p = Fc::Pattern::duplicate(cache_->pattern());

		if(!p.del(FC_FAMILY)) {
			DBG_PRINT_MSG("Warning: %s", "no faimliy property");
		}

		p.add(FC_FAMILY, family);

		Fc::Config::substitute(0, p, FcMatchPattern);
		p.default_substitute();

		FcResult result;
		Fc::Pattern match = Fc::Config::match(0, p, &result);

		assert(match);

		if(result != FcResultMatch) {
			DBG_PRINT_MSG("Warning: %s", "the default font was not found");
		}

		cache_ = FontCache::Create(match);
	}

	void Font::SetStyle (const FcChar8* style)
	{
	}

	void Font::SetFullName (const FcChar8* fullname)
	{
	}

	void Font::SetSlant (int slant)
	{
		Fc::Pattern p = Fc::Pattern::duplicate(cache_->pattern());

		if(!p.del(FC_SLANT)) {
			DBG_PRINT_MSG("Warning: %s", "no faimliy property");
		}

		p.add(FC_SLANT, slant);

		Fc::Config::substitute(0, p, FcMatchPattern);
		p.default_substitute();

		FcResult result;
		Fc::Pattern match = Fc::Config::match(0, p, &result);

		assert(match);

		if(result != FcResultMatch) {
			DBG_PRINT_MSG("Warning: %s", "the default font was not found");
		}

		cache_ = FontCache::Create(match);
	}

	void Font::SetWeight (int weight)
	{
		Fc::Pattern p = Fc::Pattern::duplicate(cache_->pattern());

		if(!p.del(FC_WEIGHT)) {
			DBG_PRINT_MSG("Warning: %s", "no faimliy property");
		}

		p.add(FC_WEIGHT, weight);

		Fc::Config::substitute(0, p, FcMatchPattern);
		p.default_substitute();

		FcResult result;
		Fc::Pattern match = Fc::Config::match(0, p, &result);

		assert(match);

		if(result != FcResultMatch) {
			DBG_PRINT_MSG("Warning: %s", "the default font was not found");
		}

		cache_ = FontCache::Create(match);
	}

	void Font::SetSize (double size)
	{
		Fc::Pattern p = Fc::Pattern::duplicate(cache_->pattern());

		if(!p.del(FC_SIZE)) {
			DBG_PRINT_MSG("Warning: %s", "no faimliy property");
		}

		p.add(FC_SIZE, size);

		Fc::Config::substitute(0, p, FcMatchPattern);
		p.default_substitute();

		FcResult result;
		Fc::Pattern match = Fc::Config::match(0, p, &result);

		assert(match);

		if(result != FcResultMatch) {
			DBG_PRINT_MSG("Warning: %s", "the default font was not found");
		}

		cache_ = FontCache::Create(match);
	}

	void Font::SetPixelSize (double pixel_size)
	{
		Fc::Pattern p = Fc::Pattern::duplicate(cache_->pattern());

		if(!p.del(FC_PIXEL_SIZE)) {
			DBG_PRINT_MSG("Warning: %s", "no faimliy property");
		}

		p.add(FC_PIXEL_SIZE, pixel_size);

		Fc::Config::substitute(0, p, FcMatchPattern);
		p.default_substitute();

		FcResult result;
		Fc::Pattern match = Fc::Config::match(0, p, &result);

		assert(match);

		if(result != FcResultMatch) {
			DBG_PRINT_MSG("Warning: %s", "the default font was not found");
		}

		cache_ = FontCache::Create(match);
	}

	size_t Font::GetTextWidth (const String& text) const
	{
		return GetTextWidth(text, text.length(), 0);
	}

	size_t Font::GetTextWidth (const String& text, size_t length,
	        size_t start) const
	{
		size_t width = 0;
		const Glyph* g = 0;

		size_t str_len = text.length();
		size_t last = std::min(start + length, str_len);
		size_t next = 0;

		if(has_kerning()) {

			Kerning kerning;
			for(size_t i = start; i < last; i++) {

				g = glyph(text[i]);
				next = i + 1;

				if(next < last) {
					kerning = GetKerning(text[i], text[next], KerningDefault);
					width += (g->advance_x + kerning.x);
				} else {
					width += g->advance_x;
				}

			}

		} else {

			for(size_t i = start; i < last; i++) {
				g = glyph(text[i]);
				width += g->advance_x;
			}

		}

		return width;
	}

    Kerning Font::GetKerning (uint32_t left_glyph, uint32_t right_glyph, KerningMode mode) const
    {
        Kerning retval;
        FT_Vector akerning;
        
        if(cache_->face_.get_kerning(left_glyph, right_glyph, mode, &akerning) == 0) {
            retval.x = akerning.x >> 6;
            retval.y = akerning.y >> 6;
        }
        
        return retval;
    }
    

}
