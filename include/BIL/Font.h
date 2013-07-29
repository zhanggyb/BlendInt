/*
 * Font.h
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#ifndef _BIL_FONT_H_
#define _BIL_FONT_H_

#include <string>

#include <BIL/Tuple.h>

using namespace std;

namespace BIL {

	struct Font;

	// friend function
	extern bool operator < (const Font& src, const Font& dist);
	extern bool operator == (const Font& src, const Font& dist);

	struct Font
	{
		Font (const string& family = string("Sans"), unsigned int size = 10,
		        bool bold = false, bool italic = false);

		Font (const Font& orig);

		Font& operator = (const Font& orig);

		/** the font family, e.g. "Droid Sans" */
		string family;

		/** font size */
		unsigned int size;

		/** whether text is bold */
		bool bold;

		/** whether text is italic */
		bool italic;
	};

} /* namespace BIL */
#endif /* FONT_H_ */
