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

	bool FontManager::loadFont (const string& name)
	{
		// TODO: load from font name
		return true;
	}

	bool FontManager::loadFontFile (const string& file)
	{
		cout << "Load font file: " << file << endl;

		using namespace std;

		if (file.empty())
			return false;

		// check if we already load this font
		FontIter it = _fonts.find(file);
		if (it != _fonts.end())
			return false;

		// check if the path is valid
		if (!fileExist(file))
			return false;

		// TODO: store the font name instead of the full path of the font file
		// _fonts[path] = font;

		return true;
	}

	void FontManager::loadFontDir (const string& path)
	{
		filesystem::path p(path);

		if (!exists(p))
			return;
		if (!is_directory(p))
			return;

		filesystem::directory_iterator end_it;
		filesystem::directory_iterator it(p);	// begin
		while (it != end_it) {
			string ext = extension(*it);
			if ((ext == ".ttf") || (ext == ".TTF")) {
				loadFontFile(it->path().string());
			}
			it++;
		}
	}

	inline bool FontManager::fileExist (const string& file)
	{
		ifstream f(file.data());

		return (f.is_open());
	}

} /* namespace BIL */
