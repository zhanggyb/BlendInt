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
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <BlendInt/Gui/Viewport3D.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Stock/Shaders.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Viewport3D::Viewport3D ()
	: AbstractWidget(),
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

	ResponseType Viewport3D::CursorEnterEvent (bool entered)
	{
		Context* context = Context::GetContext(this);
		if(!context) return Ignore;

		if(entered) {
			context->PushCursor(context->current_cursor());
			context->SetCursor(CrossCursor);
		} else {
			context->SetCursor(context->PopCursor());
		}

		return Accept;
	}

	ResponseType Viewport3D::KeyPressEvent (const KeyEvent& event)
	{
		if (event.action() == KeyPress) {
			switch (event.key()) {
				case Key_KP_Decimal: {
					// setup camera
					glm::vec3 pos = glm::vec3(8.f, -10.f, 6.f);
					glm::vec3 center = glm::vec3(0);
					glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);
					default_camera_->LookAt(pos, center, up);
					default_camera_->SetPerspective(default_camera_->fovy(),
					        1.0f * size().width() / size().height());
					Refresh();
					break;
				}

				default:
					break;
			}
		} else if (event.action() == KeyRelease
		        && m_button_down == MouseButtonMiddle) {

			// currently does nothing
			switch (event.key()) {
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

		return Accept;
	}

	ResponseType Viewport3D::MousePressEvent (const MouseEvent& event)
	{
		m_button_down = event.button();

		m_last_x = event.position().x();
		m_last_y = event.position().y();

		if (m_button_down == MouseButtonMiddle) {

			if (event.modifiers() == ModifierNone) {

				default_camera_->SaveCurrentPosition();
				default_camera_->SaveCurrentCenter();

			} else if (event.modifiers() == ModifierShift) {

				default_camera_->SaveCurrentPosition();
				default_camera_->SaveCurrentCenter();

			} else if (event.modifiers() == ModifierControl) {

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

		return Accept;
	}

	ResponseType Viewport3D::MouseReleaseEvent (const MouseEvent& event)
	{
		m_button_down = MouseButtonNone;

		return Accept;
	}

	ResponseType Viewport3D::MouseMoveEvent (const MouseEvent& event)
	{
		switch (m_button_down) {
			case MouseButtonLeft: {
				break;
			}

			case MouseButtonMiddle: {

				if (event.modifiers() == ModifierShift) {

					float dx = static_cast<float>(m_last_x
					        - event.position().x());
					float dy = static_cast<float>(m_last_y
					        - event.position().y());
					default_camera_->Pan(dx, dy);

				} else if (event.modifiers() == ModifierControl) {

					default_camera_->Zoom(m_last_y - event.position().y());

				} else if (event.modifiers() == ModifierNone) {
					float dx = static_cast<float>(m_last_x
					        - event.position().x());
					float dy = static_cast<float>(m_last_y
					        - event.position().y());
					default_camera_->Orbit(dx, dy);
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

		return Accept;
	}

	void Viewport3D::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			default_camera_->SetPerspective(default_camera_->fovy(),
			        1.f * request.size()->width() / request.size()->height());

			VertexTool tool;
			tool.GenerateVertices(*request.size(), 0, RoundNone, 0.f);
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			inner_->reset();

			set_size(*request.size());
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

	ResponseType Viewport3D::Draw (Profile& profile)
	{
        GLint vp[4];	// Original viewport
        //GLint sci[4];
        //GLboolean scissor_status;

        glGetIntegerv(GL_VIEWPORT, vp);
        //glGetBooleanv(GL_SCISSOR_TEST, &scissor_status);

        //if(scissor_status == GL_TRUE) {
        //	glGetIntegerv(GL_SCISSOR_BOX, sci);
        //}

		RefPtr<GLSLProgram> program = Shaders::instance->triangle_program();
		program->use();

		glUniform3f(Shaders::instance->location(Stock::TRIANGLE_POSITION), (float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_ANTI_ALIAS), 0);

		glVertexAttrib4f(Shaders::instance->location(Stock::TRIANGLE_COLOR),
				0.25f, 0.25f, 0.25f, 1.f);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);

		profile.BeginPushStencil();	// inner stencil
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);
		profile.EndPushStencil();

		glBindVertexArray(0);
		program->reset();

        glEnable(GL_DEPTH_TEST);
//        glEnable(GL_SCISSOR_TEST);
//        glScissor(position().x(), position().y(), size().width(),
//                size().height());

		glViewport(position().x() - profile.origin().x(),
		        position().y() - profile.origin().y(),
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
		glUniform3f(Shaders::instance->location(Stock::TRIANGLE_POSITION), (float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_GAMMA), 0);
		glUniform1i(Shaders::instance->location(Stock::TRIANGLE_ANTI_ALIAS), 0);

		profile.BeginPopStencil();	// pop inner stencil
		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0,
							GetOutlineVertices(round_type()) + 2);
		glBindVertexArray(0);
		profile.EndPopStencil();
		program->reset();

		return Accept;
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

	ResponseType Viewport3D::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType Viewport3D::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Viewport3D::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	void Viewport3D::PushBack (const RefPtr<AbstractPrimitive>& primitive)
	{
		m_primitives.push_back(primitive);
	}

	void Viewport3D::InitializeViewport3DOnce ()
	{
		VertexTool tool;
		tool.GenerateVertices(size(), 0, RoundNone, 0.f);

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();

		inner_->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(Shaders::instance->location(Stock::TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::TRIANGLE_COORD), 2,
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
