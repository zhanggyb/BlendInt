/*
 * Font.cpp
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

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
		return (src.family < dist.family ?
		        true :
		        (src.size < dist.size ?
		                true :
		                (src.bold < dist.bold ?
		                        true : (src.italic < dist.italic ? true : false))));
	}

	bool operator == (const Font& src, const Font& dist)
	{
		return (src.family == dist.family &&
		        src.size == dist.size &&
		                src.bold == dist.bold &&
		                src.italic == dist.italic);
	}


}

