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
		Camera* default_camera = new Camera;
		m_cameras.push_back(default_camera);
	}

	Viewport3D::Viewport3D(AbstractWidget* parent)
	: Widget(parent)
	{
		Camera* default_camera = new Camera;
		m_cameras.push_back(default_camera);
	}

	Viewport3D::~Viewport3D ()
	{
		vector<Camera*>::iterator it;

		for(it = m_cameras.begin(); it != m_cameras.end(); it++)
		{
			delete *it;
		}

		m_cameras.clear();
	}

	void Viewport3D::KeyPressEvent (KeyEvent* event)
	{
		if(event->action() == KeyPress) {
			float x = m_cameras[0]->position().x();
			float y = m_cameras[0]->position().y();
			float z = m_cameras[0]->position().z();

			switch (event->key()) {
				case Key_Left:
					m_cameras[0]->set_position(x - 5, y, z);
					break;

				case Key_Right:
					m_cameras[0]->set_position(x + 5, y, z);
					break;

				default:
					break;
			}
		}
	}

	void Viewport3D::MousePressEvent (MouseEvent* event)
	{
	}

	void Viewport3D::MouseReleaseEvent (MouseEvent* event)
	{
	}

	void Viewport3D::MouseMoveEvent (MouseEvent* event)
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
		//glOrtho(0.f, (float) size().width(), 0.f, (float) size().height(), 100.f, -100.f);
		//glOrtho(0.f, 8.0f, 0.f, 6.0f, 100.f, -100.f);

		gluPerspective(45.0f,(GLfloat)size().width()/(GLfloat)size().height(),0.1f,100.0f);
		//gluLookAt (5.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		gluLookAt (m_cameras[0]->position().x(),
				m_cameras[0]->position().y(),
				m_cameras[0]->position().z(),
				m_cameras[0]->target().x(),
				m_cameras[0]->target().y(),
				m_cameras[0]->target().z(),
				m_cameras[0]->direction().x(),
				m_cameras[0]->direction().y(),
				m_cameras[0]->direction().z()
		);

		glViewport(position().x(), position().y(), size().width(), size().height());

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//-------------------------------------------------------------------------------------------------------

		//test code
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(1.0, 1.0, 1.0, 0.75);
		//glTranslatef(200.f, 150.f, 0.0f);
		//glRotatef((float) glfwGetTime() * 60.f, 0.f, 0.f, 1.f);
//		glBegin(GL_POLYGON);
//		glColor3f(1.f, 0.f, 0.f);
//		glVertex3f(0.f, 0.0f, 0.f);
//		glColor3f(0.f, 1.f, 0.f);
//		glVertex3f(200.f, 0.0f, 0.f);
//		glColor3f(0.f, 0.f, 1.f);
//		glVertex3f(200.f, 200.f, 0.f);
//		glColor3f(1.f, 1.f, 1.f);
//		glVertex3f(0.f, 200.f, 0.f);
//		glEnd();

		glBegin(GL_QUADS);                  // Start Drawing The Cube

		glColor3f(0.0f,1.0f,0.0f);          // Set The Color To Green
		glVertex3f( 1.0f, 1.0f,-1.0f);          // Top Right Of The Quad (Top)
		glVertex3f(-1.0f, 1.0f,-1.0f);          // Top Left Of The Quad (Top)
		glVertex3f(-1.0f, 1.0f, 1.0f);          // Bottom Left Of The Quad (Top)
		glVertex3f( 1.0f, 1.0f, 1.0f);          // Bottom Right Of The Quad (Top)

		glColor3f(1.0f,0.5f,0.0f);          // Set The Color To Orange
		glVertex3f( 1.0f,-1.0f, 1.0f);          // Top Right Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f, 1.0f);          // Top Left Of The Quad (Bottom)
		glVertex3f(-1.0f,-1.0f,-1.0f);          // Bottom Left Of The Quad (Bottom)
		glVertex3f( 1.0f,-1.0f,-1.0f);          // Bottom Right Of The Quad (Bottom)

		glColor3f(1.0f,0.0f,0.0f);          // Set The Color To Red
		glVertex3f( 1.0f, 1.0f, 1.0f);          // Top Right Of The Quad (Front)
		glVertex3f(-1.0f, 1.0f, 1.0f);          // Top Left Of The Quad (Front)
		glVertex3f(-1.0f,-1.0f, 1.0f);          // Bottom Left Of The Quad (Front)
		glVertex3f( 1.0f,-1.0f, 1.0f);          // Bottom Right Of The Quad (Front)

		glColor3f(1.0f,1.0f,0.0f);          // Set The Color To Yellow
		glVertex3f( 1.0f,-1.0f,-1.0f);          // Bottom Left Of The Quad (Back)
		glVertex3f(-1.0f,-1.0f,-1.0f);          // Bottom Right Of The Quad (Back)
		glVertex3f(-1.0f, 1.0f,-1.0f);          // Top Right Of The Quad (Back)
		glVertex3f( 1.0f, 1.0f,-1.0f);          // Top Left Of The Quad (Back)

		glColor3f(0.0f,0.0f,1.0f);          // Set The Color To Blue
		glVertex3f(-1.0f, 1.0f, 1.0f);          // Top Right Of The Quad (Left)
		glVertex3f(-1.0f, 1.0f,-1.0f);          // Top Left Of The Quad (Left)
		glVertex3f(-1.0f,-1.0f,-1.0f);          // Bottom Left Of The Quad (Left)
		glVertex3f(-1.0f,-1.0f, 1.0f);          // Bottom Right Of The Quad (Left)

		        glColor3f(1.0f,0.0f,1.0f);          // Set The Color To Violet
		        glVertex3f( 1.0f, 1.0f,-1.0f);          // Top Right Of The Quad (Right)
		        glVertex3f( 1.0f, 1.0f, 1.0f);          // Top Left Of The Quad (Right)
		        glVertex3f( 1.0f,-1.0f, 1.0f);          // Bottom Left Of The Quad (Right)
		        glVertex3f( 1.0f,-1.0f,-1.0f);          // Bottom Right Of The Quad (Right)
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

