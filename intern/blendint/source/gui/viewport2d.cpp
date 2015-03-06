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

#include <opengl/opengl.hpp>

#include <gui/viewport2d.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

	const float Viewport2D::camera_factor = 50.f;

	Viewport2D::Viewport2D()
	: AbstractRoundWidget(),
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

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			GenerateVertices(*request.size(), 0, RoundNone, 0.f, &inner_verts, 0);
			inner_->bind();
			inner_->set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			inner_->reset();

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

	Response Viewport2D::Draw(AbstractWindow* context)
	{
		AbstractWindow* c = context;
        GLint vp[4];	// Original viewport
        int n = GetOutlineVertices(round_type()) + 2;

        glGetIntegerv(GL_VIEWPORT, vp);

		RefPtr<GLSLProgram> program = AbstractWindow::shaders()->widget_inner_program();
		program->use();

		glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA), 0);
		glUniform4f(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR),
				0.25f, 0.25f, 0.25f, 1.f);

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, n);

		c->BeginPushStencil();	// inner stencil
		glDrawArrays(GL_TRIANGLE_FAN, 0, n);
		c->EndPushStencil();

		glBindVertexArray(0);
		program->reset();

        glEnable(GL_DEPTH_TEST);

        Point pos = GetGlobalPosition();

		glViewport(pos.x() - context->viewport_origin().x(),
		        pos.y() - context->viewport_origin().y(),
		        size().width(),
		        size().height());

        // --------------------------------------------------------------------------------

		gridfloor_->Render(camera_.projection(), camera_.view());

		//Render();
        // --------------------------------------------------------------------------------

        glDisable(GL_DEPTH_TEST);
        glViewport(vp[0], vp[1], vp[2], vp[3]);

        program->use();
		glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA), 0);

		c->BeginPopStencil();	// pop inner stencil
		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, n);
		glBindVertexArray(0);
		c->EndPopStencil();
		program->reset();

		return Finish;
	}

	void Viewport2D::InitializeViewport2D()
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
