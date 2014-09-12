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

#include <BlendInt/Gui/Viewport2D.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Stock/Shaders.hpp>

#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	using Stock::Shaders;

	const float Viewport2D::camera_factor = 50.f;

	Viewport2D::Viewport2D()
	: AbstractWidget(),
	  vao_(0),
	  gridfloor_(0)
	{
		set_size(600, 500);

		InitializeViewport2D();
	}

	Viewport2D::~Viewport2D ()
	{
		glDeleteVertexArrays(1, &vao_);

		if(gridfloor_)
			delete gridfloor_;
	}

	Size Viewport2D::GetPreferredSize() const
	{
		return Size(2560, 2560);
	}

	bool Viewport2D::IsExpandX() const
	{
		return true;
	}

	bool Viewport2D::IsExpandY() const
	{
		return true;
	}

	void Viewport2D::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if (request.target() == this) {

			VertexTool tool;
			tool.GenerateVertices(*request.size(), 0, RoundNone, 0.f);
			inner_->bind();
			inner_->set_data(tool.inner_size(), tool.inner_data());
			inner_->reset();

			set_size(*request.size());

			float left = -size().width() / camera_factor;
			float right = size().width() / camera_factor;
			float bottom = -size().height() / camera_factor;
			float top = size().height() / camera_factor;

			camera_.SetOrtho(left, right, bottom, top);
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	ResponseType Viewport2D::FocusEvent(bool focus)
	{
		return Accept;
	}

	ResponseType Viewport2D::CursorEnterEvent(bool entered)
	{
		return Accept;
	}

	ResponseType Viewport2D::KeyPressEvent(const KeyEvent& event)
	{
		return Accept;
	}

	ResponseType Viewport2D::ContextMenuPressEvent(
			const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType Viewport2D::ContextMenuReleaseEvent(
			const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType Viewport2D::MousePressEvent(const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType Viewport2D::MouseReleaseEvent(const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType Viewport2D::MouseMoveEvent(const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType Viewport2D::Draw(Profile& profile)
	{
        GLint vp[4];	// Original viewport

        glGetIntegerv(GL_VIEWPORT, vp);

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

		glViewport(position().x() - profile.origin().x(),
		        position().y() - profile.origin().y(),
		        size().width(),
		        size().height());

        // --------------------------------------------------------------------------------

		gridfloor_->Render(camera_.projection(), camera_.view());

		//Render();
        // --------------------------------------------------------------------------------

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

	void Viewport2D::InitializeViewport2D()
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

		glm::vec3 pos = glm::vec3(0.f, 0.f, 10.f);
		glm::vec3 center = glm::vec3(0);
		glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
		camera_.LookAt(pos, center, up);

		float left = -size().width() / camera_factor;
		float right = size().width() / camera_factor;
		float bottom = -size().height() / camera_factor;
		float top = size().height() / camera_factor;

		camera_.SetOrtho(left, right, bottom, top);

		gridfloor_ = new GridFloor;
	}

}
