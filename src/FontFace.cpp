/*
 * FontFace.cpp
 *
 *  Created on: 2013年7月9日
 *      Author: zhanggyb
 */

#include <iostream>

#include <BIL/FontFace.h>

using namespace std;

namespace BIL {

    FontFace::FontFace (FT_Library& lib,
                        const string& filename,
                        const float size)
        : _file(filename), _face(NULL), _valid(false), _unicode(false)
    {
        size_t hres = 64;
        FT_Error error;
        FT_Matrix matrix = { (int)((1.0/hres) * 0x10000L),
                             (int)((0.0)      * 0x10000L),
                             (int)((0.0)      * 0x10000L),
                             (int)((1.0)      * 0x10000L) };

        error = FT_New_Face(lib, filename.c_str(), 0, &_face);
        if (error == FT_Err_Unknown_File_Format) {
            cerr << "Unknown font file format: " << filename << endl;
        }

        if (0 == error) {
            _valid = true;

            // store postscript name of the font
            const char *name = FT_Get_Postscript_Name(_face);
            if (name != NULL) {
                _psName = name;
            }

            error = FT_Select_Charmap (_face, FT_ENCODING_UNICODE);
            if (error) {
                cerr << "Cannot set the unicode character map: "
                     << filename << endl;
            } else {
                _unicode = true;
            }

            /* Set char size */
            error = FT_Set_Char_Size (_face, (int)(size * 64), 0, 72*hres, 72);
            if(error) {
                cerr << "Cannot set character size" << endl;
            } else {
                FT_Set_Transform (_face, &matrix, NULL);
            }
        }

    }

    FontFace::~FontFace ()
    {
        if ((_face != NULL) && _valid) {
            FT_Done_Face(_face);
        }
    }

    bool FontFace::loadGlyph (char ch)
    {
        bool ret = true;

        if (FT_Load_Glyph(_face, FT_Get_Char_Index(_face, ch), FT_LOAD_DEFAULT))
            ret = false;

        return ret;
    }

    void FontFace::setFontSize (GLuint size, GLuint dpi)
    {
        FT_Error err;

        if(! _valid) return;

        err = FT_Set_Char_Size (_face, 0, (FT_F26Dot6)(size * 64), dpi, dpi);
        if (err) {
            cerr << "The current font don't support the size, " << size << " and dpi " << dpi << endl;
        }
    }

    bool FontFace::setCharSize (float size, int dpi)
    {
        FT_Error err;

        if (! _valid) return false;

        // size_t hres = 64;

        /* Set char size */
        // error = FT_Set_Char_Size (_face, (int)(size * 64), 0, 72*hres, 72);

        //For some twisted reason, Freetype measures font size
        //in terms of 1/64ths of pixels.  Thus, to make a font
        //h pixels high, we need to request a size of h*64.
        //(h << 6 is just a prettier way of writting h*64)
        err = FT_Set_Char_Size( _face, ((int)size) << 6, 0, dpi, dpi);
        if (err) {
            cerr << "The current font don't support the size, " << size << " and dpi " << dpi << endl;
            return false;
        }

        return true;
    }

} /* namespace BIL */
