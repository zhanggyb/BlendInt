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

#pragma once

#include <map>
#include <string>
#include <vector>

#include <glm/mat4x4.hpp>

#include <BlendInt/Core/String.hpp>

#include <BlendInt/Core/Object.hpp>
#include <BlendInt/Core/Freetype.hpp>
#include <BlendInt/Gui/Glyph.hpp>
#include <BlendInt/Gui/GlyphAtlas.hpp>

#include <BlendInt/Font/FcPattern.hpp>
#include <BlendInt/Font/FtLibrary.hpp>
#include <BlendInt/Font/FtFace.hpp>

namespace BlendInt {

	class FontCache: public Object
	{
	public:

		static RefPtr<FontCache> Create (const Fc::Pattern& pattern);

		static bool Release (const Fc::Pattern& data);

		static void ReleaseAll ();

		static size_t GetCacheSize ();

		FontCache (const Fc::Pattern& pattern);

		virtual ~FontCache ();

		const GlyphMetrics* Query (uint32_t charcode, bool create = true);

		size_t glyph_count () const
		{
			return glyph_data_.size();
		}

		const Fc::Pattern& pattern () const
		{
			return pattern_;
		}

		const RefPtr<TextureAtlas2DExt> texture_atlas () const
		{
			return texture_atlas_;
		}

	private:

		friend class Font;
		friend class Context;

		Fc::Pattern pattern_;

		Ft::Library library_;

		Ft::Face face_;

		RefPtr<TextureAtlas2DExt> texture_atlas_;

		std::map<uint32_t, GlyphMetrics> glyph_data_;

		static map<FcChar32, RefPtr<FontCache> > kCacheDB;

		static FcChar32 kDefaultFontHash;
	};

} /* namespace BlendInt */
