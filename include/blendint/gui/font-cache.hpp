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

#include <blendint/core/string.hpp>
#include <blendint/core/object.hpp>

#include <blendint/font/fc-pattern.hpp>
#include <blendint/font/ft-library.hpp>
#include <blendint/font/ft-face.hpp>

#include <blendint/gui/texture-atlas.hpp>
#include <blendint/gui/glyph.hpp>

namespace BlendInt {

  class FontCache: public Object
  {
  public:

    static RefPtr<FontCache> Create (const Fc::Pattern& pattern);

    static bool Release (const Fc::Pattern& data);

    static inline size_t cache_size ()
    {
      return kCacheDB.size();
    }

    static inline size_t max_cache_size ()
    {
      return kMaxCacheSize;
    }

    FontCache (const Fc::Pattern& pattern);

    virtual ~FontCache ();

    const Glyph* Query (uint32_t charcode, bool create = true);

    size_t glyph_count () const
    {
      return glyph_data_.size();
    }

    const Fc::Pattern& pattern () const
    {
      return pattern_;
    }

    const RefPtr<TextureAtlas> texture_atlas () const
    {
      return texture_atlas_;
    }

  private:

    friend class Font;
    friend class AbstractWindow;

    static void ReleaseAll ();

    Fc::Pattern pattern_;

    Ft::Library library_;

    Ft::Face face_;

    RefPtr<TextureAtlas> texture_atlas_;

    std::map<uint32_t, Glyph> glyph_data_;

    static std::map<FcChar32, RefPtr<FontCache> > kCacheDB;

    static FcChar32 kDefaultFontHash;

    static const unsigned int kMaxCacheSize = 16;
  };

} /* namespace BlendInt */
