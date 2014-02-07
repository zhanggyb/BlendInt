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

	GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};  /* Red diffuse light. */
	GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  /* Infinite light location. */
	GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
	  {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
	  {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
	GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
	  {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
	  {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
	GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */

	Viewport3D::Viewport3D ()
			: Widget(), m_default_camera(0), m_grid(0), m_xold(0), m_yold(0), m_rotate_x(
			        0.0), m_rotate_y(0.0), m_left_down(false)
	{
		InitOnce();
	}

	Viewport3D::Viewport3D (AbstractWidget* parent)
			: Widget(parent), m_default_camera(0), m_grid(0), m_xold(0), m_yold(
			        0), m_rotate_x(0.0), m_rotate_y(0.0), m_left_down(false)
	{
		InitOnce();
	}

	Viewport3D::~Viewport3D ()
	{
		Destroy(m_default_camera);
		Destroy(m_grid);
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
		/*
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

				case Key_Up:
					m_cameras[0]->set_position(x, y, z + 5);
					break;

				case Key_Down:
					m_cameras[0]->set_position(x, y, z - 5);
					break;

				default:
					break;
			}
		}
		*/
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
		if (m_left_down)
		{
			m_rotate_y = m_rotate_y + (event->position().y() - m_yold) / 5.f;
			m_rotate_x = m_rotate_x + (event->position().x() - m_xold) / 5.f;

			if (m_rotate_y > 90)
				m_rotate_y = 90;
			if (m_rotate_y < -90)
				m_rotate_y = -90;
		}

		m_xold = event->position().x();
		m_yold = event->position().y();
	}

	void Viewport3D::Render ()
	{
//		/* Clear the buffer, clear the matrix */
		glClearColor(0.25, 0.25, 0.25, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		glRotatef(m_rotate_y, 1, 0, 0);
//		glRotatef(m_rotate_x, 0, 1, 0);
//
//		DrawGrid(50, 50, 1, 10);
//
//		//test code
		//DrawCube();

//		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
//		glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
//		glm::mat4 projection = glm::perspective(45.0f, 1.0f*5/4, 0.1f, 10.0f);
//
//		glm::mat4 mvp = projection * view * model;
//
//		m_grid->Render(mvp);

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

//		gluPerspective(45.0f,(GLfloat)size().width()/(GLfloat)size().height(),0.1f,100.0f);
//		gluLookAt (5.0f, 5.0f, 7.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

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

	void Viewport3D::DrawCube ()
	{
		/* We tell we want to draw quads */
		glBegin(GL_QUADS);

		/* Every four calls to glVertex, a quad is drawn */
		glColor3f(0, 0, 0); glVertex3f(-1, -1, -1);
		glColor3f(0, 0, 1); glVertex3f(-1, -1,  1);
		glColor3f(0, 1, 1); glVertex3f(-1,  1,  1);
		glColor3f(0, 1, 0); glVertex3f(-1,  1, -1);

		glColor3f(1, 0, 0); glVertex3f( 1, -1, -1);
		glColor3f(1, 0, 1); glVertex3f( 1, -1,  1);
		glColor3f(1, 1, 1); glVertex3f( 1,  1,  1);
		glColor3f(1, 1, 0); glVertex3f( 1,  1, -1);

		glColor3f(0, 0, 0); glVertex3f(-1, -1, -1);
		glColor3f(0, 0, 1); glVertex3f(-1, -1,  1);
		glColor3f(1, 0, 1); glVertex3f( 1, -1,  1);
		glColor3f(1, 0, 0); glVertex3f( 1, -1, -1);

		glColor3f(0, 1, 0); glVertex3f(-1,  1, -1);
		glColor3f(0, 1, 1); glVertex3f(-1,  1,  1);
		glColor3f(1, 1, 1); glVertex3f( 1,  1,  1);
		glColor3f(1, 1, 0); glVertex3f( 1,  1, -1);

		glColor3f(0, 0, 0); glVertex3f(-1, -1, -1);
		glColor3f(0, 1, 0); glVertex3f(-1,  1, -1);
		glColor3f(1, 1, 0); glVertex3f( 1,  1, -1);
		glColor3f(1, 0, 0); glVertex3f( 1, -1, -1);

		glColor3f(0, 0, 1); glVertex3f(-1, -1,  1);
		glColor3f(0, 1, 1); glVertex3f(-1,  1,  1);
		glColor3f(1, 1, 1); glVertex3f( 1,  1,  1);
		glColor3f(1, 0, 1); glVertex3f( 1, -1,  1);

		/* No more quads */
		glEnd();
	}

	void Viewport3D::DrawGrid (float width, float height, float small_step, float big_step)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(- width / 2.0, - height / 2.0, 0.0);

		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glColor4f(1.0f, 1.0f, 1.0f, 0.1f);
		glLineStipple(1, 0xAAAA);

		for (int num = 1; num < width; num++) {
			int step = num * small_step;
			glBegin(GL_LINES);
			glVertex2f(0, step);
			glVertex2f(width, step);
			glEnd();
		}

		for (int num = 1; num < height; num++) {
			int step = num * small_step;
			glBegin(GL_LINES);
			glVertex2f(step, 0);
			glVertex2f(step, height);
			glEnd();
		}

		glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
		glLineStipple(1, 0xAAAA);
		for (int num = 1; num < width; num++) {
			int step = num * big_step;
			glBegin(GL_LINES);
			glVertex2f(0, step);
			glVertex2f(width, step);
			glEnd();
		}

		for (int num = 1; num < height; num++) {
			int step = num * big_step;
			glBegin(GL_LINES);
			glVertex2f(step, 0);
			glVertex2f(step, height);
			glEnd();
		}

		glDisable(GL_LINE_STIPPLE);

		glPopMatrix();
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
		m_default_camera->Rotate(yaw,pitch,0);

		m_default_camera->SetProjection(45, 5.0f/4);

		m_grid = new Grid;
		Retain(m_grid);

		m_grid->SetProgram(ShaderManager::Instance()->primitive_program());

		m_cube = new Cube;
		Retain(m_cube);
		m_cube->SetProgram(ShaderManager::Instance()->primitive_program());

		/* Setup cube vertex data. */
		v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
		v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
		v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
		v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
		v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
		v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;
	}

}

