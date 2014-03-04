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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <BlendInt/Gui/Viewport3D.hpp>
#include <BlendInt/Interface.hpp>

#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

	Viewport3D::Viewport3D ()
			: Widget(), m_default_camera(0), m_cube(0), m_grid(0), m_last_x(0),
			  m_last_y(0), m_rX(0.0), m_rY(0.0), m_button_down(MouseButtonNone)
	{
		InitOnce();
	}

	Viewport3D::~Viewport3D ()
	{
		m_cameras.clear();
	}

	void Viewport3D::KeyPressEvent (KeyEvent* event)
	{
        if(event->action() == KeyPress) {
            switch(event->key()) {
                case Key_KP_Decimal: {
                    // setup camera
                    glm::vec3 pos = glm::vec3(5.f);
                    glm::vec3 center = glm::vec3(0);
                    glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);
                    m_default_camera->LookAt(pos, center, up);
                    m_default_camera->SetPerspective(m_default_camera->fovy(), 1.0f * size().width()/size().height());
                    Refresh();
                    break;
                }
                    
                default:
                    break;
            }
        } else if(event->action() == KeyRelease && m_button_down == MouseButtonMiddle) {

        	// currently does nothing
        	switch(event->key()) {
        		case Key_LeftShift:
        		case Key_RightShift:
        			break;

        		case Key_LeftAlt:
        		case Key_RightAlt:
        			break;

        		default:
        			break;
            }

        }
        event->accept(this);
	}

	void Viewport3D::MousePressEvent (MouseEvent* event)
	{
		m_button_down = event->button();

		m_last_x = event->position().x();
		m_last_y = event->position().y();

		if(m_button_down == MouseButtonMiddle) {

			if(event->modifiers() == ModifierNone) {

                m_default_camera->SaveCurrentPosition();
                m_default_camera->SaveCurrentCenter();

			} else if (event->modifiers() == ModifierShift) {

				m_default_camera->SaveCurrentPosition();
				m_default_camera->SaveCurrentCenter();

            } else if (event->modifiers() == ModifierControl) {

                m_default_camera->SaveCurrentPosition();

            }

		} else if (m_button_down == MouseButtonScrollUp) {

			std::cout << "scroll up" << std::endl;

            m_default_camera->SaveCurrentPosition();

			m_default_camera->Zoom(5.f);

		} else if (m_button_down == MouseButtonScrollDown) {

			std::cout << "scroll down" << std::endl;

			m_default_camera->SaveCurrentPosition();

			m_default_camera->Zoom(-5.f);

		}

		event->accept(this);
	}

	void Viewport3D::MouseReleaseEvent (MouseEvent* event)
	{
		m_button_down = MouseButtonNone;
		event->accept(this);
	}

	void Viewport3D::MouseMoveEvent (MouseEvent* event)
	{
		switch (m_button_down) {
			case MouseButtonLeft: {
				break;
			}

			case MouseButtonMiddle: {

				if (event->modifiers() == ModifierShift) {

					float dx = static_cast<float>(m_last_x
					        - event->position().x());
					float dy = static_cast<float>(m_last_y
					        - event->position().y());
					m_default_camera->Pan(dx, dy);

				} else if (event->modifiers() == ModifierControl) {

					m_default_camera->Zoom(m_last_y - event->position().y());

				} else if (event->modifiers() == ModifierNone) {
					float dx = static_cast<float>(m_last_x
					        - event->position().x());
					float dy = static_cast<float>(m_last_y
					        - event->position().y());
					m_default_camera->Orbit(dx, dy);
				}

				Refresh();

				break;
			}

			case MouseButtonRight: {
				break;
			}

			default:
				break;
		}

		event->accept(this);
	}

	bool Viewport3D::Update(int type, const void* data)
	{
		switch(type) {
			case FormSize: {

				const Size* size_p = static_cast<const Size*>(data);
				m_default_camera->SetPerspective(m_default_camera->fovy(), 1.f * size_p->width()/size_p->height());

				return true;
			}

			default:
				return Widget::Update(type, data);
		}

	}

	void Viewport3D::Render ()
	{
		/* Clear the buffer, clear the matrix */
		glClearColor(0.25, 0.25, 0.25, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_cube->Render(m_default_camera->projection() * m_default_camera->view());

		m_grid->Render(m_default_camera->projection() * m_default_camera->view());
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

		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_DEPTH_TEST);

		// set back the previous matrices and viewport
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixd(proj_matrix);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixd(model_matrix);

		glViewport(0, 0, Interface::Instance()->size().width(), Interface::Instance()->size().height());

#ifdef DEBUG
		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		if(focused()) {
			glColor4f(1.0f, 1.0f, 0.0f, 0.75f);
		} else {
			glColor4f(1.0f, 1.0f, 1.0f, 0.75f);
		}
		glLineStipple(1, 0xAAAA);
		glBegin(GL_LINE_LOOP);
		glVertex2i(0, 0);
		glVertex2i(size().width(), 0);
		glVertex2i(size().width(), size().height());
		glVertex2i(0, size().height());
		glEnd();

		glDisable(GL_LINE_STIPPLE);

#endif
	}

	void Viewport3D::InitOnce()
	{
		set_expand_x(true);
		set_expand_y(true);

		m_default_camera.reset(new NavigationCamera);

		// setup camera
		glm::vec3 pos = glm::vec3(5.f);
		glm::vec3 center = glm::vec3(0);
		glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);
		m_default_camera->LookAt(pos, center, up);

		m_default_camera->SetPerspective(m_default_camera->fovy(), 1.f * size().width()/size().height());
		m_default_camera->Update();

		m_cube.reset(new Cube);

		m_grid.reset(new Grid);
	}

}

