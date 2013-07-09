/*
 * Font.h
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#ifndef _BIL_FONT_H_
#define _BIL_FONT_H_

#include <FTGL/ftgl.h>
#include <string>
#include <vector>
#include <GL/gl.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <BIL/Color.h>
#include <BIL/Point.h>
#include <BIL/Rect.h>

using namespace std;

namespace BIL {

	class FontData
	{
	public:

		FontData (FT_Library& lib, const string& fontfile);

		virtual ~FontData ();

		bool isValid (void) const {return _valid;}

	private:

		std::string _file; /** font file */

		FT_Face _face;	/** freetype2 face */

		bool _valid;	/** if the font face is valid */

		static const std::string DEFAULT_FONT;
	};

	class Font
	{
	public:
		Font () {}
	};

} /* namespace BIL */
#endif /* FONT_H_ */
