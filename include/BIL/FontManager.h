/*
 * FontManager.h
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#ifndef _BIL_FONTMANAGER_H_
#define _BIL_FONTMANAGER_H_

#include <map>
#include <string>
#include <FTGL/ftgl.h>
#include <FTGL/FTGLTextureFont.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <BIL/Font.h>

using namespace std;

typedef map<string, BIL::FontData*> FontList;
typedef FontList::const_iterator FontIter;

namespace BIL {

	class FontManager
	{
	public:
		virtual ~FontManager ();

		bool initialize (void);

		FTFont* getFont (const string &fontname);

		static FontManager& instance (void)
		{
			static FontManager tm;
			return tm;
		}

		/**
		 * @brief load truetype font
		 * @param path font path
		 * @return
		 */
		bool loadFont (const string& name);

	private:

		/**
		 * @brief Default constructor
		 *
		 * Move the default constructor to private
		 */
		FontManager ()
				: _fontLib(NULL)
		{
		}

		bool loadFontFile (const string& file);

		/**
		 * @brief load all Truetype fonts in the directory
		 * @param path the target directory
		 *
		 * load all Truetype fonts (.ttf or .TTF) in the target directory
		 */
		void loadFontDir (const string& path);

		FT_Library _fontLib;

		FontManager& operator = (const FontManager &orig);

		// container for fonts
		FontList _fonts;

		static bool initialized;
	};

} /* namespace BIL */
#endif /* FONTMANAGER_H_ */
