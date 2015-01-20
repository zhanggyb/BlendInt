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

#ifndef _BLENDINT_GUI_FONT_HPP_
#define _BLENDINT_GUI_FONT_HPP_

#include <BlendInt/Core/Rect.hpp>
#include <BlendInt/Core/String.hpp>
#include <BlendInt/Core/Color.hpp>

#include <BlendInt/Gui/FontCache.hpp>

using namespace std;

namespace BlendInt {

	class Font
	{
	public:

#ifdef __LINUX__
		Font (const std::string& name = std::string("Droid Sans"),
						unsigned int size = 12,
						int flag = 0);
#endif

#ifdef __APPLE__
		Font (const std::string& family = std::string("Helvetical Neue"),
						unsigned int size = 12,
						int flag = 0);
#endif

		Font (const Font& orig);

		~Font ()
		{

		}

		Font& operator = (const Font& orig);

		void SetName (const std::string& name);

		void SetSize (unsigned int size);

		void SetBold (bool bold);

		bool IsBold () const
		{
			return m_data.flag & FontStyleBold;
		}

		void SetItalic (bool italic);

		bool IsItalic () const
		{
			return m_data.flag & FontStyleItalic;
		}

		void SetOutline (bool outline, float thickness = 0.5f);

		bool IsOutline () const
		{
			return m_data.flag & FontStyleOutline;
		}

		void SetOutlineThickness (float thickness);

		void SetShadow (bool shadow, float offset_x = 1.5f, float offset_y = -1.5f);

		int Print (float x, float y, const char* string, size_t start = 0) const;

		int Print (float x, float y, const char* string, size_t length, size_t start) const;

		int Print (float x, float y, const std::string& string, size_t start = 0) const;

		int Print (float x, float y, const std::string& string, size_t length, size_t start) const;

		int Print (float x, float y, const String& string, size_t start = 0) const;

		int Print (float x, float y, const String& string, size_t length, size_t start) const;

		int GetHeight () const
		{
			return m_cache->m_ft_face.face()->size->metrics.height >> 6;
		}

		int GetAscender () const
		{
			return m_cache->m_ft_face.face()->size->metrics.ascender >> 6;
		}

		int GetDescender () const
		{
			return m_cache->m_ft_face.face()->size->metrics.descender >> 6;
		}

		int GetMaxAdvance () const
		{
			return m_cache->m_ft_face.face()->size->metrics.max_advance >> 6;
		}

		Rect GetTextOutline (const String& string) const;

		size_t GetTextWidth (const std::string& string) const;

		size_t GetTextWidth (const std::string& string, size_t length, size_t start) const;

		size_t GetTextWidth (const String& string) const;

		size_t GetTextWidth (const String& string, size_t length, size_t start) const;

		size_t GetReversedTextWidth (const std::string& string) const;

		size_t GetReversedTextWidth (const std::string& string, size_t length, size_t start) const;

		size_t GetReversedTextWidth (const String& string) const;

		size_t GetReversedTextWidth (const String& string, size_t length, size_t start) const;

		void set_pen (const Point& pen)
		{
			m_pen = pen;
		}

		void set_pen (int x, int y)
		{
			m_pen.set_x(x);
			m_pen.set_y(y);
		}

		const Point& pen () const
		{
			return m_pen;
		}

		void set_color (const Color& color)
		{
			m_color = color;
		}

		const Color& color () const
		{
			return m_color;
		}

#ifdef DEBUG

		const RefPtr<GLTexture2D>& GetTexture (uint32_t character);

#endif

	private:

		FontTypeBase m_data;

		Point m_pen;

		Color m_color;	// for foreground

		bool m_shadow;

		float m_shadow_offset_x;
		float m_shadow_offset_y;

		// Color m_background	// for shadow

		RefPtr<FontCache> m_cache;
	};

	// ------------------------

	/**
	 *
	 */
	class FontExt: public Object
	{

	public:

		FontExt();
		
		FontExt (const FcChar8* family,
				double size,
				int weight = FC_WEIGHT_REGULAR,
				int slant = FC_SLANT_ROMAN);

		virtual ~FontExt();

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

		const GlyphMetrics* glyph (uint32_t charcode) const
		{
			return cache_->Query(charcode, true);
		}

		void bind () const
		{
			cache_->texture_atlas()->bind();
		}

		void reset () const
		{
			cache_->texture_atlas()->reset();
		}

	private:

		RefPtr<FontCacheExt> cache_;

	};
	
} /* namespace BlendInt */
#endif /* FONT_H_ */
