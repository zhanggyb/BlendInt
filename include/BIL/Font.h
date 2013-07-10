/*
 * Font.h
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#ifndef _BIL_FONT_H_
#define _BIL_FONT_H_

#include <string>

using namespace std;

namespace BIL {

	class Font
	{
	public:
		Font ();

		Font (const string& family, float size = 10, bool bold = false, bool italic = false);

		virtual ~Font ();

	private:

		bool _bold;
		bool _italic;

		float _size;	/** Font Size */

		string _family;	/** the font family, e.g. "Droid Sans" */

	};

} /* namespace BIL */
#endif /* FONT_H_ */
