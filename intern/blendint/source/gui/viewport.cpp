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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include <opengl/opengl.hpp>

#include <gui/viewport.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

	Viewport::Viewport()
	: AbstractFrame(),
	  vao_(0),
	  hover_(false)
	{
		set_size(640, 480);

		projection_matrix_  = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		model_matrix_ = glm::mat3(1.f);

		InitializeViewport();
	}

	Viewport::~Viewport()
	{
		glDeleteVertexArrays(1, &vao_);
	}

	bool Viewport::IsExpandX() const
	{
		return true;
	}

	bool Viewport::IsExpandY() const
	{
		return true;
	}

	Size Viewport::GetPreferredSize() const
	{
		return Size(640, 480);
	}

	Response Viewport::PerformMouseHover(AbstractWindow* context)
	{
		if(Contain(context->GetGlobalCursorPosition())) {
			return Finish;
		} else {
			return Ignore;
		}
	}

	void Viewport::PerformPositionUpdate(const PositionUpdateRequest& request)
	{
		if(request.target() == this) {

			Point offset = GetOffset();
			float x = static_cast<float>(request.position()->x()  + offset.x());
			float y = static_cast<float>(request.position()->y()  + offset.y());

			projection_matrix_  = glm::ortho(
				x,
				x + (float)size().width(),
				y,
				y + (float)size().height(),
				100.f, -100.f);

			model_matrix_ = glm::translate(glm::mat3(1.f), glm::vec2(x, y));

			set_position(*request.position());

		}

		if(request.source() == this) {
			ReportPositionUpdate(request);
		}
	}

	void Viewport::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			Point offset = GetOffset();
			float x = static_cast<float>(position().x() + offset.x());
			float y = static_cast<float>(position().y() + offset.y());

			projection_matrix_  = glm::ortho(
				x,
				x + (float)request.size()->width(),
				y,
				y + (float)request.size()->height(),
				100.f, -100.f);

			set_size(*request.size());

			default_camera_->SetPerspective(default_camera_->fovy(),
			        1.f * request.size()->width() / request.size()->height());

			std::vector<GLfloat> inner_verts;
			GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);

			buffer_.bind();
			buffer_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			buffer_.reset();

		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	Response Viewport::PerformKeyPress(AbstractWindow* context)
	{
		return Ignore;
	}

	void Viewport::PerformFocusOn (AbstractWindow* context)
	{
	}

	void Viewport::PerformFocusOff (AbstractWindow* context)
	{
	}

	void Viewport::PerformHoverIn(AbstractWindow* context)
	{
		hover_ = true;
		RequestRedraw();
	}

	void Viewport::PerformHoverOut(AbstractWindow* context)
	{
		hover_ = false;
		RequestRedraw();
	}

	Response Viewport::PerformMousePress(AbstractWindow* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	Response Viewport::PerformMouseRelease(AbstractWindow* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	Response Viewport::PerformMouseMove(AbstractWindow* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	bool Viewport::PreDraw(AbstractWindow* context)
	{
		if(!visiable()) return false;

		context->register_active_frame(this);

		glViewport(position().x(), position().y(), size().width(), size().height());

		glEnable(GL_SCISSOR_TEST);
		glScissor(position().x(), position().y(), size().width(), size().height());

		AbstractWindow::shaders()->SetWidgetProjectionMatrix(projection_matrix_);
		AbstractWindow::shaders()->SetWidgetModelMatrix(model_matrix_);

		return true;
	}

	Response Viewport::Draw(AbstractWindow* context)
	{
		AbstractWindow::shaders()->widget_inner_program()->use();

		glUniform1i(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA), 0);

		if(hover_) {
			glUniform4f(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR), 0.275f, 0.275f, 0.275f, 1.f);
		} else {
			glUniform4f(AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR), 0.25f, 0.25f, 0.25f, 1.f);
		}

		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glBindVertexArray(0);
		GLSLProgram::reset();

		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);

		gridfloor_->Render(default_camera_->projection(), default_camera_->view());

		glDisable(GL_DEPTH_TEST);
		return Ignore;
	}

	void Viewport::PostDraw(AbstractWindow* context)
	{
		glDisable(GL_SCISSOR_TEST);
		glViewport(0, 0, context->size().width(), context->size().height());
	}

	void Viewport::InitializeViewport()
	{
		std::vector<GLfloat> inner_verts;
		GenerateVertices (size (), 0.f, RoundNone, 0.f, &inner_verts, 0);

		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);

		buffer_.generate();
		buffer_.bind();
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		glEnableVertexAttribArray(AttributeCoord);
		glVertexAttribPointer(AttributeCoord, 3,
				GL_FLOAT, GL_FALSE, 0, 0);
		glBindVertexArray(0);
		buffer_.reset();

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

