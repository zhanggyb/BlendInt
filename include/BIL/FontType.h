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
        FontType (const string& filename,
                  float size = 10.0);

        /**
         * @brief Constructor to create object from memory
         * @param buffer A pointer to the beginning of the font data in memory
         * @param bufsize The size of the memory chunk used by the font data
         * @param index The index of the face withing the font. Default is 0
         * @param size Font size
         */
        FontType (const FT_Byte* buffer,
                  FT_Long bufsize,
                  FT_Long index = 0,
                  float size = 10.0);

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

        const FT_Face& getFontFace (void) const {return _face;}

        const FT_Stroker& getStroker (void) const {return _stroker;}

        bool loadGlyph (char charcode);

        /**
         * @brief Load a single glyph into the glyph slot of a face object
         * @param glyph_index The index of the glyph in the font file
         * @param load_flags A flag indicating what to load for this glyph
         * @return true for success, false for error
         */
        bool loadGlyph (FT_UInt glyph_index,
                        FT_Int32 load_flags = FT_LOAD_DEFAULT);

        void setFontSize (GLuint size, GLuint dpi);

        bool setCharSize (float size, int dpi = 72);

        /**
         * @brief Set character size
         * @param width The normal width, in 26.6 fractional points
         * @param height The normal height, in 26.6 fractional points
         * @param horz_res The horizontal resolution in dpi
         * @param vert_res The vertical resolution in dpi
         * @return true for success, false for error
         */
        bool setCharSize (FT_F26Dot6 width,
                          FT_F26Dot6 height,
                          FT_UInt horz_res,
                          FT_UInt vert_res);

        /**
         * @brief Get the glyph index of a given character code
         * @param charcode The character code
         * @return The glyph index. 0 means 'undefined character code'
         */
        FT_UInt getCharIndex (const FT_ULong charcode);

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
        bool loadChar (FT_ULong charcode, FT_Int32 load_flags);

        bool renderGlyph (FT_Render_Mode render_mode);

        bool setLcdFilter (FT_LcdFilter filter);

        bool setLcdFilterWeights (unsigned char* weights);

        bool getKerning (FT_UInt left_glyph,
                         FT_UInt right_glyph,
                         FT_UInt kern_mode,
                         FT_Vector *akerning);

    private:

        FT_Library _library;    /**< Freetype Library */

        FT_Face _face;          /**< freetype2 face */

        FT_Stroker _stroker;    /**< Font stroker */

        bool _valid;            /**< if the font face is valid */

        bool _unicode;          /**< if has unicode charmap */

        string _file;           /**< path name of the font file */
    };

} /* namespace BIL */

#endif /* _BIL_FONTTYPE_H_ */
