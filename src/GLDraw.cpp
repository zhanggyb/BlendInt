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

	GLDraw::GLDraw (Drawable *obj)
			: _drawableArea(obj)
	{
		// TODO Auto-generated constructor stub

	}

	GLDraw::~GLDraw ()
	{
		// TODO Auto-generated destructor stub
	}

	void GLDraw::setPointSize (GLfloat size)
	{
		glPointSize(size);
	}

	void GLDraw::setLineWidth (GLfloat width)
	{
		glLineWidth(width);
	}

	void GLDraw::setLineStyle (GLfloat style)
	{
	}

	void GLDraw::setColor (const Color& color)
	{
		glColor4f(color.red(), color.green(), color.blue(), color.alpha());
	}

	void GLDraw::line (const Point2Di& from, const Point2Di& to)
	{
	}

	void GLDraw::line (const Point3Di& from, const Point3Di& to)
	{
		glBegin(GL_LINES);
		glVertex3i(from.getX(), from.getY(), from.getZ());
		glVertex3i(to.getX(), to.getY(), to.getZ());
		glEnd();
	}

	void GLDraw::line (const Point3Df& from, const Point3Df& to)
	{
		glBegin(GL_LINES);
		glVertex3f(from.getX(), from.getY(), from.getZ());
		glVertex3f(to.getX(), to.getY(), to.getZ());
		glEnd();
	}

	void GLDraw::rect (GLint x, GLint y, GLint w, GLint h)
	{
		glBegin(GL_QUADS);
		glVertex2i(x, y);
		glVertex2i(x + w, y);
		glVertex2i(x + w, y + h);
		glVertex2i(x, y + h);
		glEnd();
	}

	void GLDraw::rect (GLfloat x, GLfloat y, GLfloat w, GLfloat h)
	{
		glBegin(GL_QUADS);
		glVertex2f(x, y);
		glVertex2f(x + w, y);
		glVertex2f(x + w, y + h);
		glVertex2f(x, y + h);
		glEnd();
	}

	void GLDraw::rect (const Rect2Di& rect)
	{
		glBegin(GL_QUADS);
		glVertex2i(rect.getX(), rect.getY());
		glVertex2i(rect.getX() + rect.getWidth(), rect.getY());
		glVertex2i(rect.getX() + rect.getWidth(),
		        rect.getY() + rect.getHeight());
		glVertex2i(rect.getX(), rect.getY() + rect.getHeight());
		glEnd();
	}

	void GLDraw::text (const std::string& text, Point3Df position)
	{
	}

} /* namespace BIL */

