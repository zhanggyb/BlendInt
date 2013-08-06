/*
 * AbstractButton.cpp
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#include <GL/glew.h>
#include <GL/gl.h>
#include <BIL/AbstractButton.h>

namespace BIL {

	AbstractButton::AbstractButton (BasicObject *parent)
			: Controller(parent)
	{
		// TODO Auto-generated constructor stub
		_size = Vec2ui(128, 32);
	}

	AbstractButton::~AbstractButton ()
	{
		// TODO Auto-generated destructor stub
	}

	void AbstractButton::render (void)
	{
		// Demo code, copy from GLFW webpage

		GLint x = _pos.coord.x;
		GLint y = _pos.coord.y;
		GLint w = _size.vec.x;
		GLint h = _size.vec.y;

		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glColor3f(0.45f, 0.45f, 0.45f);

		glBegin(GL_QUADS);
		glRecti(x, y, x + w, y + h);
		glEnd();

		return;
	}

} /* namespace BIL */
