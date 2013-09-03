/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_FREETYPE_H_
#define _BIL_FREETYPE_H_

#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_LCD_FILTER_H
#include FT_STROKER_H

#include <BIL/Tuple.hpp>
#include <BIL/Glyph.hpp>
#include <BIL/Font.hpp>
#include <BIL/Rect.hpp>

using namespace std;

namespace BIL {

	/**
	 * @brief Wrapper of Freetype API
	 */
	class Freetype
	{
	public:

		Freetype ();

		/**
		 * @brief open a font family
		 * @param font
		 * @param dpi
		 * @return
		 */
		bool open (const Font& font = Font("Sans"),
					unsigned int dpi = 96);

		/**
		 * @brief open font file
		 * @param filename File path name
		 * @param size Font size
		 */
		bool open (const std::string& filename,
					unsigned int size = 9,
					unsigned int dpi = 96);

		/**
		 * @brief open font from memory
		 * @param buffer A pointer to the beginning of the font data in memory
		 * @param bufsize The size of the memory chunk used by the font data
		 * @param index The index of the face withing the font. Default is 0
		 * @param size Font size
		 */
		bool open (const FT_Byte* buffer, FT_Long bufsize, FT_Long index = 0,
				unsigned int size = 9, unsigned int dpi = 96);

		void close ();

		/**
		 * @brief destructor
		 *
		 * @warning Do not delete gFontService before any FontType
		 * object destructed
		 */
		virtual ~Freetype ();

		bool valid ()
		{
			return valid_;
		}

		bool isUseKerning (void);

		int height () const
		{
			if(!valid_) return 0;

			return height_;
		}

		int ascender () const
		{
			if(!valid_) return 0.0;
			return ascender_;
		}

		int descender () const
		{
			if(!valid_) return 0.0;
			return descender_;
		}

		int max_advance () const
		{
			if(!valid_) return 0.0;
			return max_advance_;
		}

		const FT_Face& getFontFace (void) const
		{
			return face_;
		}

		const FT_Stroker& getStroker (void) const
		{
			return stroker_;
		}

		//bool setFontSize (unsigned int size, unsigned int dpi = 96);

		bool setCharSize (unsigned int size, unsigned int dpi = 96);

		/**
		 * @brief Get the glyph index of a given character code
		 * @param charcode The character code
		 * @return The glyph index. 0 means 'undefined character code'
		 */
		FT_UInt getCharIndex (const FT_ULong charcode);

		/**
		 * @brief Load a single glyph into the glyph slot of a face object
		 * @param glyph_index The index of the glyph in the font file
		 * @param load_flags A flag indicating what to load for this glyph
		 * @return true for success, false for error
		 */
		bool loadGlyph (FT_UInt glyph_index, FT_Int32 load_flags =
				FT_LOAD_DEFAULT);

		/**
		 * Call this member function after loadGlyph()
		 */
		bool renderGlyph (FT_Render_Mode render_mode = FT_RENDER_MODE_NORMAL);

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
		bool loadCharacter (FT_ULong charcode, FT_Int32 load_flags);

		bool setLcdFilter (FT_LcdFilter filter);

		bool setLcdFilterWeights (unsigned char* weights);

		Tuple2l getKerning (const Glyph& left,
						  const Glyph& right,
						  FT_UInt kern_mode = FT_KERNING_DEFAULT);

		const Font& font () const
		{
			return font_;
		}

		unsigned int dpi () const
		{
			return dpi_;
		}

		void set_dpi (unsigned int dpi)
		{
			dpi_ = dpi;
		}

	private:

		bool getKerning (FT_UInt left_glyph, FT_UInt right_glyph,
				FT_UInt kern_mode, FT_Vector *akerning);

	private:

		FT_Library library_; /**< Freetype Library */

		FT_Face face_; /**< freetype2 face */

		FT_Stroker stroker_; /**< Font stroker */

		bool valid_; /**< if the font face is valid */

		bool unicode_; /**< if has unicode charmap */

		/** used to compute a default line spacing */
		int height_;

		/**/
		int ascender_;

		/**/
		int descender_;

		/** Max horizontal advance */
		int max_advance_;

		Font font_;

		unsigned int dpi_;
	};

} /* namespace BIL */

#endif /* _BIL_FREETYPE_H_ */
