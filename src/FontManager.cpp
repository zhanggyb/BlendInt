/*
 * FontManager.cpp
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#include <iostream>
#include <algorithm>
#include <fstream>

#include <boost/filesystem.hpp>

#include "FontManager.h"

using namespace std;
using namespace boost;
using namespace boost::filesystem;

namespace BIL {

	bool FontManager::initialized = false;

	FontManager::~FontManager ()
	{
		FontIter it;
		for (it = _fonts.begin(); it != _fonts.end(); it++) {
			delete (*it).second;
		}
		_fonts.clear();

		// unload font library
		if (_fontLib != NULL) {
			FT_Done_FreeType(_fontLib);
		}
	}

	bool FontManager::initialize (void)
	{
		if (initialized == true)
			return false;

		FT_Error error = FT_Init_FreeType(&_fontLib);
		if (error) {
			cerr << "Cannot initialize FreeType library" << endl;
			return false;
		}

		// load system files
		loadFontDir("/usr/share/fonts");

		initialized = true;
		return true;
	}

	bool FontManager::loadFont (const string& name)
	{
		// TODO: load from font name
		return true;
	}

	bool FontManager::loadFontFile (const string& file)
	{
		if (file.empty())
			return false;

		// check if we already load this font
		FontIter it = _fonts.find(file);
		if (it != _fonts.end())
			return false;

		// check if the path is valid
		if (!exists(path(file)))
			return false;

		FontData *font = new FontData(_fontLib, file);
		if (font->isValid()) {
			_fonts[file] = font;
		} else {
			delete font; font = NULL;
		}

		return true;
	}

	void FontManager::loadFontDir (const string& path)
	{
		filesystem::path p(path);
		string ext;

		if (!exists(p))
			return;
		if (!is_directory(p))
			return;

		filesystem::directory_iterator end_it;
		filesystem::directory_iterator it(p);	// begin
		while (it != end_it) {
			if (is_directory(it->path())) {
				loadFontDir(it->path().string());
			}
			ext = extension(*it);
			if ((ext == ".ttf") || (ext == ".TTF")) {
				loadFontFile(it->path().string());
			}
			it++;
		}
	}

} /* namespace BIL */
