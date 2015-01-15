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
#include <glm/gtx/matrix_transform_2d.hpp>

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Core/Color.hpp>

#include <BlendInt/OpenGL/GLFramebuffer.hpp>

#include <BlendInt/Gui/AbstractWidget.hpp>
#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	AbstractWidget::AbstractWidget()
	: AbstractView()
	{
		destroyed_.reset(new Cpp::Event<AbstractWidget*>);
	}

	AbstractWidget::~AbstractWidget()
	{
		destroyed_->fire(this);
	}

	bool AbstractWidget::PreDraw(const Context* context)
	{
		if(!visiable()) return false;

		//glm::mat4 model;
		//Context::shaders->GetUIModelMatrix(model);

//		Point pos = GetGlobalPosition();

		Point offset = GetOffset();

		glm::mat3 matrix = glm::translate(Context::shaders->widget_model_matrix(),
				glm::vec2(position().x() + offset.x(), position().y() + offset.y()));
//		glm::mat4 matrix = glm::translate(glm::mat4(1.f), glm::vec3(pos.x() + offset_x(), pos.y() + offset_y(), 0.f));

		Context::shaders->PushWidgetModelMatrix();
		Context::shaders->SetWidgetModelMatrix(matrix);

		return true;
	}

	void AbstractWidget::PostDraw(const Context* context)
	{
		Context::shaders->PopWidgetModelMatrix();
	}

	void AbstractWidget::PerformFocusOn (const Context* context)
	{

	}

	void AbstractWidget::PerformFocusOff (const Context* context)
	{

	}

	void AbstractWidget::PerformHoverIn(const Context* context)
	{

	}

	void AbstractWidget::PerformHoverOut(const Context* context)
	{

	}

	ResponseType AbstractWidget::PerformKeyPress (const Context* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	ResponseType AbstractWidget::PerformContextMenuPress (const Context* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	ResponseType AbstractWidget::PerformContextMenuRelease (const Context* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	ResponseType AbstractWidget::PerformMousePress (const Context* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	ResponseType AbstractWidget::PerformMouseRelease (const Context* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	ResponseType AbstractWidget::PerformMouseMove (const Context* context)
	{
		return subs_count() ? Ignore : Finish;
	}

	bool AbstractWidget::RenderSubWidgetsToTexture (
			AbstractWidget* widget,
			const Context* context,
			GLTexture2D* texture)
	{
		bool retval = false;
		assert(texture != nullptr);

        // Create and set texture to render to.
        GLTexture2D* tex = texture;
        if(!tex->id())
            tex->generate();

        tex->bind();
        tex->SetWrapMode(GL_REPEAT, GL_REPEAT);
        tex->SetMinFilter(GL_NEAREST);
        tex->SetMagFilter(GL_NEAREST);
        tex->SetImage(0, GL_RGBA, widget->size().width(), widget->size().height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

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
                              widget->size().width(), widget->size().height());
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

			Context* c = const_cast<Context*>(context);
            glm::vec3 pos = Context::shaders->widget_model_matrix() * glm::vec3(0.f, 0.f, 1.f);
            Point original = context->viewport_origin();
            c->viewport_origin_.reset(original.x() + pos.x, original.y() + pos.y);

			Context::shaders->PushWidgetModelMatrix();
			Context::shaders->PushWidgetProjectionMatrix();

			glm::mat3 identity(1.f);
			Context::shaders->SetWidgetModelMatrix(identity);

			glm::mat4 projection = glm::ortho(
				0.f, (float)widget->size().width(),
				0.f, (float)widget->size().height(),
				100.f, -100.f);
			Context::shaders->SetWidgetProjectionMatrix(projection);

			glBindFramebuffer(GL_FRAMEBUFFER, fbo);

            // in this off-screen framebuffer, a new stencil buffer was created, reset the stencil count to 0 and restore later
            GLuint original_stencil_count = c->stencil_count_;
			c->stencil_count_ = 0;

            glClearColor(0.f, 0.f, 0.f, 0.f);
            glClearDepth(1.0);
            glClearStencil(0);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            // FIXME: the blend func works abnormally in most cases.
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);

            glViewport(0, 0, widget->size().width(), widget->size().height());
			glDisable(GL_SCISSOR_TEST);

			//DrawPanel();

            // Draw context:
			widget->DrawSubViewsOnce(context);

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // restore viewport and framebuffer

			Context::shaders->PopWidgetProjectionMatrix();
			Context::shaders->PopWidgetModelMatrix();

			if(scissor_test) {
				glEnable(GL_SCISSOR_TEST);
			}

            c->viewport_origin_ = original;
			glViewport(vp[0], vp[1], vp[2], vp[3]);

			#ifdef DEBUG
			assert(c->stencil_count_ == 0);
			#endif
			c->stencil_count_ = original_stencil_count;

			retval = true;
        }

        tex->reset();

        //delete tex; tex = 0;

        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glDeleteRenderbuffers(1, &rb);
		glDeleteFramebuffers(1, &fbo);

        glBindFramebuffer(GL_FRAMEBUFFER, current_framebuffer);

		return retval;
	}

}
