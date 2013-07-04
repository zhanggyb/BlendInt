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

using namespace std;

typedef map<string, FTFont*> FontList;
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

	private:
		FontManager ()
		{
		}

		FontManager (const FontManager &orig);
		FontManager& operator = (const FontManager &orig);

		bool loadFont (const string &path);

		// container for fonts
		FontList _fonts;
	};

} /* namespace BIL */
#endif /* FONTMANAGER_H_ */
