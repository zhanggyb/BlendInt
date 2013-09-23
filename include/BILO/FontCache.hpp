/*
 * This file is part of BILO (Blender-like Interface Library in
 * OpenGL).
 *
 * BILO (Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BILO_FONTCACHE_HPP_
#define _BILO_FONTCACHE_HPP_

#include <map>
#include <functional>
#include <wchar.h>

#include <BILO/Freetype.hpp>
#include <BILO/Glyph.hpp>

#include <BILO/TextureFont.hpp>
#include <BILO/TextureAtlas.hpp>

using namespace std;

namespace BILO {

	class Font;
	class String;

	template<class T>
	struct greater_second: std::binary_function<T, T, bool>
	{
		inline bool operator() (const T& lhs, const T& rhs)
		{
			return lhs.second > rhs.second;
		}
	};

	/**
	 * @brief Class in charge of caching fonts
	 */
	class FontCache
	{
	public:
		// static member functions

		/**
		 * @brief create or get the FontCache object
		 * @param font Font, default is regular "Sans"
		 * @param force if true, remove unused cache and create one
		 * @return cache object created, NULL for failure
		 */
		static FontCache* create (const Font& font = Font("Sans"),
				unsigned int dpi = 96, bool force = true);

		static FontCache* getCache (const Font& font = Font("Sans"), unsigned int dpi = 96);

		static bool release (const Font& font = Font("Sans"));

		static void releaseAll ();

		static void setMaxCaches (unsigned int size)
		{
			maxCaches = size;
		}


#ifdef DEBUG
		static void list ();
#endif

	public:



		/**
		 * @brief query the Glyph of a wchar
		 * @param charcode wchar character
		 * @param create create a glyph if not found
		 * @return pointer to a Glyph object
		 *
		 * @warning Do not delete the pointer get from this funciton
		 */
		//Glyph* query (wchar_t charcode, bool create = true);

		const Glyph& queryGlyph (wchar_t charcode, bool create = true);

		const GLuint queryTexture (wchar_t charcode, bool create = true);

		unsigned int queryWidth (wchar_t charcode, bool create = true);

		unsigned int queryHeight (wchar_t charcode, bool create = true);

		void print (const String& string);

		void print (const String& string, size_t length);

		void print (float x, float y, const String& string);

		void print (float x, float y, const String& string, size_t length);

		int getHeight ()
		{
			if(!m_freetype) return 0;
			
			return m_freetype->height();
		}

		int getAscender ()
		{
			if(m_freetype == NULL) return 0;
			return m_freetype->ascender();
		}

		int getDescender ()
		{
			if(m_freetype == NULL) return 0;
			return m_freetype->descender();
		}

		int getMaxAdvance ()
		{
			if(m_freetype == NULL) return 0;
			return m_freetype->max_advance();
		}

		Tuple2l getKerning (const wchar_t& left, const wchar_t& right,
				FT_UInt kern_mode = FT_KERNING_DEFAULT)
		{
			return m_freetype->getKerning(left, right, kern_mode);
		}

		const Freetype* fontengine (void) const
		{
			return m_freetype;
		}

		void set_dpi (unsigned int dpi)
		{
			if(m_freetype) {
				m_freetype->set_dpi (dpi);
			}
		}

		Rect calculateOutline (const String& string);

#ifdef DEBUG
		void printcount ();
#endif

	private:

		/**
		 * @brief Initialize glyph database
		 * @return true for success, false for failure
         *
         * @warning Do not simply initialize all characer in ascii
         * cause render error. I dont't know why this happens
		 */
		bool setup ();

		/**
		 * @brief Default constructor
		 * @param font Font type
		 * @param dpi the DPI to be used
		 */
		FontCache (const Font& font, unsigned int dpi = 96);

		/**
		 * @brief private destructor
		 *
		 * Move destructor to private area to avoid this object be deleted
		 * in anywhere.
		 */
		~FontCache ();

	private:
		// member variables

		Freetype* m_freetype;

		TextureAtlas atlas_;

		//map<wchar_t, TextureFont*> texture_fonts_;

	private:
		// static members

		static unsigned int maxCaches;

		static map<Font, FontCache*> cacheDB;
		static map<Font, unsigned long> cacheCountDB;

	private:
		// member functions disabled

		FontCache ();

		FontCache (const FontCache& orig);

		FontCache& operator = (const FontCache& orig);
	};

} /* namespace BILO */
#endif /* FONTCACHE_H_ */
