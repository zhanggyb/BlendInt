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

		Font (const string& family);

		virtual ~Font ();

	private:

		bool _bold;
		bool _italic;

		string _family;

	};

} /* namespace BIL */
#endif /* FONT_H_ */
