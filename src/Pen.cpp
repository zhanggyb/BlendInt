/*
 * Pen.cpp
 *
 *  Created on: 2013年7月8日
 *      Author: zhanggyb
 */

// Load glew first
#include <GL/glew.h>
#include <GL/gl.h>

#include <BIL/Pen.h>

namespace BIL {

	Pen::Pen (float size, float width)
		: _size (size), _width (size)
	{
		// TODO Auto-generated constructor stub
	}

	Pen::~Pen ()
	{
		// TODO Auto-generated destructor stub
	}

	void Pen::setPointSize (GLfloat size)
	{
		_size = size;
	}

	void Pen::setLineWidth (GLfloat width)
	{
		_width = width;
	}

	void Pen::setLineStyle (GLfloat style)
	{
	}

} /* namespace BIL */
