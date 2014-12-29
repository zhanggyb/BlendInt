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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/Panel.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

#include <BlendInt/Gui/AbstractFrame.hpp>

#include <BlendInt/OpenGL/GLFramebuffer.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Panel::Panel ()
	: Widget(),
	  layout_(0)
	{
		set_size(400, 300);
		set_refresh(true);

		InitializeFrame();
	}

	Panel::~Panel ()
	{
		glDeleteVertexArrays(3, vao_);
	}

	void Panel::SetLayout(AbstractLayout* layout)
	{
		if((layout == 0) || (layout == layout_)) return;

		if(layout_) {
			layout_->destroyed().disconnectOne(this, &Panel::OnLayoutDestroyed);
		}

		for(AbstractView* p = first_subview(); p; p->next_view()) {
			layout->AddWidget(dynamic_cast<AbstractWidget*>(p));
		}

		if(PushBackSubView(layout)) {
			layout_ = layout;
			events()->connect(layout_->destroyed(), this, &Panel::OnLayoutDestroyed);
			MoveSubViewTo(layout_, 0, 0);
			ResizeSubView(layout_, size());
		} else {
			DBG_PRINT_MSG("Warning: %s", "Fail to set layout");
		}

		RequestRedraw();
	}

	void Panel::AddWidget(AbstractWidget* widget)
	{
		if(layout_) {
			layout_->AddWidget(widget);
		} else {
			PushBackSubView(widget);
		}

		RequestRedraw();
	}

	void Panel::InsertWidget(int index, AbstractWidget* widget)
	{
		if(layout_) {
			layout_->InsertWidget(index, widget);
		} else {
			InsertSubView(index, widget);
		}

		RequestRedraw();
	}

	bool Panel::IsExpandX() const
	{
		if(layout_) {
			return layout_->IsExpandX();
		}

		for(AbstractView* p = first_subview(); p; p = p->next_view()) {
			if(p->IsExpandX()) return true;
		}

		return false;
	}

	bool Panel::IsExpandY() const
	{
		if(layout_) {
			return layout_->IsExpandY();
		}

		for(AbstractView* p = first_subview(); p; p = p->next_view()) {
			if(p->IsExpandY()) return true;
		}

		return false;
	}

	Size Panel::GetPreferredSize() const
	{
		Size prefer_size;

		if(subs_count() == 0) {
			prefer_size.reset(400, 300);
		} else {

			if(layout_) {
				assert(subs_count() == 1);
				prefer_size = layout_->GetPreferredSize();
			} else {

				int minx = 0;
				int miny = 0;
				int maxx = 0;
				int maxy = 0;

				for(AbstractView* p = first_subview(); p; p = p->next_view()) {
					minx = std::min(minx, p->position().x());
					miny = std::min(miny, p->position().y());
					maxx = std::max(maxx, p->position().x() + p->size().width());
					maxy = std::max(maxy, p->position().y() + p->size().height());
				}

				prefer_size.reset(maxx - minx, maxy - miny);
			}
		}

		return prefer_size;
	}

	void Panel::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {

			set_size(*request.size());

			std::vector<GLfloat> inner_verts;
			std::vector<GLfloat> outer_verts;

			if (Theme::instance->regular().shaded) {
				GenerateRoundedVertices(Vertical,
						Theme::instance->regular().shadetop,
						Theme::instance->regular().shadedown,
						&inner_verts,
						&outer_verts);
			} else {
				GenerateRoundedVertices(&inner_verts, &outer_verts);
			}

			buffer_.bind(0);
			buffer_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
			buffer_.bind(1);
			buffer_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

			buffer_.bind(2);
			float* ptr = (float*)buffer_.map();
			*(ptr + 4) = (float)size().width();
			*(ptr + 9) = (float)size().height();
			*(ptr + 12) = (float)size().width();
			*(ptr + 13) = (float)size().height();
			buffer_.unmap();

			buffer_.reset();

			if(layout_) {
				ResizeSubView(layout_, size());
			}

			RequestRedraw();
		}

		if(request.source() == this) {
			ReportSizeUpdate(request);
		}
	}

	void Panel::PerformRoundTypeUpdate(int round_type)
	{
		set_round_type(round_type);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (Theme::instance->regular().shaded) {
			GenerateRoundedVertices(Vertical,
					Theme::instance->regular().shadetop,
					Theme::instance->regular().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		buffer_.reset();

		RequestRedraw();
	}

	void Panel::PerformRoundRadiusUpdate(float radius)
	{
		set_round_radius(radius);

		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (Theme::instance->regular().shaded) {
			GenerateRoundedVertices(Vertical,
					Theme::instance->regular().shadetop,
					Theme::instance->regular().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		buffer_.reset();

		RequestRedraw();
	}

	ResponseType Panel::Draw (Profile& profile)
	{
		if(refresh()) {
			//DBG_PRINT_MSG("%s", "refresh once");
			RenderToBuffer(profile);
		}

        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        Shaders::instance->widget_image_program()->use();

        texture_buffer_.bind();
        glUniform2f(Shaders::instance->location(Stock::WIDGET_IMAGE_POSITION), 0.f, 0.f);
        glUniform1i(Shaders::instance->location(Stock::WIDGET_IMAGE_TEXTURE), 0);
        glUniform1i(Shaders::instance->location(Stock::WIDGET_IMAGE_GAMMA), 0);

        glBindVertexArray(vao_[2]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        texture_buffer_.reset();
        GLSLProgram::reset();

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        return Finish;

	}

	void Panel::InitializeFrame ()
	{
		std::vector<GLfloat> inner_verts;
		std::vector<GLfloat> outer_verts;

		if (Theme::instance->regular().shaded) {
			GenerateRoundedVertices(Vertical,
					Theme::instance->regular().shadetop,
					Theme::instance->regular().shadedown,
					&inner_verts,
					&outer_verts);
		} else {
			GenerateRoundedVertices(&inner_verts, &outer_verts);
		}

		glGenVertexArrays(3, vao_);
		buffer_.generate ();

		glBindVertexArray(vao_[0]);

		buffer_.bind(0);
		buffer_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_INNER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_INNER_COORD), 3,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);
		buffer_.bind(1);
		buffer_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
		glEnableVertexAttribArray(Shaders::instance->location(Stock::WIDGET_OUTER_COORD));
		glVertexAttribPointer(Shaders::instance->location(Stock::WIDGET_OUTER_COORD), 2,
				GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[2]);
		GLfloat vertices[] = {
				// coord											uv
				0.f, 0.f,											0.f, 0.f,
				(float)size().width(), 0.f,							1.f, 0.f,
				0.f, (float)size().height(),						0.f, 1.f,
				(float)size().width(), (float)size().height(),		1.f, 1.f
		};

		buffer_.bind(2);
		buffer_.set_data(sizeof(vertices), vertices);

		glEnableVertexAttribArray (
				Shaders::instance->location (Stock::WIDGET_IMAGE_COORD));
		glEnableVertexAttribArray (
				Shaders::instance->location (Stock::WIDGET_IMAGE_UV));
		glVertexAttribPointer (Shaders::instance->location (Stock::WIDGET_IMAGE_COORD),
				2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer (Shaders::instance->location (Stock::WIDGET_IMAGE_UV), 2,
				GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
				BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);
		buffer_.reset();
	}

	void Panel::RenderToBuffer(Profile& profile)
	{
        // Create and set texture to render to.
        GLTexture2D* tex = &texture_buffer_;
        if(!tex->id())
            tex->generate();

        tex->bind();
        tex->SetWrapMode(GL_REPEAT, GL_REPEAT);
        tex->SetMinFilter(GL_NEAREST);
        tex->SetMagFilter(GL_NEAREST);
        tex->SetImage(0, GL_RGBA, size().width(), size().height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        GLint current_framebuffer = 0;
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &current_framebuffer);

        // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
        GLuint fbo = 0;
		glGenFramebuffers (1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

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

            GLint vp[4];
            glGetIntegerv(GL_VIEWPORT, vp);

			GLboolean scissor_test;
			glGetBooleanv(GL_SCISSOR_TEST, &scissor_test);

            glm::vec2 pos = get_relative_position(Shaders::instance->widget_model_matrix());
			Profile off_screen_profile(profile, pos.x, pos.y);

			Shaders::instance->PushWidgetModelMatrix();
			Shaders::instance->PushWidgetProjectionMatrix();

			glm::mat3 identity(1.f);
			Shaders::instance->SetWidgetModelMatrix(identity);

			glm::mat4 projection = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f,
			        -100.f);
			Shaders::instance->SetWidgetProjectionMatrix(projection);

			glBindFramebuffer(GL_FRAMEBUFFER, fbo);

            glClearColor(0.f, 0.f, 0.f, 0.f);
            glClearDepth(1.0);
            glClearStencil(0);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);

            glViewport(0, 0, size().width(), size().height());
			glDisable(GL_SCISSOR_TEST);

			DrawPanel();

            // Draw context:
			DrawSubViewsOnce(profile);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // restore viewport and framebuffer

			Shaders::instance->PopWidgetProjectionMatrix();
			Shaders::instance->PopWidgetModelMatrix();

			if(scissor_test) {
				glEnable(GL_SCISSOR_TEST);
			}

			glViewport(vp[0], vp[1], vp[2], vp[3]);

        }

        tex->reset();

        //delete tex; tex = 0;

        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glDeleteRenderbuffers(1, &rb);

        glBindFramebuffer(GL_FRAMEBUFFER, current_framebuffer);
		glDeleteFramebuffers(1, &fbo);
	}

	void Panel::DrawPanel()
	{
		Shaders::instance->widget_inner_program()->use();

		glUniform1i(Shaders::instance->location(Stock::WIDGET_INNER_GAMMA), 0);

		glUniform4fv(Shaders::instance->location(Stock::WIDGET_INNER_COLOR), 1,
				Theme::instance->regular().inner.data());

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		Shaders::instance->widget_outer_program()->use();

		glUniform2f(Shaders::instance->location(Stock::WIDGET_OUTER_POSITION), 0.f, 0.f);
		glUniform4fv(Shaders::instance->location(Stock::WIDGET_OUTER_COLOR), 1,
		        Theme::instance->regular().outline.data());

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		        GetOutlineVertices(round_type()) * 2 + 2);

		if (emboss()) {
			glUniform4f(Shaders::instance->location(Stock::WIDGET_OUTER_COLOR), 1.0f,
			        1.0f, 1.0f, 0.16f);
			glUniform2f(Shaders::instance->location(Stock::WIDGET_OUTER_POSITION),
			        0.f, - 1.f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0,
			        GetHalfOutlineVertices(round_type()) * 2);
		}

		glBindVertexArray(0);
		GLSLProgram::reset();
	}

	void Panel::OnLayoutDestroyed(AbstractWidget* layout)
	{
#ifdef DEBUG
		assert(layout == layout_);
#endif

		DBG_PRINT_MSG("layout %s is destroyed", layout->name().c_str());
		layout->destroyed().disconnectOne(this, &Panel::OnLayoutDestroyed);
		layout_ = 0;
	}

} /* namespace BlendInt */
