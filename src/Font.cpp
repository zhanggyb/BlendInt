/*
 * Font.cpp
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#include <string.h>
#include <BIL/Font.h>

namespace BIL {

	Font::Font (const string& family, unsigned int size, bool bold, bool italic)
			: family(family), size(size), bold(bold), italic(italic)
	{
	}

	Font::Font (const Font& orig)
	{
		family = orig.family;
		size = orig.size;
		bold = orig.bold;
		italic = orig.italic;
	}

	Font& Font::operator = (const Font& orig)
	{
		family = orig.family;
		size = orig.size;
		bold = orig.bold;
		italic = orig.italic;

		return *this;
	}

	bool operator < (const Font& src, const Font& dist)
	{
		if(src.family < dist.family) {
			return true;
		} else if(src.family > dist.family) {
			return false;
		}

		if(src.size < dist.size) {
			return true;
		} else if(src.size > dist.size) {
			return false;
		}

		if(src.bold < dist.bold) {
			return true;
		} else if(src.bold > dist.bold) {
			return false;
		}

		if(src.italic < dist.italic) {
			return true;
		} else if (src.italic > dist.italic) {
			return false;
		}

		return false;
		/*
		return (src.family < dist.family ?
				true : (src.size < dist.size ?
		        true : (src.bold < dist.bold ?
		                true : (src.italic < dist.italic ? true : false))));
		 */
	}

	bool operator == (const Font& src, const Font& dist)
	{
		if (strcmp(src.family.c_str(), dist.family.c_str()) == 0) {
			return true;
		}

		return (src.family == dist.family &&
				src.size == dist.size &&
				src.bold == dist.bold &&
				src.italic == dist.italic);
	}

}

