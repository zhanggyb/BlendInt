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
#include <glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <assert.h>
#include <algorithm>

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/OpenGL/GLFramebuffer.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

#include <BlendInt/Gui/FramePanel.hpp>
#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	FramePanel::FramePanel()
	: Frame(), m_refresh(true)
	{
		set_drop_shadow(true);
		InitializeFramePanel();
	}
	
	FramePanel::~FramePanel ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void FramePanel::PerformRefresh(const RefreshRequest& request)
	{
		m_refresh = true;
		ReportRefresh(request);
	}

	void FramePanel::PerformSizeUpdate(const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.Setup(*request.size(), 0, RoundNone, 0);
			m_inner->Bind();
			tool.SetInnerBufferData(m_inner.get());
			m_inner->Reset();
		}

		Frame::PerformSizeUpdate(request);
	}

	ResponseType FramePanel::Draw (const RedrawEvent& event)
	{
		if(m_refresh) {

			RenderToBuffer();

			m_refresh = false;
		}

		//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		m_buffer.Draw(position().x(), position().y());

		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return Accept;
	}

	void FramePanel::InitializeFramePanel()
	{
		glGenVertexArrays(1, &m_vao);

		glBindVertexArray(m_vao);
		VertexTool tool;
		tool.Setup(size(), 0, RoundNone, 0);

		m_inner.reset(new GLArrayBuffer);
		m_inner->Generate();
		m_inner->Bind();
		tool.SetInnerBufferData(m_inner.get());

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		m_inner->Reset();

	}

	void FramePanel::RenderToBuffer ()
	{
		using Stock::Shaders;

		GLsizei width = size().width();
		GLsizei height = size().height();
		GLfloat left = position().x();
		GLfloat bottom = position().y();

		GLfloat right = left + width;
		GLfloat top = bottom + height;

		m_buffer.SetCoord(0.f, 0.f, width, height);
		// Create and set texture to render to.
		GLTexture2D* tex = m_buffer.texture();
		if(!tex->texture())
			tex->Generate();

		tex->Bind();
		tex->SetWrapMode(GL_REPEAT, GL_REPEAT);
		tex->SetMinFilter(GL_NEAREST);
		tex->SetMagFilter(GL_NEAREST);
		tex->SetImage(0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
		GLFramebuffer* fb = new GLFramebuffer;
		fb->Generate();
		fb->Bind();

		// Set "renderedTexture" as our colour attachement #0
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D, tex->texture(), 0);
		//fb->Attach(*tex, GL_COLOR_ATTACHMENT0);

		GLuint rb = 0;
		glGenRenderbuffers(1, &rb);

		glBindRenderbuffer(GL_RENDERBUFFER, rb);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
				width, height);

		//Attach depth buffer to FBO
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				GL_RENDERBUFFER, rb);

		if(GLFramebuffer::CheckStatus()) {

			fb->Bind();

			glClearColor(0.0, 0.0, 0.0, 0.0);

			glClearDepth(1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			//glEnable(GL_BLEND);

			glm::mat4 origin;
			glm::mat4 projection = glm::ortho(left, right, bottom, top, 100.f,
			        -100.f);

			RefPtr<GLSLProgram> program =
			        Shaders::instance->triangle_program();
			program->GetUniformfv("u_projection", glm::value_ptr(origin));
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
			        glm::value_ptr(projection));
			program = Shaders::instance->line_program();
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
			        glm::value_ptr(projection));
			program = Shaders::instance->text_program();
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
			        glm::value_ptr(projection));
			program = Shaders::instance->image_program();
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
			        glm::value_ptr(projection));

			RedrawEvent event;
			ScissorStatus scissor;

            GLint vp[4];
            glGetIntegerv(GL_VIEWPORT, vp);
			glViewport(0, 0, width, height);

			// Draw frame panel
			program = Shaders::instance->triangle_program();
			program->Use();

			program->SetUniform3f("u_position", (float)position().x(), (float)position().y(), 0.f);
			program->SetVertexAttrib4f("a_color", 0.447f, 0.447f, 0.447f, 1.0f);
			program->SetUniform1i("u_gamma", 0);
			program->SetUniform1i("u_AA", 0);

			glBindVertexArray(m_vao);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
			glBindVertexArray(0);

			if(deque().size()) {
				Section::DispatchDrawEvent(deque()[0], event, scissor);
			}

			// Restore the viewport setting and projection matrix
			glViewport(vp[0], vp[1], vp[2], vp[3]);
			program = Shaders::instance->triangle_program();
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
					glm::value_ptr(origin));
			program = Shaders::instance->line_program();
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
					glm::value_ptr(origin));
			program = Shaders::instance->text_program();
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
					glm::value_ptr(origin));
			program = Shaders::instance->image_program();
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
					glm::value_ptr(origin));

			program->Reset();
		}

		fb->Reset();
		tex->Reset();

		//delete tex; tex = 0;

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glDeleteRenderbuffers(1, &rb);

		fb->Reset();
		delete fb; fb = 0;

	}

}

