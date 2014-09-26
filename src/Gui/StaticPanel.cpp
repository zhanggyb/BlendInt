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
	: AbstractPanel()
	{
		//set_margin(10, 10, 10, 10);
		set_round_type(RoundAll);

		InitializeStaticPanelOnce();
	}

	StaticPanel::~StaticPanel ()
	{
		glDeleteVertexArrays(2, vao_);
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

			FillSubWidgets(*request.size(), margin());

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

			Refresh();
		}

		if(request.source() == this) {
			ReportRoundRadiusUpdate(request);
		}
	}

	ResponseType StaticPanel::Draw (Profile& profile)
	{
		if(refresh()) RenderToBuffer();

		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		tex_buffer_.Draw(0.f, 0.f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return Accept;
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
	}

	void StaticPanel::RenderToBuffer ()
	{
		GLsizei width = size().width();
		GLsizei height = size().height();

		tex_buffer_.SetCoord(0.f, 0.f, (float)width, (float)height);
		// Create and set texture to render to.
		GLTexture2D* tex = tex_buffer_.texture();
		if(tex->id() == 0)
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
				GL_TEXTURE_2D, tex->id(), 0);
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

			Profile off_screen_profile;

			glm::mat4 identity(1.f);
			Shaders::instance->PushUIModelMatrix();
			Shaders::instance->SetUIModelMatrix(identity);

			glClearColor(0.f, 0.f, 0.f, 0.f);
			glClearDepth(1.0);
			glClearStencil(0);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);

			glm::mat4 projection = glm::ortho(0.f, (float)width, 0.f, (float)height, 100.f,
			        -100.f);

			Shaders::instance->PushUIProjectionMatrix();
			Shaders::instance->SetUIProjectionMatrix(projection);

            GLint vp[4];
            glGetIntegerv(GL_VIEWPORT, vp);
			glViewport(0, 0, width, height);

			GLboolean scissor_test;
			glGetBooleanv(GL_SCISSOR_TEST, &scissor_test);
			glDisable(GL_SCISSOR_TEST);

			// Draw frame panel
			Shaders::instance->triangle_program()->use();

			glUniform3f(Shaders::instance->location(Stock::TRIANGLE_POSITION), 0.f, 0.f, 0.f);
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
			GLSLProgram::reset();

			for(AbstractWidget* p = first(); p; p = p->next()) {
				DispatchDrawEvent(p, off_screen_profile);
			}

			// Restore the viewport setting and projection matrix
			glViewport(vp[0], vp[1], vp[2], vp[3]);

			Shaders::instance->PopUIProjectionMatrix();
			Shaders::instance->PopUIModelMatrix();

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			if(scissor_test) {
				glEnable(GL_SCISSOR_TEST);
			}
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

