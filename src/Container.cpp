/*
 * Container.cpp
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#include <GL/glew.h>
#include <GL/gl.h>

#include <BIL/Container.h>

namespace BIL {

	Container::Container (BasicObject *parent)
			: Drawable(parent)
	{
		// TODO Auto-generated constructor stub
		_size = Vec2ui(250, 200);
		setMargin(Vec4i(1, 1, 1, 1));
	}

	Container::~Container ()
	{
		// TODO Auto-generated destructor stub
	}

	void Container::render (void)
	{

	}

} /* namespace BIL */
