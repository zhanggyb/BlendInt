/*
 * Font.cpp
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#include <iostream>

#include <BIL/Font.h>

using namespace std;

namespace BIL {

	const std::string FontData::DEFAULT_FONT("Arial");

	FontData::FontData (FT_Library& lib, const string& fontfile)
			: _file(fontfile), _face(NULL), _valid(false)
	{
		// TODO Auto-generated constructor stub

		FT_Error error = FT_New_Face(lib, fontfile.c_str(), 0, &_face);
		if (error == FT_Err_Unknown_File_Format) {
			cerr << "Unknown font file format: " << fontfile << endl;
		}

		if (!error) _valid = true;

		cout << "Font: " << fontfile << " loaded" << endl;
	}

	FontData::~FontData ()
	{
		// TODO Auto-generated destructor stub
		if (_face != NULL) {
			FT_Done_Face(_face);
		}

		cout << "Font: " << _file << " unloaded" << endl;
	}

} /* namespace BIL */
