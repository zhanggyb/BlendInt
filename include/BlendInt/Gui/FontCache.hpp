/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_FONTCACHE_HPP_
#define _BLENDINT_FONTCACHE_HPP_

#include <map>
#include <functional>
#include <string>
#include <vector>

#include <glm/mat4x4.hpp>

#include <BlendInt/Core/String.hpp>

#include <BlendInt/Core/Object.hpp>
#include <BlendInt/Core/Freetype.hpp>
#include <BlendInt/Gui/Glyph.hpp>
#include <BlendInt/Gui/GlyphAtlas.hpp>

using namespace std;

namespace BlendInt {

	template<class T>
	struct greater_second: std::binary_function<T, T, bool>
	{
		inline bool operator() (const T& lhs, const T& rhs)
		{
			return lhs.second > rhs.second;
		}
	};

	enum FontStyleFlag {
		FontStyleBold = 0x0,
		FontStyleItalic = 0x1 << 1,
		FontStyleOutline = 0x1 << 2
	};

	/**
	 * @brief Basic structure to identify a font file and texture atlas
	 */
	struct FontTypeBase {

		FontTypeBase ()
		: size(9),
		  flag(0),
		  dpi(72),
		  thickness(0.5f)
		{

		}

		~FontTypeBase ()
		{

		}

		FontTypeBase (const FontTypeBase& orig)
		: name(orig.name),
		  size(orig.size),
		  flag(orig.flag),
		  dpi(orig.dpi),
		  thickness(orig.thickness)
		{

		}

		FontTypeBase& operator = (const FontTypeBase& orig)
		{
			name = orig.name;
			size = orig.size;
			flag = orig.flag;
			dpi = orig.dpi;
			thickness = orig.thickness;

			return *this;
		}

		std::string name;	/** The font name or filepath to the font file */
		unsigned int size;	/** Character size want to be loaded */
		unsigned int flag;	/** The font style flag, see FontStyleFlag */
		unsigned int dpi;	/** The DPI used in Freetype setting */
		float thickness;	/** The thickness of outline */
	};

	extern bool operator < (const FontTypeBase& src, const FontTypeBase& dist);
	extern bool operator == (const FontTypeBase& src, const FontTypeBase& dist);

	class FontCache: public Object
	{
	public:

		static RefPtr<FontCache> Create (const FontTypeBase& data);

		static bool Release (const FontTypeBase& data);

		static void ReleaseAll ();

		static size_t GetCacheSize ();

		static void SetDefaultTextureSize (int width, int height);

		static int GetDefaultTextureWidth ();

		static int GetDefaultTextureHeight ();

#ifdef DEBUG
		static void list ();
#endif

		const GlyphExt* Query (const FontTypeBase& font_data, uint32_t charcode, bool create = true);

	private:

		friend class Font;
		template <typename T> friend class RefPtr;

		FontCache (const FontTypeBase& data);

		~FontCache ();

		void Initialize (const FontTypeBase& font_data, uint32_t char_code = 32, int size = 95);

		void SetGlyphData (GlyphExt& glyph, int x, int y, FT_GlyphSlot slot, const RefPtr<TextureAtlas2D>& atlas);

		void SetGlyphData (GlyphExt& glyph, int x, int y, FT_GlyphSlot slot, FT_BitmapGlyph bitmap_glyph, const RefPtr<TextureAtlas2D>& atlas);

		GLuint m_vao;

		GLuint m_vbo;

		uint32_t m_start;
		int m_size;

		std::vector<GlyphExt> m_preset;

		std::map<uint32_t, GlyphExt> m_extension;

		RefPtr<GlyphAtlas> m_last;

		FTLibrary m_ft_lib;
		FTFace m_ft_face;

		static map<FontTypeBase, RefPtr<FontCache> > cache_db;

		static int default_texture_width;
		static int default_texture_height;
	};

} /* namespace BlendInt */
#endif /* FONTCACHE_H_ */
