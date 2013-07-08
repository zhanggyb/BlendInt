/*
 * Controller.cpp
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#include <GL/gl.h>

#include <BIL/Controller.h>
#include <BIL/Rect.h>

namespace BIL {

	Controller::Controller (BasicObject *parent)
			: Drawable(parent)
	{
		// TODO Auto-generated constructor stub

		// Set the default padding and margin
		setPadding(1, 1, 1, 1);
		setMargin(1, 1, 1, 1);
	}

	Controller::~Controller ()
	{
		// TODO Auto-generated destructor stub
	}

	void Controller::render (void)
	{

	}

} /* namespace BIL */
