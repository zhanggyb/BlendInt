/*
 * GLDraw.cpp
 *
 *  Created on: 2013年7月8日
 *      Author: zhanggyb
 */

// Load glew first
#include <GL/glew.h>
#include <GL/gl.h>

#include <BIL/GLDraw.h>

namespace BIL {

	GLDraw::GLDraw (Drawable *area)
	: _area(area)
	{
		// TODO Auto-generated constructor stub

	}

	GLDraw::~GLDraw ()
	{
		// TODO Auto-generated destructor stub
	}

} /* namespace BIL */
