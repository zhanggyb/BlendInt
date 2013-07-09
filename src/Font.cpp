/*
 * Font.cpp
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#include <BIL/Font.h>

namespace BIL {

	Font::Font()
	: _bold (false), _italic (false)
	{

	}

	Font::Font (const string& family)
	: _bold (false), _italic (false), _family (family)
	{

	}

	Font::~Font(void)
	{

	}

}
