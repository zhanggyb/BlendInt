/*
 * FontFace.h
 *
 *  Created on: 2013年7月9日
 *      Author: zhanggyb
 */

#ifndef _BIL_FONTFACE_H_
#define _BIL_FONTFACE_H_

#include <string>
#include <vector>
#include <map>
#include <GL/gl.h>

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;

namespace BIL {

	class FontFace
	{
	public:

		FontFace (FT_Library& lib, const string& fontfile);

		virtual ~FontFace ();

		bool isValid (void) const
		{
			return _valid;
		}

		const FT_Face& getFontFace (void) const {return _face;}

		const string& getPostscriptName (void) const
		{
			return _psName;
		}

		bool loadGlyph (char ch);

		void setFontSize (GLuint size, GLuint dpi);

	private:

		string _file; /** font file */

		FT_Face _face; /** freetype2 face */

		string _psName; /** Postscript Name */

		bool _valid; /** if the font face is valid */

		bool _unicode;	/** if has unicode charmap */

		static map<string, FT_Face> fontdb;
	};

} /* namespace BIL */
#endif /* _BIL_FONTFACE_H_ */
