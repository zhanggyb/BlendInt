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
         * @brief Default constructor
         */
        FontType (const string& filename,
                  const float size = 10.0);

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

        bool loadGlyph (char ch);

        bool loadGlyph (FT_UInt glyph_index, FT_Int32 load_flags);

        void setFontSize (GLuint size, GLuint dpi);

        bool setCharSize (float size, int dpi = 72);

        FT_UInt getCharIndex (const FT_ULong ch);

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

        string _file;           /**< font file */

        bool _valid;            /**< if the font face is valid */

        bool _unicode;          /**< if has unicode charmap */
    };

} /* namespace BIL */

#endif /* _BIL_FONTTYPE_H_ */
