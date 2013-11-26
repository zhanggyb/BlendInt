/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>

#include <BlendInt/Viewport3D.hpp>
#include <BlendInt/Interface.hpp>

namespace BlendInt {

	Viewport3D::Viewport3D()
	: Widget()
	{

	}

	Viewport3D::Viewport3D(AbstractWidget* parent)
	: Widget(parent)
	{

	}

	Viewport3D::~Viewport3D ()
	{

	}

	void Viewport3D::Render ()
	{
		// store the current matrices
		GLdouble proj_matrix[16];
		GLdouble model_matrix[16];

		glGetDoublev(GL_MODELVIEW_MATRIX, model_matrix);
		glGetDoublev(GL_PROJECTION_MATRIX, proj_matrix);

		glEnable (GL_SCISSOR_TEST);
		glScissor (position().x(),
				position().y(),
				size().width(),
				size().height());

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.f, (float) size().width(), 0.f, (float) size().height(), 100.f, -100.f);
		//gluPerspective(45.0f,(GLfloat)size().width()/(GLfloat)size().height(),0.1f,100.0f);

		glViewport(position().x(), position().y(), size().width(), size().height());

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//-------------------------------------------------------------------------------------------------------

		//test code
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(1.0, 1.0, 1.0, 0.75);
		glTranslatef(200.f, 150.f, 0.0f);
		glRotatef((float) glfwGetTime() * 60.f, 0.f, 0.f, 1.f);
		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-200.f, 0.0f, 0.f);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(200.f, 0.0f, 0.f);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(000.f, 300.f, 0.f);
		glEnd();

		//-------------------------------------------------------------------------------------------------------

		glDisable(GL_SCISSOR_TEST);

		// set back the previous matrices and viewport
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixd(proj_matrix);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(model_matrix);

		glViewport(0, 0, Interface::instance()->size().width(), Interface::instance()->size().height());

#ifdef DEBUG
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(position().x(), position().y(), z());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glColor4f(1.0f, 1.0f, 1.0f, 0.75f);
		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
		glVertex2i(0, 0);
		glVertex2i(size().width(), 0);
		glVertex2i(size().width(), size().height());
		glVertex2i(0, size().height());
		glEnd();

		glDisable(GL_LINE_STIPPLE);

		glDisable(GL_BLEND);

		glPopMatrix();
#endif
	}

}
