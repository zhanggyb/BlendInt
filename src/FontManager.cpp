/*
 * FontManager.cpp
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#include <iostream>
#include <algorithm>
#include <fstream>

#include "FontManager.h"

namespace BIL {

	FontManager::~FontManager ()
	{
		// TODO Auto-generated destructor stub
		/*
		FontIter it;
		for (it = _fonts.begin(); it != _fonts.end(); it++) {
			delete (*it).second;
		}
	*/
		_fonts.clear();
	}

	bool FontManager::initialize (void)
	{
		// TODO: load more fonts

		return (loadFont(string("/usr/share/fonts/TTF/DroidSans.ttf")));
	}

	bool FontManager::loadFontFile (const string& path)
	{
		using namespace std;

		if (path.empty())
			return false;

		// check if we already load this font
		FontIter it = _fonts.find(path);
		if (it != _fonts.end())
			return false;

		// check if the path is valid
		if (! fileExist (path)) return false;

		// TODO: store the font name instead of the full path of the font file
		// _fonts[path] = font;

		return true;
	}

	inline bool FontManager::fileExist (const string& file)
	{
		ifstream f(file.data());

		return (f.is_open());
	}

} /* namespace BIL */
