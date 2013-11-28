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

	GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};  /* Red diffuse light. */
	GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  /* Infinite light location. */
	GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
	  {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
	  {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
	GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
	  {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
	  {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
	GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */

	Viewport3D::Viewport3D()
	: Widget()
	{
		Camera* default_camera = new Camera;
		m_cameras.push_back(default_camera);

		  /* Setup cube vertex data. */
		  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
		  v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
		  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
		  v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
		  v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
		  v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;

	}

	Viewport3D::Viewport3D(AbstractWidget* parent)
	: Widget(parent)
	{
		Camera* default_camera = new Camera;
		m_cameras.push_back(default_camera);

		  /* Setup cube vertex data. */
		  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
		  v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
		  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
		  v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
		  v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
		  v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;

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

		  /* Use depth buffering for hidden surface elimination. */
		  glEnable(GL_DEPTH_TEST);

		  int i;

		  for (i = 0; i < 6; i++) {
		    glBegin(GL_QUADS);
		    glNormal3fv(&n[i][0]);
		    glVertex3fv(&v[faces[i][0]][0]);
		    glVertex3fv(&v[faces[i][1]][0]);
		    glVertex3fv(&v[faces[i][2]][0]);
		    glVertex3fv(&v[faces[i][3]][0]);
		    glEnd();
		  }
		 DrawGrid(10.f, 10.f, 0.2f, 1.0f);

		//-------------------------------------------------------------------------------------------------------

		  glDisable(GL_DEPTH_TEST);


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

}

