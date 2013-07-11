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

	FontFace::FontFace (FT_Library& lib, const string& fontfile)
			: _file(fontfile), _face(NULL), _valid(false), _unicode(false)
	{
		FT_Error error = FT_New_Face(lib, fontfile.c_str(), 0, &_face);
		if (error == FT_Err_Unknown_File_Format) {
			cerr << "Unknown font file format: " << fontfile << endl;
		}

		if (!error) {
			_valid = true;

			const char *name = FT_Get_Postscript_Name(_face);
			if (name != NULL) {
				_psName = name;
			}
		}

		error = FT_Select_Charmap (_face, FT_ENCODING_UNICODE);
		if (error) {
			cerr << "Cannot set the unicode character map: " << fontfile << endl;
		} else {
			_unicode = true;
		}
	}

	FontFace::~FontFace ()
	{
		if (_face != NULL) {
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
			return;
		}
	}

} /* namespace BIL */
