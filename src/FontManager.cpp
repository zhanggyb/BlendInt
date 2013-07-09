/*
 * FontManager.cpp
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#include <iostream>
#include <algorithm>

#include <boost/filesystem.hpp>

#include "FontManager.h"

using namespace std;
using namespace boost;
using namespace boost::filesystem;

namespace BIL {

	bool FontManager::initialized = false;

	bool FontManager::initialize (void)
	{
		if (initialized == true)
			return false;

		FT_Error error = FT_Init_FreeType(&_fontLib);
		if (error) {
			cerr << "Cannot initialize FreeType library" << endl;
			return false;
		}

		// load system files	TODO: load more fonts
		loadFontDir("/usr/share/fonts");

		initialized = true;
		return true;
	}

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

		// clear the namedb but no need to delete the objects again
		_namedb.clear();
	}

#ifdef DEBUG
	void FontManager::printfonts (void)
	{
		map<string, FontFace*>::const_iterator it;

		for (it = _namedb.begin(); it != _namedb.end(); it++)
		{
			cout << (*it).first << endl;
		}
	}
#endif

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

	FontFace *font = new FontFace(_fontLib, file);
	if (font->isValid()) {
		_fonts[file] = font;
		string psname = font->getPostscriptName();
		if (!psname.empty()) {
			_namedb[psname] = font;
		}
	} else {
		delete font;
		font = NULL;
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
		if ((ext == ".ttf")
			|| (ext == ".TTF")
			|| (ext == ".otf")
			|| (ext == ".OTF"))
		{
			loadFontFile(it->path().string());
		}
		it++;
	}
}

} /* namespace BIL */
