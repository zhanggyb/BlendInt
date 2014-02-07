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

#ifdef __UNIX__
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#endif
#endif  // __UNIX__

#include <iostream>

#include <BlendInt/Viewport3D.hpp>
#include <BlendInt/Interface.hpp>
#include <BlendInt/ShaderManager.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace BlendInt {

	Viewport3D::Viewport3D ()
			: Widget(), m_default_camera(0), m_xold(0),
			  m_yold(0), m_rX(0.0), m_rY(0.0), m_left_down(false)
	{
		InitOnce();
	}

	Viewport3D::Viewport3D (AbstractWidget* parent)
			: Widget(parent), m_default_camera(0), m_xold(0),
			  m_yold(0), m_rX(0.0), m_rY(0.0), m_left_down(false)
	{
		InitOnce();
	}

	Viewport3D::~Viewport3D ()
	{
		Destroy(m_default_camera);
		Destroy(m_cube);

		vector<AbstractCamera*>::iterator it;

		for(it = m_cameras.begin(); it != m_cameras.end(); it++)
		{
			Destroy(*it);
		}

		m_cameras.clear();
	}

	void Viewport3D::KeyPressEvent (KeyEvent* event)
	{

	}

	void Viewport3D::MousePressEvent (MouseEvent* event)
	{
		if(event->button() == MouseButtonLeft) {
			m_left_down = true;
		}
		m_xold = event->position().x();
		m_yold = event->position().y();
	}

	void Viewport3D::MouseReleaseEvent (MouseEvent* event)
	{
		if(event->button() == MouseButtonLeft) {
			m_left_down = false;
		}
	}

	void Viewport3D::MouseMoveEvent (MouseEvent* event)
	{
		if(m_left_down) {
			m_rY += (event->position().y() - m_yold) / 100.0f;
			m_rX += (m_xold - event->position().x()) / 100.0f;

			m_default_camera->Rotate(m_rX, m_rY, 0);
			m_default_camera->Update();
		}
	}

	void Viewport3D::Render ()
	{
		/* Clear the buffer, clear the matrix */
		glClearColor(0.25, 0.25, 0.25, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_cube->Render(m_default_camera->projection() * m_default_camera->view());
	}

	void Viewport3D::Draw ()
	{
		// store the current matrices
		GLdouble proj_matrix[16];
		GLdouble model_matrix[16];

		glGetDoublev(GL_MODELVIEW_MATRIX, model_matrix);
		glGetDoublev(GL_PROJECTION_MATRIX, proj_matrix);

		glEnable(GL_DEPTH_TEST);
		glEnable (GL_SCISSOR_TEST);
		glScissor (position().x(),
				position().y(),
				size().width(),
				size().height());

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//glOrtho(0.f, (float) size().width(), 0.f, (float) size().height(), 100.f, -100.f);
		//glOrtho(0.f, 8.0f, 0.f, 6.0f, 100.f, -100.f);

		//gluPerspective(45.0f,(GLfloat)size().width()/(GLfloat)size().height(),0.1f,100.0f);
		//gluLookAt (5.0f, 5.0f, 7.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		glViewport(position().x(), position().y(), size().width(), size().height());

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//-------------------------------------------------------------------------------------------------------

		Render ();

		//-------------------------------------------------------------------------------------------------------

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_SCISSOR_TEST);

		// set back the previous matrices and viewport
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixd(proj_matrix);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(model_matrix);

		glViewport(0, 0, Interface::Instance()->size().width(), Interface::Instance()->size().height());

#ifdef DEBUG
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
#endif
	}

	void Viewport3D::InitOnce()
	{
		m_default_camera = new FreeCamera;
		Retain(m_default_camera);

		// setup camera
		glm::vec3 p = glm::vec3(5.f);
		m_default_camera->set_position(5.f, 5.f, 5.f);
		glm::vec3 look =  glm::normalize(p);

		//rotate the camera for proper orientation
		float yaw = glm::degrees(float(atan2(look.z, look.x)+M_PI));
		float pitch = glm::degrees(asin(look.y));
		m_default_camera->set_look(look);
		m_default_camera->Rotate(yaw,pitch,0);

		m_default_camera->SetProjection(m_default_camera->fovy(), 5.0f/4);
		//m_default_camera->Rotate(10.f, 5.f, 15.f);
		m_default_camera->Update();

		m_cube = new Cube;
		Retain(m_cube);
		//m_cube->SetProgram(ShaderManager::Instance()->primitive_program());
	}

}

