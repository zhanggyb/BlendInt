/*
 * Size.cpp
 *
 *  Created on: 2013年7月3日
 *      Author: zhanggyb
 */

#include "Size.h"

namespace BIL {

	Size2i& Size2i::operator = (const Size2i& orig)
	{
		this->_width = orig.getWidth();
		this->_height = orig.getHeight();

		return *this;
	}

	Size2i::~Size2i ()
	{
		// TODO Auto-generated destructor stub
	}

} /* namespace BIL */
