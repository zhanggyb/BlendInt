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

#ifndef _BLENDINT_FREETYPE_HPP_
#define _BLENDINT_FREETYPE_HPP_

#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_LCD_FILTER_H
#include FT_STROKER_H

#include <BlendInt/Core/Rect.hpp>

using namespace std;

namespace BlendInt {

	/**
	 * @brief Wrapper of Freetype API
	 */
	class Freetype
	{
	public:

		Freetype ();

		/**
		 * @brief destructor
		 *
		 * @warning Do not delete gFontService before any FontType
		 * object destructed
		 */
		virtual ~Freetype ();

		/**
		 * @brief open font file
		 * @param filename File path name
		 * @param size Font size
		 */
		bool Open (const std::string& filename,
					unsigned int size = 9,
					unsigned int dpi = 96);

		/**
		 * @brief open font from memory
		 * @param buffer A pointer to the beginning of the font data in memory
		 * @param bufsize The size of the memory chunk used by the font data
		 * @param index The index of the face withing the font. Default is 0
		 * @param size Font size
		 */
		bool Open (const FT_Byte* buffer, FT_Long bufsize, FT_Long index = 0,
				unsigned int size = 9, unsigned int dpi = 96);

		void Close ();

		bool valid () const
		{
			return m_valid;
		}

		bool IsUseKerning (void);

		int height () const
		{
			return m_valid ? m_height : 0;
		}

		int ascender () const
		{
			return m_valid ? m_ascender : 0;
		}

		int descender () const
		{
			return m_valid ? m_descender : 0;
		}

		int max_advance () const
		{
			return m_valid ? m_max_advance : 0;
		}

		FT_GlyphSlot GetGlyphSlot () const;

		const FT_Stroker& stroker () const
		{
			return m_stroker;
		}

		//bool setFontSize (unsigned int size, unsigned int dpi = 96);

		bool SetCharSize (unsigned int size, unsigned int dpi = 96);

		bool SetPixelSize (unsigned int width, unsigned int height);

		/**
		 * @brief Get the glyph index of a given character code
		 * @param charcode The character code
		 * @return The glyph index. 0 means 'undefined character code'
		 */
		FT_UInt GetCharIndex (const FT_ULong charcode);

		/**
		 * @brief Load a single glyph into the glyph slot of a face object
		 * @param glyph_index The index of the glyph in the font file
		 * @param load_flags A flag indicating what to load for this glyph
		 * @return true for success, false for error
		 */
		bool LoadGlyph (FT_UInt glyph_index, FT_Int32 load_flags =
				FT_LOAD_DEFAULT);

		/**
		 * Call this member function after loadGlyph()
		 */
		bool RenderGlyph (FT_Render_Mode render_mode = FT_RENDER_MODE_NORMAL);

		/**
		 * @brief Load a single glyph into the glyph slot of a face object
		 * @param charcode Character code
		 * @param A flag indicating what to load for this glyph
		 *
		 * Equivalent to calling getCharIndex() then loadGlyph()
		 *
		 * Set load_flags to FT_LOAD_RENDER to convert the glyph image to an
		 * anti-aliased bitmap immediately. This can avoid calling
		 * renderGlyph()
		 */
		bool LoadCharacter (FT_ULong charcode, FT_Int32 load_flags) const;

		bool SetLcdFilter (FT_LcdFilter filter);

		bool SetLcdFilterWeights (unsigned char* weights);

		FT_Vector GetKerning (FT_UInt left, FT_UInt right, FT_UInt mode = FT_KERNING_DEFAULT);

//		Tuple2l getKerning (const wchar_t& left,
//						  const wchar_t& right,
//						  FT_UInt kern_mode = FT_KERNING_DEFAULT);

		unsigned int dpi () const
		{
			return m_dpi;
		}

		void set_dpi (unsigned int dpi)
		{
			m_dpi = dpi;
		}

		const FT_Face& face () const
		{
			return m_face;
		}

	private:

		bool getKerning (FT_UInt left_glyph, FT_UInt right_glyph,
				FT_UInt kern_mode, FT_Vector *akerning);

		FT_Library m_library; /**< Freetype Library */

		FT_Face m_face; /**< freetype2 face */

		FT_Stroker m_stroker; /**< Font stroker */

		bool m_valid; /**< if the font face is valid */

		bool m_unicode; /**< if has unicode charmap */

		/** used to compute a default line spacing */
		int m_height;

		/**/
		int m_ascender;

		/**/
		int m_descender;

		/** Max horizontal advance */
		int m_max_advance;

		unsigned int m_dpi;

		Freetype (const Freetype& orig);

		Freetype& operator = (const Freetype& orig);
	};

} /* namespace BlendInt */

#endif /* _BIL_FREETYPE_H_ */
