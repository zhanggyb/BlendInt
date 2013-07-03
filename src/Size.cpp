/*
 * Size.cpp
 *
 *  Created on: 2013年7月3日
 *      Author: zhanggyb
 */

#include "Size.h"

namespace BIL {

	Size& Size::operator = (const Size& orig)
	{
		this->_width = orig.getWidth();
		this->_height = orig.getHeight();

		return *this;
	}

	Size::~Size ()
	{
		// TODO Auto-generated destructor stub
	}

} /* namespace BIL */
