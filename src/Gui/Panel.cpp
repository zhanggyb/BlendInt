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
#include <BlendInt/Gui/Panel.hpp>
#include <BlendInt/Gui/Decoration.hpp>

#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Gui/Section.hpp>

#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Panel::Panel ()
	: AbstractContainer(),
	  space_(2)
	{
		set_size(360, 360);
		set_margin(2, 2, 2, 2);

		set_drop_shadow(true);

		InitializeVirtualWindow();
	}

	Panel::~Panel ()
	{
		glDeleteVertexArrays(2, vao_);
	}

	void Panel::Setup (AbstractWidget* widget)
	{
		if(widget == 0) return;

		if(widget->container() == this) return;

		int sum = widget_count();

		if (sum > 1) {
			DBG_PRINT_MSG("TODO: %s", "delete tail widgets");

			AbstractWidget* tmp = 0;
			for(AbstractWidget* p = first()->next(); p; p = tmp)
			{
				tmp = p->next();
				if(p->managed() && (p->reference_count() == 0))
				{
					delete p;
				} else {
					DBG_PRINT_MSG("Warning: %s is not set managed and will not be deleted", p->name().c_str());
				}
			}
		}

		if(InsertSubWidget(ContentIndex, widget)) {
			FillSubWidgets(position(), size(), margin());
		}
	}

	Size Panel::GetPreferredSize () const
	{
		Size prefer;

		Size tmp;
		for(AbstractWidget* p = first(); p; p = p->next())
		{
			tmp = p->GetPreferredSize();

			prefer.set_width(std::max(prefer.width(), tmp.width()));
			prefer.add_height(tmp.height());
		}

		prefer.add_height((widget_count() - 1) * space_);

		prefer.add_width(margin().hsum());
		prefer.add_height(margin().vsum());

		return prefer;
	}

	ResponseType Panel::Draw (Profile& profile)
	{
		RefPtr<GLSLProgram> program =
						Shaders::instance->triangle_program();
		program->Use();

		glUniform3f(Shaders::instance->triangle_uniform_position(), (float) position().x(), (float) position().y(), 0.f);
		glUniform1i(Shaders::instance->triangle_uniform_gamma(), 0);
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 0);

		glVertexAttrib4f(Shaders::instance->triangle_attrib_color(), 0.447f, 0.447f, 0.447f, 1.f);

		glBindVertexArray(vao_[0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

		glVertexAttrib4f(Shaders::instance->triangle_attrib_color(), 0.8f, 0.8f, 0.8f, 0.8f);
		glUniform1i(Shaders::instance->triangle_uniform_antialias(), 1);

		glBindVertexArray(vao_[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0,
		        GetOutlineVertices(round_type()) * 2 + 2);

		glBindVertexArray(0);
		program->reset();

		return Ignore;
	}

	ResponseType Panel::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType Panel::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Panel::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Panel::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Panel::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Ignore;
	}

	ResponseType Panel::MousePressEvent (const MouseEvent& event)
	{
		if(container() == event.section()) {
			if(event.section()->last_hover_widget() == this) {
				MoveToLast();
				return Accept;
			}
		}

		return Ignore;
	}

	ResponseType Panel::MouseReleaseEvent (
	        const MouseEvent& event)
	{
		return Ignore;
	}

	ResponseType Panel::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;
	}

	void Panel::PerformPositionUpdate (const PositionUpdateRequest& request)
	{
		if(request.target() == this) {
			int x = request.position()->x() - position().x();
			int y = request.position()->y() - position().y();

			MoveSubWidgets(x, y);

			set_position(*request.position());

			Refresh();
		}

		if(request.source() != container()) {
			ReportPositionUpdate(request);
		}
	}

	void Panel::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			VertexTool tool;
			tool.GenerateVertices(*request.size(), DefaultBorderWidth(), RoundNone, 0.f);

			inner_->bind();
			inner_->set_sub_data(0, tool.inner_size(), tool.inner_data());
			outer_->bind();
			outer_->set_sub_data(0, tool.outer_size(), tool.outer_data());

			set_size(*request.size());

			FillSubWidgets(position(), *request.size(), margin());
			Refresh();
		}

		ReportSizeUpdate(request);
	}

	void Panel::PerformRoundTypeUpdate (
	        const RoundTypeUpdateRequest& request)
	{
		if(request.target() == this) {
			Refresh();
		}

		ReportRoundTypeUpdate(request);
	}

	void Panel::PerformRoundRadiusUpdate (
	        const RoundRadiusUpdateRequest& request)
	{
		if(request.target() == this) {
			Refresh();
		}

		ReportRoundRadiusUpdate(request);
	}

	void Panel::FillSubWidgets(const Point& out_pos, const Size& size, const Margin& margin)
	{
		int x = out_pos.x() + margin.left();
		int y = out_pos.y() + margin.bottom();
		int w = size.width() - margin.hsum();
		int h = size.height() - margin.vsum();

		FillSubWidgets(x, y, w, h);
	}

	void Panel::FillSubWidgets(int x, int y, int w, int h)
	{
		AbstractWidget* dec = GetWidgetAt(DecorationIndex);
		AbstractWidget* content = GetWidgetAt(ContentIndex);

		Size dec_prefer = dec->GetPreferredSize();

		y = y + h;

		if(content) {

			if(h > dec_prefer.height()) {
				ResizeSubWidget(dec, w, dec_prefer.height());
				ResizeSubWidget(content, w, h - dec_prefer.height() - space_);
			} else {
				ResizeSubWidget(dec, w, h);
				ResizeSubWidget(content, w, 0);
			}

			y = y - dec->size().height();
			SetSubWidgetPosition(dec, x, y);
			y -= space_;
			y = y - content->size().height();
			SetSubWidgetPosition(content, x, y);

		} else {

			if(h > dec_prefer.height()) {
				ResizeSubWidget(dec, w, dec_prefer.height());
			} else {
				ResizeSubWidget(dec, w, h);
			}

			y = y - dec->size().height();
			SetSubWidgetPosition(dec, x, y);
		}
	}

	void Panel::InitializeVirtualWindow ()
	{
		// set decoration
		Decoration* dec = Manage(new Decoration);
		DBG_SET_NAME(dec, "Decoration");
		PushBackSubWidget(dec);

		FillSubWidgets (position(), size(), margin());

		VertexTool tool;
		tool.GenerateVertices (size(), DefaultBorderWidth(), RoundNone, 0.f);

		glGenVertexArrays(2, vao_);
		glBindVertexArray(vao_[0]);

		inner_.reset(new GLArrayBuffer);
		inner_->generate();
		inner_->bind();
		inner_->set_data(tool.inner_size(), tool.inner_data());

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(vao_[1]);

		outer_.reset(new GLArrayBuffer);
		outer_->generate();
		outer_->bind();
		outer_->set_data(tool.outer_size(), tool.outer_data());

		glEnableVertexAttribArray(Shaders::instance->triangle_attrib_coord());
		glVertexAttribPointer(Shaders::instance->triangle_attrib_coord(), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
		GLArrayBuffer::reset();
	}

	StaticPanel::StaticPanel ()
	: Panel(),
	  refresh_(true)
	{
	}

	StaticPanel::~StaticPanel ()
	{
	}

	void StaticPanel::PerformRefresh (const RefreshRequest& request)
	{
		refresh_ = true;
		ReportRefresh(request);
	}

	void StaticPanel::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if(request.target() == this) {
			refresh_ = true;
		}

		Panel::PerformSizeUpdate(request);
	}

	ResponseType StaticPanel::Draw (Profile& profile)
	{
		if(refresh_) {
			RenderToBuffer(profile);
			refresh_ = false;
		}

		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		tex_buffer_.Draw(position().x(), position().y());

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return Accept;
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

			glClearColor(0.208f, 0.208f, 0.208f, 1.f);
			glClearDepth(1.0);
			glClearStencil(0);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);

			glm::mat4 origin;
			glGetUniformfv(Shaders::instance->triangle_program()->id(),
					Shaders::instance->triangle_uniform_projection(),
					glm::value_ptr(origin));

			glm::mat4 projection = glm::ortho(left, right, bottom, top, 100.f,
			        -100.f);

			RefPtr<GLSLProgram> program =
			        Shaders::instance->triangle_program();
			program->Use();
			glUniformMatrix4fv(Shaders::instance->triangle_uniform_projection(), 1, GL_FALSE,
			        glm::value_ptr(projection));
			program = Shaders::instance->line_program();
			program->Use();
			glUniformMatrix4fv(Shaders::instance->line_uniform_projection(), 1, GL_FALSE,
			        glm::value_ptr(projection));
			program = Shaders::instance->text_program();
			program->Use();
			glUniformMatrix4fv(Shaders::instance->text_uniform_projection(), 1, GL_FALSE,
			        glm::value_ptr(projection));
			program = Shaders::instance->image_program();
			program->Use();
			glUniformMatrix4fv(Shaders::instance->image_uniform_projection(), 1, GL_FALSE,
			        glm::value_ptr(projection));

            GLint vp[4];
            glGetIntegerv(GL_VIEWPORT, vp);
			glViewport(0, 0, size().width(), size().height());

			// Draw frame panel
			Panel::Draw(profile);

			Profile off_screen_profile(position());

			for(AbstractWidget* p = first(); p; p = p->next()) {
				DispatchDrawEvent(p, off_screen_profile);
			}

			// Restore the viewport setting and projection matrix
			glViewport(vp[0], vp[1], vp[2], vp[3]);

			program = Shaders::instance->triangle_program();
			program->Use();
			glUniformMatrix4fv(Shaders::instance->triangle_uniform_projection(), 1, GL_FALSE,
					glm::value_ptr(origin));
			program = Shaders::instance->line_program();
			program->Use();
			glUniformMatrix4fv(Shaders::instance->line_uniform_projection(), 1, GL_FALSE,
					glm::value_ptr(origin));
			program = Shaders::instance->text_program();
			program->Use();
			glUniformMatrix4fv(Shaders::instance->text_uniform_projection(), 1, GL_FALSE,
					glm::value_ptr(origin));
			program = Shaders::instance->image_program();
			program->Use();
			glUniformMatrix4fv(Shaders::instance->image_uniform_projection(), 1, GL_FALSE,
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

