/*
 * FontType.cpp
 *
 *  Created on: 2013年7月9日
 *      Author: zhanggyb
 */

#include <iostream>

#include <BIL/FontType.h>

using namespace std;

namespace BIL {

    FontType::FontType (const string& filename,
                        float size)
        : _library(NULL), _face(NULL),
          _stroker(NULL), _valid(false),
          _unicode(false), _file(filename)
    {
        // size_t hres = 64;
        FT_Error error;

        /*
          FT_Matrix matrix = { (int)((1.0/hres) * 0x10000L),
          (int)((0.0)      * 0x10000L),
          (int)((0.0)      * 0x10000L),
          (int)((1.0)      * 0x10000L) };
        */

        error = FT_Init_FreeType(&_library);
        if(error) {
            cerr << "Cannot initialize FreeType library" << endl;
            return;
        }

        error = FT_New_Face(_library, filename.c_str(), 0, &_face);
        if (error == FT_Err_Unknown_File_Format) {
            cerr << "Unknown font file format: " << filename << endl;
            return;
        }
        if (error) {
            cerr << "Fail to generate a new Font Face" << endl;
            return;
        }

        error = FT_Stroker_New (_library, &_stroker);

        if (error) {
            cerr << "Fail to load Stroker" << endl;
            return;
        }

        _valid = true;          // now treat it success

        error = FT_Select_Charmap (_face, FT_ENCODING_UNICODE);
        if (error) {
            cerr << "Cannot set the unicode character map: "
                 << filename << endl;
        } else {
            _unicode = true;
        }

        /* Set char size */
        /*
          error = FT_Set_Char_Size (_face, (int)(size * 64), 0, 72*hres, 72);
          if(error) {
          cerr << "Cannot set character size" << endl;
          } else {
          FT_Set_Transform (_face, &matrix, NULL);
          }
        */

    }

    FontType::FontType (const FT_Byte* buffer,
                        FT_Long bufsize,
                        FT_Long index,
                        float size)
        : _library(NULL), _face(NULL),
          _stroker(NULL), _valid(false),
          _unicode(false), _file()
    {
        // size_t hres = 64;
        FT_Error error;
        /*
          FT_Matrix matrix = { (int)((1.0/hres) * 0x10000L),
          (int)((0.0)      * 0x10000L),
          (int)((0.0)      * 0x10000L),
          (int)((1.0)      * 0x10000L) };
        */

        error = FT_Init_FreeType(&_library);
        if(error) {
            cerr << "Cannot initialize FreeType library" << endl;
            return;
        }

        error = FT_New_Memory_Face(_library, buffer, bufsize, index, &_face);
        if (error) {
            cerr << "Fail to generate a new Font Face from memory" << endl;
            return;
        }

        error = FT_Stroker_New (_library, &_stroker);

        if (error) {
            cerr << "Fail to load Stroker" << endl;
            return;
        }

        _valid = true;          // now treat it success

        error = FT_Select_Charmap (_face, FT_ENCODING_UNICODE);
        if (error) {
            cerr << "Cannot set the unicode character map" << endl;
        } else {
            _unicode = true;
        }

        /* Set char size */
        /*
          error = FT_Set_Char_Size (_face, (int)(size * 64), 0, 72*hres, 72);
          if(error) {
          cerr << "Cannot set character size" << endl;
          } else {
          FT_Set_Transform (_face, &matrix, NULL);
          }
        */
    }

    FontType::~FontType ()
    {
        if(_stroker != NULL) {
            FT_Stroker_Done(_stroker);
            _stroker = NULL;
        }

        if (_face != NULL) {
            FT_Done_Face(_face);
            _face = NULL;
        }

        if (_library != NULL) {
            FT_Done_FreeType(_library);
            _library = NULL;
        }
    }

    bool FontType::loadGlyph (char charcode)
    {
        bool ret = true;

        if (FT_Load_Glyph(_face,
                          FT_Get_Char_Index(_face, charcode),
                          FT_LOAD_DEFAULT))
            ret = false;

        return ret;
    }

    bool FontType::loadGlyph (FT_UInt glyph_index, FT_Int32 load_flags)
    {
        if(! _valid) return false;

        FT_Error error;

        error = FT_Load_Glyph(_face, glyph_index, load_flags);

        if(error) {
            cerr << "Fail to load glyph" << endl;
            return false;
        }

        return true;
    }

    bool FontType::setFontSize (GLuint size, GLuint dpi)
    {
        if(! _valid) return false;

        FT_Error error;

        error = FT_Set_Char_Size (_face, 0, (FT_F26Dot6)(size * 64), dpi, dpi);
        if (error) {
            cerr << "The current font don't support the size, " << size << " and dpi " << dpi << endl;
            return false;
        }

        return true;
    }

    bool FontType::setCharSize (float size, int dpi)
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

    bool FontType::setCharSize (FT_F26Dot6 width,
                                FT_F26Dot6 height,
                                FT_UInt horz_res,
                                FT_UInt vert_res)
    {
        if(! _valid) return false;

        FT_Error error;

        error = FT_Set_Char_Size(_face, width, height, horz_res, vert_res);

        if(error) {
            cerr << "Fail to set character size" << endl;
            return false;
        }

        return true;
    }

    FT_UInt FontType::getCharIndex (const FT_ULong charcode)
    {
        if(!_valid) return 0;

        return (FT_Get_Char_Index (_face, charcode));
    }

    bool FontType::setLcdFilter(FT_LcdFilter filter)
    {
        if(!_valid) return false;

        FT_Error error;

        error = FT_Library_SetLcdFilter (_library, filter);

        if (error) {
            cerr << "Fail to set lcd filter" << endl;
            return false;
        }

        return true;
    }

    bool FontType::setLcdFilterWeights(unsigned char* weights)
    {
        if(!_valid) return false;

        FT_Error error;

        error = FT_Library_SetLcdFilterWeights(_library, weights);
        if (error) {
            cerr << "Fail to set lcd filter weights" << endl;
            return false;
        }

        return true;
    }

    bool FontType::loadChar (FT_ULong charcode, FT_Int32 load_flags)
    {
        if (!_valid) return false;

        FT_Error error;

        error = FT_Load_Char (_face, charcode, load_flags);
        if(error) {
            cerr << "Fail to load Character" << endl;
            return false;
        }

        return true;
    }

    bool FontType::renderGlyph (FT_Render_Mode render_mode)
    {
        if(!_valid) return false;
        FT_Error error;

        error = FT_Render_Glyph(_face->glyph, // glyph slot
                                render_mode);
        if(error) {
            cerr << "Fail to render glyph" << endl;
            return false;
        }

        return true;
    }

    bool FontType::getKerning (FT_UInt left_glyph,
                               FT_UInt right_glyph,
                               FT_UInt kern_mode,
                               FT_Vector* akerning)
    {
        if((!_valid) || (_face == NULL)) return false;

        FT_Error error;
        error = FT_Get_Kerning (_face,
                                left_glyph,
                                right_glyph,
                                kern_mode,
                                akerning);
        if(error) {
            cerr << "Fail to get kerning" << endl;
            return false;
        }

        return true;
    }

} /* namespace BIL */
