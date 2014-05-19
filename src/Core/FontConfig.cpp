/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <fstream>
#include <algorithm>

#include <boost/filesystem.hpp>

#include <BlendInt/Core/FontConfig.hpp>

using namespace std;
using namespace boost;
using namespace boost::filesystem;

namespace BlendInt {

	FontConfig* FontConfig::service = 0;

	FontConfig* FontConfig::instance ()
	{
		if (!service) {
			cerr << "The fontconfig service is not initialized successfully! Exit"
			        << endl;
			exit(EXIT_FAILURE);
		}

		return service;
	}

	bool FontConfig::initialize ()
	{
		// load system files    TODO: load more fonts
		// loadFontDir("/usr/share/fonts");
		bool result = true;
		if (!FcInit()) {
			std::cerr << "Fail to initialize fontconfig" << std::endl;
			result = false;
		}

		if(!service) {
			service = new FontConfig;
		}

		if(!service) {
			result = false;
		} else {
//			if(!service->loadDefaultFontToMem()) {
//				std::cerr << "Fail to load default font file to memory" << std::endl;
//				result = false;
//			}
		}

		return result;
	}

	void FontConfig::release()
	{
		if(service) {
			delete service;
			service = 0;
		}

		FcFini();
	}

	FontConfig::~FontConfig ()
	{
		if (_defaultFontBuf) {
			delete[] _defaultFontBuf;
			_defaultFontBuf = NULL;
			_defaultFontBufSize = 0;
		}
	}

	bool FontConfig::loadDefaultFontToMem (const string& family)
	{
		string filepath = getFontPath(family);

		if (filepath.empty()) {
			return false;
		}

		ifstream file(filepath.c_str(), ios::binary);
		if (file) {
			file.seekg(0, ios::end);
			fstream::pos_type filesize = file.tellg();
			file.seekg(0);

			if (_defaultFontBuf) {
				delete[] _defaultFontBuf;
				_defaultFontBuf = NULL;
			}

			_defaultFontBufSize = filesize;
			_defaultFontBuf = new unsigned char[_defaultFontBufSize];
			file.read((char*) _defaultFontBuf, _defaultFontBufSize);

			return true;
		}

		return false;
	}

	string FontConfig::getFontPath (const string& family, float size, bool bold,
	        bool italic)
	{
		string file;

		int weight = bold ? FC_WEIGHT_BOLD : FC_WEIGHT_REGULAR;
		int slant = italic ? FC_SLANT_ITALIC : FC_SLANT_ROMAN;

		FcPattern *pattern = FcPatternCreate();
		FcPatternAddDouble(pattern, FC_SIZE, size);
		FcPatternAddInteger(pattern, FC_WEIGHT, weight);
		FcPatternAddInteger(pattern, FC_SLANT, slant);
		FcPatternAddString(pattern, FC_FAMILY, (FcChar8*) family.c_str());
		FcConfigSubstitute(0, pattern, FcMatchPattern);
		FcDefaultSubstitute(pattern);
		FcResult result;
		FcPattern *match = FcFontMatch(0, pattern, &result);
		FcPatternDestroy(pattern);

		if (match) {
			FcValue value;
			FcResult result = FcPatternGet(match, FC_FILE, 0, &value);
			if (result) {
				// print erro
				fprintf(stderr, "ERROR: Fail to match font file");
			} else {
				file = (char*) (value.u.s);
			}
		} else {
			// TODO: if no file found, return the default font file
			fprintf(stderr, "ERROR: font %s is not found\n", family.c_str());
		}

		FcPatternDestroy(match);

		return file;
	}

	bool FontConfig::loadFontFile (const string& file)
	{
		if (file.empty())
			return false;

		// check if the path is valid
		if (!exists(path(file)))
			return false;

		/*
		 FontType *font = new FontType(_fontLib, file);
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
		 */

		return true;
	}

	void FontConfig::loadFontDir (const string& path)
	{
		filesystem::path p(path);
		string ext;

		if (!exists(p))
			return;
		if (!is_directory(p))
			return;

		filesystem::directory_iterator end_it;
		filesystem::directory_iterator it(p);   // begin
		while (it != end_it) {
			if (is_directory(it->path())) {
				loadFontDir(it->path().string());
			}
			ext = extension(*it);
			if ((ext == ".ttf") || (ext == ".TTF") || (ext == ".otf")
			        || (ext == ".OTF")) {
				loadFontFile(it->path().string());
			}
			it++;
		}
	}

} /* namespace BlendInt */
