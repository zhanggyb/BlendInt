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

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <opengl/opengl.hpp>

#include <gui/viewport3d.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

	Viewport3D::Viewport3D ()
	: AbstractRoundWidget(),
	  vao_(0),
	  m_last_x(0),
	  m_last_y(0),
	  m_rX(0.0),
	  m_rY(0.0),
	  m_button_down(MouseButtonNone)
	{
		set_size(600, 500);

		InitializeViewport3DOnce();
	}

	Viewport3D::~Viewport3D ()
	{
		glDeleteVertexArrays(1, &vao_);
		cameras_.clear();
	}

	void Viewport3D::PerformHoverIn(AbstractWindow* context)
	{
		context->PushCursor();
		context->SetCursor(CrossCursor);
	}

	void Viewport3D::PerformHoverOut(AbstractWindow* context)
	{
		context->PopCursor();
	}

	Response Viewport3D::PerformKeyPress (AbstractWindow* context)
	{
		if (context->GetKeyAction() == KeyPress) {
			switch (context->GetKeyInput()) {
				case Key_KP_Decimal: {
					// setup camera
					glm::vec3 pos = glm::vec3(8.f, -10.f, 6.f);
					glm::vec3 center = glm::vec3(0);
					glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);
					default_camera_->LookAt(pos, center, up);
					default_camera_->SetPerspective(default_camera_->fovy(),
					        1.0f * size().width() / size().height());
					RequestRedraw();
					break;
				}

				default:
					break;
			}
		} else if (context->GetKeyAction() == KeyRelease
		        && m_button_down == MouseButtonMiddle) {

			// currently does nothing
			switch (context->GetKeyInput()) {
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

		return Finish;
	}

	Response Viewport3D::PerformMousePress (AbstractWindow* context)
	{
		m_button_down = context->GetMouseButton();

		m_last_x = context->GetGlobalCursorPosition().x();
		m_last_y = context->GetGlobalCursorPosition().y();

		if (m_button_down == MouseButtonMiddle) {

			if (context->GetModifiers() == ModifierNone) {

				default_camera_->SaveCurrentPosition();
				default_camera_->SaveCurrentCenter();

			} else if (context->GetModifiers() == ModifierShift) {

				default_camera_->SaveCurrentPosition();
				default_camera_->SaveCurrentCenter();

			} else if (context->GetModifiers() == ModifierControl) {

				default_camera_->SaveCurrentPosition();

			}

		} else if (m_button_down == MouseButtonScrollUp) {

			std::cout << "scroll up" << std::endl;

			default_camera_->SaveCurrentPosition();

			default_camera_->Zoom(5.f);

		} else if (m_button_down == MouseButtonScrollDown) {

			std::cout << "scroll down" << std::endl;

			default_camera_->SaveCurrentPosition();

			default_camera_->Zoom(-5.f);

		}

		return Finish;
	}

	Response Viewport3D::PerformMouseRelease (AbstractWindow* context)
	{
		m_button_down = MouseButtonNone;

		return Finish;
	}

	Response Viewport3D::PerformMouseMove (AbstractWindow* context)
	{
		switch (m_button_down) {
			case MouseButtonLeft: {
				break;
			}

			case MouseButtonMiddle: {

				if (context->GetModifiers() == ModifierShift) {

					float dx = static_cast<float>(m_last_x
					        - context->GetGlobalCursorPosition().x());
					float dy = static_cast<float>(m_last_y
					        - context->GetGlobalCursorPosition().y());
					default_camera_->Pan(dx, dy);

				} else if (context->GetModifiers() == ModifierControl) {

					default_camera_->Zoom(m_last_y - context->GetGlobalCursorPosition().y());

				} else if (context->GetModifiers() == ModifierNone) {
					float dx = static_cast<float>(m_last_x
					        - context->GetGlobalCursorPosition().x());
					float dy = static_cast<float>(m_last_y
					        - context->GetGlobalCursorPosition().y());
					default_camera_->Orbit(dx, dy);
				}

				RequestRedraw();

				break;
			}

			case MouseButtonRight: {
				break;
			}

			default:
				break;
		}

		return Finish;
	}

	void Viewport3D::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			set_size(*request.size());

			default_camera_->SetPerspective(default_camera_->fovy(),
			        1.f * request.size()->width() / request.size()->height());

			std::vector<GLfloat> inner_verts;
			GenerateVertices(*request.size(), 0, RoundNone, 0.f, &inner_verts, 0);
			inner_->bind();
			inner_->set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			inner_->reset();

		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void Viewport3D::Render ()
	{
		/* Clear the buffer, clear the matrix */
		//glClearColor(0.25, 0.25, 0.25, 1.0);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		gridfloor_->Render(default_camera_->projection(), default_camera_->view());

		for(std::deque<RefPtr<AbstractPrimitive> >::iterator it = m_primitives.begin(); it != m_primitives.end(); it++)
		{
			(*it)->Render(default_camera_->projection(), default_camera_->view());
		}
	}

	Response Viewport3D::Draw (AbstractWindow* context)
	{
		AbstractWindow* c = context;
        GLint vp[4];	// Original viewport
        //GLint sci[4];
        //GLboolean scissor_status;
        int n = GetOutlineVertices(round_type()) + 2;

        glGetIntegerv(GL_VIEWPORT, vp);
        //glGetBooleanv(GL_SCISSOR_TEST, &scissor_status);

        //if(scissor_status == GL_TRUE) {
        //	glGetIntegerv(GL_SCISSOR_BOX, sci);
        //}

		RefPtr<GLSLProgram> program = AbstractWindow::shaders->widget_inner_program();
		program->use();

		glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_GAMMA), 0);
		glUniform4f(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_COLOR),
				0.25f, 0.25f, 0.25f, 1.f);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, n);

		c->BeginPushStencil();	// inner stencil
		glDrawArrays(GL_TRIANGLE_FAN, 0, n);
		c->EndPushStencil();

		glBindVertexArray(0);
		program->reset();

        glEnable(GL_DEPTH_TEST);
//        glEnable(GL_SCISSOR_TEST);
//        glScissor(position().x(), position().y(), size().width(),
//                size().height());

        Point pos = GetGlobalPosition();

        glViewport(pos.x() - context->viewport_origin().x(),
        		pos.y() - context->viewport_origin().y(),
		        size().width(),
		        size().height());

        // --------------------------------------------------------------------------------
        Render();
        // --------------------------------------------------------------------------------

//        if(scissor_status == GL_TRUE) {
//        	glScissor(sci[0], sci[1], sci[2], sci[3]);
//        } else {
//        	glDisable(GL_SCISSOR_TEST);
//        }

        glDisable(GL_DEPTH_TEST);

        glViewport(vp[0], vp[1], vp[2], vp[3]);

        program->use();
		glUniform1i(AbstractWindow::shaders->location(Shaders::WIDGET_INNER_GAMMA), 0);

		c->BeginPopStencil();	// pop inner stencil
		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, n);
		glBindVertexArray(0);
		c->EndPopStencil();
		program->reset();

		return Finish;
	}
	
	Size Viewport3D::GetPreferredSize () const
	{
		return Size(2560, 2560);
	}

	bool Viewport3D::IsExpandX() const
	{
		return true;
	}

	bool Viewport3D::IsExpandY() const
	{
		return true;
	}

	void Viewport3D::PushBack (const RefPtr<AbstractPrimitive>& primitive)
	{
		m_primitives.push_back(primitive);
	}

	void Viewport3D::InitializeViewport3DOnce ()
	{
		std::vector<GLfloat> inner_verts;
		GenerateVertices(size(), 0, RoundNone, 0.f, &inner_verts, 0);

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();

		inner_->set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord, 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();

		default_camera_.reset(new PerspectiveCamera);

		// setup camera
		glm::vec3 pos = glm::vec3(8.f, -10.f, 6.f);
		glm::vec3 center = glm::vec3(0);
		glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);
		default_camera_->LookAt(pos, center, up);

		default_camera_->SetPerspective(default_camera_->fovy(),
		        1.f * size().width() / size().height());

		gridfloor_.reset(new GridFloor);
	}

}
