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
 * License along with BlendInt.	 If not, see
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
#endif	// __UNIX__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/OpenGL/GLFramebuffer.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Gui/StaticPanel.hpp>

#include <BlendInt/Gui/Decoration.hpp>

#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/Section.hpp>

#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	using Stock::Shaders;

	StaticPanel::StaticPanel ()
	: AbstractPanel(),
	  refresh_(true),
	  pressed_(false),
	  realign_(false),
	  shadow_(0)
	{
		set_margin(10, 10, 10, 10);
		set_round_type(RoundAll);

		InitializeStaticPanelOnce();
	}

	StaticPanel::~StaticPanel ()
	{
		glDeleteVertexArrays(2, vao_);

		if(shadow_) delete shadow_;
	}

	void StaticPanel::PerformRefresh (const RefreshRequest& request)
	{
		if(!pressed_) {

			if(!refresh_) {
				refresh_ = true;
				ReportRefresh(request);
			}

		}
	}

	void StaticPanel::PerformPositionUpdate(const PositionUpdateRequest& request)
	{
		if(request.target() == this) {

			if(!pressed_) {
				int x = request.position()->x() - position().x();
				int y = request.position()->y() - position().y();

				MoveSubWidgets(x, y);
			}

			Refresh();
		}

		if(request.source() == this) {
			ReportPositionUpdate(request);
		}
	}

	void StaticPanel::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(*request.size(), DefaultBorderWidth(), round_type(), round_radius());

			inner_->bind();
			inner_->set_sub_data(0, tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_sub_data(0, tool.outer_size(), tool.outer_data());

			set_size(*request.size());

			shadow_->Resize(size());

			FillSubWidgets(position(), *request.size(), margin());

			refresh_ = true;
			Refresh();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void StaticPanel::PerformRoundTypeUpdate (
	        const RoundTypeUpdateRequest& request)
	{
		if(request.target() == this) {

			VertexTool tool;
			tool.GenerateVertices(size(), DefaultBorderWidth(), *request.round_type(), round_radius());

			inner_->bind();
			inner_->set_sub_data(0, tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_sub_data(0, tool.outer_size(), tool.outer_data());

			shadow_->SetRoundType(*request.round_type());

			Refresh();
		}

		if(request.source() == this) {
			ReportRoundTypeUpdate(request);
		}
	}

	void StaticPanel::PerformRoundRadiusUpdate (
	        const RoundRadiusUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(size(), DefaultBorderWidth(), round_type(), *request.round_radius());

			inner_->bind();
			inner_->set_sub_data(0, tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_sub_data(0, tool.outer_size(), tool.outer_data());

			shadow_->SetRadius(*request.round_radius());
			Refresh();
		}

		if(request.source() == this) {
			ReportRoundRadiusUpdate(request);
		}
	}

	ResponseType StaticPanel::Draw (Profile& profile)
	{
		shadow_->Draw(glm::vec3(position().x(), position().y(), 0.f));

		if(refresh_) {
			RenderToBuffer(profile);
			refresh_ = false;
		}

		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		tex_buffer_.Draw(position().x(), position().y());

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return Accept;
	}

	ResponseType StaticPanel::MousePressEvent (const MouseEvent& event)
	{
		if(container() == event.section()) {
			if(event.section()->last_hover_widget() == this ||
					event.section()->last_hover_widget() == decoration()) {
				MoveToLast();

				last_position_ = position();
				cursor_position_ = event.position();
				pressed_ = true;

				event.context()->SetFocusedWidget(this);

				return Accept;
			}
		}

		return Ignore;
	}

	ResponseType StaticPanel::MouseReleaseEvent (const MouseEvent& event)
	{
		if(pressed_) {

			if(realign_) {
				DBG_PRINT_MSG("%s", "now fill subwidgets");
				FillSubWidgets(position(), size(), margin());
			}

			realign_ = false;

		}

		pressed_ = false;
		return Accept;
	}

	ResponseType StaticPanel::MouseMoveEvent (const MouseEvent& event)
	{
		if(pressed_) {

			int offset_x = event.position().x() - cursor_position_.x();
			int offset_y = event.position().y() - cursor_position_.y();

			SetPosition(last_position_.x() + offset_x,
					last_position_.y() + offset_y);

			realign_ = true;

			return Accept;
		}

		return Ignore;
	}

	void StaticPanel::InitializeStaticPanelOnce ()
	{
		VertexTool tool;
		tool.GenerateVertices (size(), DefaultBorderWidth(), round_type(), round_radius());

		glGenVertexArrays(2, vao_);
		glBindVertexArray(vao_[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(Shaders::instance->location(Stock::TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::TRIANGLE_COORD), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);

		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());

		glEnableVertexAttribArray(Shaders::instance->location(Stock::TRIANGLE_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::TRIANGLE_COORD), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();

		shadow_ = new Shadow(size(), round_type(), round_radius());
	}

	void StaticPanel::RenderToBuffer (Profile& profile)
	{
		GLsizei width = size().width();
		GLsizei height = size().height();

		GLfloat left = position().x();
		GLfloat bottom = position().y();

		GLfloat right = left + width;
		GLfloat top = bottom + height;

		tex_buffer_.SetCoord(0.f, 0.f, size().width(), size().height());
		// Create and set texture to render to.
		GLTexture2D* tex = tex_buffer_.texture();
		if(!tex->texture())
			tex->generate();

		tex->bind();
		tex->SetWrapMode(GL_REPEAT, GL_REPEAT);
		tex->SetMinFilter(GL_NEAREST);
		tex->SetMagFilter(GL_NEAREST);
		tex->SetImage(0, GL_RGBA, size().width(), size().height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
		GLFramebuffer* fb = new GLFramebuffer;
		fb->generate();
		fb->bind();

		// Set "renderedTexture" as our colour attachement #0
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D, tex->texture(), 0);
		//fb->Attach(*tex, GL_COLOR_ATTACHMENT0);

		// Critical: Create a Depth_STENCIL renderbuffer for this off-screen rendering
		GLuint rb;
		glGenRenderbuffers(1, &rb);

		glBindRenderbuffer(GL_RENDERBUFFER, rb);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL,
				size().width(), size().height());
		//Attach depth buffer to FBO
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				GL_RENDERBUFFER, rb);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
				GL_RENDERBUFFER, rb);

		if(GLFramebuffer::CheckStatus()) {

			fb->bind();

			glClearColor(0.f, 0.f, 0.f, 0.f);
			glClearDepth(1.0);
			glClearStencil(0);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);

			glm::mat4 origin;
			glGetUniformfv(Shaders::instance->triangle_program()->id(),
					Shaders::instance->location(Stock::TRIANGLE_PROJECTION),
					glm::value_ptr(origin));

			glm::mat4 projection = glm::ortho(left, right, bottom, top, 100.f,
			        -100.f);

			Shaders::instance->SetUIProjectionMatrix(projection);

			RefPtr<GLSLProgram> program =
			        Shaders::instance->triangle_program();
			program->use();
			glUniformMatrix4fv(Shaders::instance->location(Stock::TRIANGLE_PROJECTION), 1, GL_FALSE,
			        glm::value_ptr(projection));
//			program = Shaders::instance->widget_program();
//			program->use();
//			glUniformMatrix4fv(Shaders::instance->location(Stock::WIDGET_PROJECTION), 1, GL_FALSE,
//					glm::value_ptr(projection));
			program = Shaders::instance->text_program();
			program->use();
			glUniformMatrix4fv(Shaders::instance->location(Stock::TEXT_PROJECTION), 1, GL_FALSE,
			        glm::value_ptr(projection));
			program = Shaders::instance->image_program();
			program->use();
			glUniformMatrix4fv(Shaders::instance->location(Stock::IMAGE_PROJECTION), 1, GL_FALSE,
			        glm::value_ptr(projection));

            GLint vp[4];
            glGetIntegerv(GL_VIEWPORT, vp);
			glViewport(0, 0, size().width(), size().height());

			// Draw frame panel
			program = Shaders::instance->triangle_program();
			program->use();

			glUniform3f(Shaders::instance->location(Stock::TRIANGLE_POSITION), (float) position().x(), (float) position().y(), 0.f);
			glUniform1i(Shaders::instance->location(Stock::TRIANGLE_GAMMA), 0);
			glUniform1i(Shaders::instance->location(Stock::TRIANGLE_ANTI_ALIAS), 0);

			glVertexAttrib4fv(Shaders::instance->location(Stock::TRIANGLE_COLOR), Theme::instance->tooltip().inner.data());

			glBindVertexArray(vao_[0]);
			glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

			glVertexAttrib4fv(Shaders::instance->location(Stock::TRIANGLE_COLOR), Theme::instance->tooltip().outline.data());
			glUniform1i(Shaders::instance->location(Stock::TRIANGLE_ANTI_ALIAS), 1);

			glBindVertexArray(vao_[1]);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
			        GetOutlineVertices(round_type()) * 2 + 2);

			glBindVertexArray(0);
			program->reset();

			Profile off_screen_profile(position());

			for(AbstractWidget* p = first(); p; p = p->next()) {
				DispatchDrawEvent(p, off_screen_profile);
			}

			// Restore the viewport setting and projection matrix
			glViewport(vp[0], vp[1], vp[2], vp[3]);

			Shaders::instance->SetUIProjectionMatrix(origin);

			program = Shaders::instance->triangle_program();
			program->use();
			glUniformMatrix4fv(Shaders::instance->location(Stock::TRIANGLE_PROJECTION), 1, GL_FALSE,
					glm::value_ptr(origin));
//			program = Shaders::instance->widget_program();
//			program->use();
//			glUniformMatrix4fv(Shaders::instance->location(Stock::WIDGET_PROJECTION), 1, GL_FALSE,
//					glm::value_ptr(origin));
			program = Shaders::instance->text_program();
			program->use();
			glUniformMatrix4fv(Shaders::instance->location(Stock::TEXT_PROJECTION), 1, GL_FALSE,
					glm::value_ptr(origin));
			program = Shaders::instance->image_program();
			program->use();
			glUniformMatrix4fv(Shaders::instance->location(Stock::IMAGE_PROJECTION), 1, GL_FALSE,
					glm::value_ptr(origin));

			program->reset();

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		}

		fb->reset();
		tex->reset();

		//delete tex; tex = 0;

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glDeleteRenderbuffers(1, &rb);

		fb->reset();
		delete fb; fb = 0;
	}

}

