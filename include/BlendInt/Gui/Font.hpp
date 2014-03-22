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

#include <glm/mat4x4.hpp>

#include <BlendInt/Core/Rect.hpp>
#include <BlendInt/Core/String.hpp>

#include <BlendInt/Gui/FontCache.hpp>

using namespace std;

namespace BlendInt {

	class Font
	{
	public:

		Font (const std::string& family = std::string("Sans"), unsigned int size = 9,
		        bool bold = false, bool italic = false);

		/*
		Font (const std::string& family,
				unsigned int size = 9,
        bool bold = false,
        bool italic = false);
		*/

		Font (const char* family, unsigned int size = 9, bool bold = false, bool italic = false);

		Font (const Font& orig);

		Font& operator = (const Font& orig);

		void Print (const glm::mat4& mvp, const String& string, size_t start = 0)
		{
			m_cache->Print(mvp, string, start);
		}

		void Print (const glm::mat4& mvp, const String& string, size_t length, size_t start = 0)
		{
			m_cache->Print(mvp, string, length, start);
		}

		void Print (const glm::mat4& mvp, float x, float y, const String& string, size_t start = 0)
		{
			m_cache->Print(mvp, x, y, string, start);
		}

		void Print (const glm::mat4& mvp, float x, float y, const String& string, size_t length, size_t start = 0)
		{
			m_cache->Print(mvp, x, y, string, length, start);
		}

		int get_height () const
		{
			return m_cache->m_freetype.height();
		}

		int get_ascender () const
		{
			return m_cache->m_freetype.ascender();
		}

		int get_descender () const
		{
			return m_cache->m_freetype.descender();
		}

		int get_max_advance () const
		{
			return m_cache->m_freetype.max_advance();
		}

		Rect get_text_outline (const String& string)
		{
			return m_cache->get_text_outline(string);
		}

		unsigned int GetTextWidth (const String& string, size_t length, size_t start = 0)
		{
			return m_cache->GetTextWidth(string, length, start);
		}

		unsigned int get_text_height () const
		{
			return m_cache->get_text_height();
		}

		/**
		 * @brief the font family, e.g. "Droid Sans"
		 *
		 * @note Currently cannot support non-English family
		 */
		String family;

		/** font size */
		unsigned int size;

		/** whether text is bold */
		bool bold;

		/** whether text is italic */
		bool italic;

		RefPtr<FontCache> m_cache;
	};

} /* namespace BlendInt */
#endif /* FONT_H_ */
