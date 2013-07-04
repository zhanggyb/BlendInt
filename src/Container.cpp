/*
 * Container.cpp
 *
 *  Created on: 2013年7月4日
 *      Author: zhanggyb
 */

#include <GL/gl.h>

#include <BIL/Container.h>

namespace BIL {

	Container::Container (BasicObject *parent)
			: Drawable(parent)
	{
		// TODO Auto-generated constructor stub
		_size = Size2Dui(250, 200);
		_margin = Rect2Di(1, 1, 1, 1);
	}

	Container::~Container ()
	{
		// TODO Auto-generated destructor stub
	}

	void Container::render (void)
	{

		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glColor3ub(0xf0, 0xf0, 0xf0);
		glLineWidth(1.0f);

		GLint v[2];

		glBegin(GL_LINE_STRIP);
		v[0] = _pos.getX() + _margin.getLeft();
		v[1] = _pos.getY() + _margin.getBottom();
		glVertex2iv(v);
		v[0] = _pos.getX() + _size.getWidth() - _margin.getRight();
		v[1] = _pos.getY() + _margin.getBottom();
		glVertex2iv(v);
		v[0] = _pos.getX() + _size.getWidth() - _margin.getRight();
		v[1] = _pos.getY() + _size.getHeight() - _margin.getTop();
		glVertex2iv(v);
		v[0] = _pos.getX() + _margin.getLeft();
		v[1] = _pos.getY() + _size.getHeight() - _margin.getTop();
		glVertex2iv(v);
		v[0] = _pos.getX() + _margin.getLeft();
		v[1] = _pos.getY() + _margin.getBottom();
		glVertex2iv(v);
		glEnd();
		glFlush();
	}

} /* namespace BIL */
