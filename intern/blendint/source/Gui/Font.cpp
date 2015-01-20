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

#include <string.h>
#include <cwchar>

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Font/FcConfig.hpp>

#include <BlendInt/Gui/Font.hpp>

namespace BlendInt {

	Font::Font ()
	: Object()
	{
		Fc::Pattern p;

#ifdef __LINUX__
		p.add(FC_FAMILY, "Sans");
#endif
#ifdef __APPLE__
		p.add(FC_FAMILY, "Helvetica Neue");
#endif

		p.add(FC_SIZE, 12);
		p.add(FC_WEIGHT, FC_WEIGHT_REGULAR);
		p.add(FC_SLANT, FC_SLANT_ROMAN);

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
		// TODO: implement this function
		return 0;
	}

	size_t Font::GetTextWidth (const String& string, size_t length,
	        size_t start) const
	{
		// TODO: implement this function
		return 0;
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
