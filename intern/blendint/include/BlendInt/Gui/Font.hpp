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

#pragma once

#include <BlendInt/Core/Rect.hpp>
#include <BlendInt/Core/String.hpp>

#include <BlendInt/Gui/FontCache.hpp>

using namespace std;

namespace BlendInt {

    /**
     * @brief Font kerning
     */
    struct Kerning
    {
        Kerning()
        : x(0), y(0)
        {}
        
        Kerning(int xi, int yi)
        : x(xi), y(yi)
        {}
        
        Kerning (const Kerning& orig)
        : x(orig.x), y(orig.y)
        {
        }

        Kerning& operator = (const Kerning& orig)
        {
            x = orig.x;
            y = orig.y;
            
            return *this;
        }
        
        int x;
        int y;
    };
    
	/**
	 *
	 */
	class Font: public Object
	{

	public:

        enum KerningMode {
            KerningDefault = FT_KERNING_DEFAULT,
            KerningUnfitted = FT_KERNING_UNFITTED,
            KerningUnscaled = FT_KERNING_UNSCALED
        };
        
		Font();
		
        Font (const FcChar8* name);
        
		Font (const FcChar8* family,
				double size,
				int weight = FC_WEIGHT_REGULAR,
				int slant = FC_SLANT_ROMAN);

		Font (const Font& orig)
		: Object()
		{
			cache_ = orig.cache_;
		}

		virtual ~Font();

		Font& operator = (const Font& orig)
		{
			cache_ = orig.cache_;
			return *this;
		}

		void SetFamily (const FcChar8* family);

		/**
		 * @brief Set font style, overrides weight and slant
		 */
		void SetStyle (const FcChar8* style);

		void SetFullName (const FcChar8* fullname);

		/**
		 * @brief Set font slant
		 *
		 * Italic, oblique or roman
		 */
		void SetSlant (int slant);

		/**
		 * @brief Set font weight
		 *
		 * Light, medium, demibold, bold or blank
		 */
		void SetWeight (int weight);

		void SetSize (double size);

		void SetPixelSize (double pixel_size);

		size_t GetTextWidth (const String& text) const;

		size_t GetTextWidth (const String& string, size_t length, size_t start) const;

        Kerning GetKerning (uint32_t left_glyph, uint32_t right_glyph, KerningMode mode = KerningDefault) const;
        
		const Glyph* glyph (uint32_t charcode) const
		{
			return cache_->Query(charcode, true);
		}

		int height () const
		{
			return cache_->face_.face()->size->metrics.height >> 6;
		}

		int ascender () const
		{
			return cache_->face_.face()->size->metrics.ascender >> 6;
		}

		int descender () const
		{
			return cache_->face_.face()->size->metrics.descender >> 6;
		}

		int max_advance () const
		{
			return cache_->face_.face()->size->metrics.max_advance >> 6;
		}

		bool has_kerning () const
		{
			return cache_->face_.has_kerning();
		}

		void bind_texture () const
		{
			cache_->texture_atlas()->bind();
		}

		void release_texture () const
		{
			cache_->texture_atlas()->reset();
		}

	private:

		friend inline bool operator == (const Font& src, const Font& dst);

		RefPtr<FontCache> cache_;

	};
	
	inline bool operator == (const Font& src, const Font& dst)
	{
		return src.cache_->pattern().hash() == dst.cache_->pattern().hash();
	}

} /* namespace BlendInt */
