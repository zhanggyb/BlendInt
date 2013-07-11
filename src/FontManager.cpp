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

	FontManager* FontManager::gFontManager = NULL;

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
		// loadFontDir("/usr/share/fonts");

		if (! FcInit ()) return false;

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

		// finish fontconfig
		if (initialized) FcFini();
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

bool FontManager::loadFont (const string& family)
{
	string ret = getFontPath (family);

	cout << "get font: " << ret << endl;

	if (ret.empty()) {
		return false;
	}

	// TODO: do sth

	return true;
}

string FontManager::getFontPath (const string& family, float size, bool bold, bool italic)
{
	string file;

	int weight = bold ? FC_WEIGHT_BOLD : FC_WEIGHT_REGULAR;
	int slant = italic ? FC_SLANT_ITALIC : FC_SLANT_ROMAN;

	FcPattern *pattern = FcPatternCreate ();
	FcPatternAddDouble (pattern, FC_SIZE, size);
	FcPatternAddInteger (pattern, FC_WEIGHT, weight);
	FcPatternAddInteger (pattern, FC_SLANT, slant);
	FcPatternAddString (pattern, FC_FAMILY, (FcChar8*) family.c_str());
	FcConfigSubstitute (0, pattern, FcMatchPattern);
	FcDefaultSubstitute (pattern);
	FcResult result;
	FcPattern *match = FcFontMatch (0, pattern, &result);
	FcPatternDestroy (pattern);

	if (!match) {
		// TODO: return default font
		return getFontPath (Font("Sans"));
	} else {
		FcValue value;
		FcResult result = FcPatternGet (match, FC_FILE, 0, &value);
		if (result) {
			// print error
		} else {
			file = (char*)(value.u.s);
		}
	}

	FcPatternDestroy (match);

	return file;
}

string FontManager::getFontPath (const Font& font)
{
	string file;

	int weight = font.bold() ? FC_WEIGHT_BOLD : FC_WEIGHT_REGULAR;
	int slant = font.italic() ? FC_SLANT_ITALIC : FC_SLANT_ROMAN;

	FcPattern *pattern = FcPatternCreate ();
	FcPatternAddDouble (pattern, FC_SIZE, font.size());
	FcPatternAddInteger (pattern, FC_WEIGHT, weight);
	FcPatternAddInteger (pattern, FC_SLANT, slant);
	FcPatternAddString (pattern, FC_FAMILY, (FcChar8*) font.family().c_str());
	FcConfigSubstitute (0, pattern, FcMatchPattern);
	FcDefaultSubstitute (pattern);
	FcResult result;
	FcPattern *match = FcFontMatch (0, pattern, &result);
	FcPatternDestroy (pattern);

	if (!match) {
		// TODO: return default font
	} else {
		FcValue value;
		FcResult result = FcPatternGet (match, FC_FILE, 0, &value);
		if (result) {
			// print error
		} else {
			file = (char*)(value.u.s);
		}
	}

	FcPatternDestroy (match);

	return file;
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

