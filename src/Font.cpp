/*
 * Font.cpp
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#include <BIL/Font.h>

namespace BIL {

	const std::string Font::DEFAULT_FONT("Arial");

	Font::Font (const std::string& name, FONT_TYPE type)
			: _name(name)
	{
		// TODO Auto-generated constructor stub

	}

	Font::Font (const Font& orig)
	{
		_name = orig.getName();
		_size = orig.getSize();
	}

	Font& Font::operator = (const Font& orig)
	{
		_name = orig.getName();
		_size = orig.getSize();
		return *this;
	}

	Font::~Font ()
	{
		// TODO Auto-generated destructor stub
	}

} /* namespace BIL */
