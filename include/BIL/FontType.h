/*
 * FontType.h
 *
 *  Created on: 2013年7月9日
 *      Author: zhanggyb
 */

#ifndef _BIL_FONTTYPE_H_
#define _BIL_FONTTYPE_H_

#include <string>
#include <vector>
#include <map>
#include <GL/gl.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_LCD_FILTER_H
#include FT_STROKER_H

#include <BIL/Tuple.h>
#include <BIL/Glyph.h>

using namespace std;

namespace BIL {

	typedef Tuple2<FT_Pos> Tuple2long;

	class FontType
	{
	public:

		/**
		 * @brief Constructor to create object from a font file
		 * @param filename File path name
		 * @param size Font size
		 */
		FontType (const string& filename, unsigned int size = 10);

		/**
		 * @brief Constructor to create object from memory
		 * @param buffer A pointer to the beginning of the font data in memory
		 * @param bufsize The size of the memory chunk used by the font data
		 * @param index The index of the face withing the font. Default is 0
		 * @param size Font size
		 */
		FontType (const FT_Byte* buffer, FT_Long bufsize, FT_Long index = 0,
		        unsigned int size = 10);

		/**
		 * @brief destructor
		 *
		 * @warning Do not delete gFontService before any FontType
		 * object destructed
		 */
		virtual ~FontType ();

		bool isValid (void) const
		{
			return _valid;
		}

		const FT_Face& getFontFace (void) const
		{
			return _face;
		}

		const FT_Stroker& getStroker (void) const
		{
			return _stroker;
		}

		bool setFontSize (unsigned int size, unsigned int dpi = 96);

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

		Vec2l getKerning (const Glyph& left, const Glyph& right, FT_UInt kern_mode = FT_KERNING_DEFAULT);

	private:

		bool getKerning (FT_UInt left_glyph, FT_UInt right_glyph,
		        FT_UInt kern_mode, FT_Vector *akerning);

	private:

		FT_Library _library; /**< Freetype Library */

		FT_Face _face; /**< freetype2 face */

		FT_Stroker _stroker; /**< Font stroker */

		bool _valid; /**< if the font face is valid */

		bool _unicode; /**< if has unicode charmap */

		unsigned int _fontsize;

		unsigned int _dpi;

		string _file; /**< path name of the font file */
	};

} /* namespace BIL */

#endif /* _BIL_FONTTYPE_H_ */
