/*
 * FontManager.cpp
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#include <iostream>

#include "FontManager.h"

namespace BIL {

	FontManager::~FontManager ()
	{
		// TODO Auto-generated destructor stub
		FontIter it;
		for (it = _fonts.begin(); it != _fonts.end(); it++) {
			delete (*it).second;
		}

		_fonts.clear();
	}

	bool FontManager::initialize (void)
	{
		// TODO: load more fonts

		return (loadFont(string("/usr/share/fonts/TTF/DroidSans.ttf")));
	}

	FTFont* FontManager::getFont (const string& fontname)
	{
		using namespace std;

		FontIter result = _fonts.find(fontname);
		if (result != _fonts.end()) {
			return result->second;
		} else {
			cerr << "Font not found" << endl;
			return NULL;
		}
	}

	bool FontManager::loadFont (const string& path)
	{
		FTTextureFont *font = new FTTextureFont(path.data());

		if (font->Error())
			return false;

		// TODO: store the font name instead of the full path of the font file
		_fonts[path] = font;

		return true;
	}

} /* namespace BIL */
