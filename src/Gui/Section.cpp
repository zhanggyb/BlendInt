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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/OpenGL/GLFramebuffer.hpp>

#include <BlendInt/Gui/Section.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	AbstractWidget* Section::iterator_ptr = 0;

	Section::Section ()
	: AbstractContainer(),
	  m_focused_widget(0),
	  last_hover_widget_(0)
	{
		set_size(800, 600);
	}

	Section::~Section ()
	{
		for(AbstractWidget* p = first(); p; p = p->next())
		{
			p->destroyed().disconnectOne(this, &Section::OnSubWidgetDestroyedInSection);
		}

		// unset hover status
		ClearHoverWidgets();

		// avoid calling deleted Section pointer in Context::MousePressEvent
		if(iterator_ptr == this) {
			iterator_ptr = 0;
		}
	}

	void Section::PushFront(AbstractWidget* widget)
	{
		if(PushFrontSubWidget(widget)) {

			EnableShadow(widget);
			events()->connect(widget->destroyed(), this, &Section::OnSubWidgetDestroyedInSection);

		}
	}

	void Section::PushBack (AbstractWidget* widget)
	{
		if(PushBackSubWidget(widget)) {

			EnableShadow(widget);
			events()->connect(widget->destroyed(), this, &Section::OnSubWidgetDestroyedInSection);

		}
	}

	void Section::Remove (AbstractWidget* widget)
	{
		if(RemoveSubWidget(widget)) {

			widget->destroyed().disconnectOne(this, &Section::OnSubWidgetDestroyedInSection);

			if(widget == last_hover_widget_) {

				assert(widget->hover());

				widget->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);
				widget->set_hover(false);
				widget = 0;

			}

			if(first() == 0) {

				if(managed() && (reference_count() == 0)) {
					DBG_PRINT_MSG("no sub widgets, delete this section: %s", name().c_str());
					delete this;
				} else {
					DBG_PRINT_MSG("Warning: %s", "the section is empty but it's not set managed"
							", it will not be deleted automatically");
				}

			}

		}
	}

	bool Section::Contain (const Point& point) const
	{
		return true;
	}

	Section* Section::GetSection (AbstractWidget* widget)
	{
		AbstractContainer* container = widget->container();
		AbstractWidget* section = 0;

		if(container == 0) {
			return dynamic_cast<Section*>(widget);
		} else {

			while(container->container()) {
				section = container;
				container = container->container();
			}

		}

		return dynamic_cast<Section*>(section);
	}

	void Section::RenderToTexture (AbstractWidget* widget, GLTexture2D* texture)
	{
		using Stock::Shaders;

#ifdef DEBUG
		assert(widget && texture);
#endif

		GLsizei width = widget->size().width();
		GLsizei height = widget->size().height();
		GLfloat left = widget->position().x();
		GLfloat bottom = widget->position().y();
		if(widget->shadow_) {
			width += Theme::instance->shadow_width() * 2;
			height += Theme::instance->shadow_width() * 2;
			left -= Theme::instance->shadow_width();
			bottom -= Theme::instance->shadow_width();
		}
		GLfloat right = left + width;
		GLfloat top = bottom + height;

		// Create and set texture to render to.
		GLTexture2D* tex = texture;
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

			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_BLEND);

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

			Profile event;
			ScissorStatus scissor;

            GLint vp[4];
            glGetIntegerv(GL_VIEWPORT, vp);
			glViewport(0, 0, width, height);

			DispatchDrawEvent(widget, event, scissor);

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

	void Section::RenderToFile (AbstractWidget* widget, const char* filename)
	{
		using Stock::Shaders;

#ifdef DEBUG
		assert(widget);
#endif

		GLsizei width = widget->size().width();
		GLsizei height = widget->size().height();
		GLfloat left = widget->position().x();
		GLfloat bottom = widget->position().y();
		if(widget->shadow_) {
			width += Theme::instance->shadow_width() * 2;
			height += Theme::instance->shadow_width() * 2;
			left -= Theme::instance->shadow_width();
			bottom -= Theme::instance->shadow_width();
		}
		GLfloat right = left + width;
		GLfloat top = bottom + height;

		// Create and set texture to render to.
		GLTexture2D* tex = new GLTexture2D;
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

			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
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

			Profile event;
			ScissorStatus scissor;

            GLint vp[4];
            glGetIntegerv(GL_VIEWPORT, vp);
			glViewport(0, 0, width, height);

			DispatchDrawEvent(widget, event, scissor);

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
			// ---------------------------------------------

			tex->Bind();	// make sure bind again as the current texture may be changed in the draw event
			tex->WriteToFile(filename);
		}

		fb->Reset();

		tex->Reset();
		delete tex; tex = 0;

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glDeleteRenderbuffers(1, &rb);

		fb->Reset();
		delete fb; fb = 0;
	}

	void Section::PerformPositionUpdate (const PositionUpdateRequest& request)
	{
		if (request.target() == this) {
			set_position (*request.position());
		}

		ReportPositionUpdate(request);
	}

	void Section::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			set_size (*request.size());
		}

		if (request.source()->container() == this) {
			if (request.source()->drop_shadow() && request.source()->shadow_) {
				request.source()->shadow_->Resize(*request.size());
			}
		}

		ReportSizeUpdate(request);
	}

	void Section::PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request)
	{
		if (request.source()->container() == this) {
			if (request.source()->drop_shadow() && request.source()->shadow_) {
				request.source()->shadow_->SetRoundType(*request.round_type());
			}
		}

		ReportRoundTypeUpdate(request);
	}

	void Section::PerformRoundRadiusUpdate (
	        const RoundRadiusUpdateRequest& request)
	{
		if (request.source()->container() == this) {
			if (request.source()->drop_shadow() && request.source()->shadow_) {
				request.source()->shadow_->SetRadius(*request.round_radius());
			}
		}

		ReportRoundRadiusUpdate(request);
	}

	ResponseType Section::Draw (const Profile& profile)
	{
		const_cast<Profile&>(profile).section_ = this;

		for(AbstractWidget* p = first(); p; p = p->next())
		{
			DispatchDrawEvent(p, profile);
		}

		return Accept;
	}

	ResponseType Section::FocusEvent (bool focus)
	{
		return Ignore;
	}

	ResponseType Section::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Section::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;
	}

	ResponseType Section::ContextMenuPressEvent (
	        const ContextMenuEvent& event)
	{
		const_cast<ContextMenuEvent&>(event).m_section = this;

		return Ignore;
	}

	ResponseType Section::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		const_cast<ContextMenuEvent&>(event).m_section = this;

		return Ignore;
	}

	ResponseType Section::MousePressEvent (const MouseEvent& event)
	{
		const_cast<MouseEvent&>(event).m_section = this;

		CheckAndUpdateHoverWidget(event);

		if(last_hover_widget_) {

			/*
			{
				DBG_PRINT_MSG("print hover widgets in section %s", name().c_str());
				AbstractWidget* hover_widget = m_last_hover_widget;

				while(hover_widget != this) {
					DBG_PRINT_MSG("\t%s", hover_widget->name().c_str());
					hover_widget = hover_widget->container();
				}
			}
			*/

			return DispatchMousePressEvent(last_hover_widget_, event);

		} else {
			return Ignore;
		}
	}

	ResponseType Section::MouseReleaseEvent (const MouseEvent& event)
	{
		const_cast<MouseEvent&>(event).m_section = this;

		CheckAndUpdateHoverWidget(event);

		if(last_hover_widget_) {

			/*
			{
				DBG_PRINT_MSG("print hover widgets in section %s", name().c_str());
				AbstractWidget* hover_widget = m_last_hover_widget;

				while(hover_widget != this) {
					DBG_PRINT_MSG("\t%s", hover_widget->name().c_str());
					hover_widget = hover_widget->container();
				}
			}
			*/

			return DispatchMouseReleaseEvent(last_hover_widget_, event);

		} else {
			return Ignore;
		}
	}

	ResponseType Section::MouseMoveEvent (const MouseEvent& event)
	{
		const_cast<MouseEvent&>(event).m_section = this;

		CheckAndUpdateHoverWidget(event);

		if(last_hover_widget_) {
			return last_hover_widget_->MouseMoveEvent(event);
		}

		return Ignore;
	}

	void Section::DispatchDrawEvent (AbstractWidget* widget,
	        const Profile& profile, ScissorStatus& scissor)
	{
		if (widget->visiable()) {

			if(widget->drop_shadow() && widget->shadow_) {
				widget->shadow_->Draw(glm::vec3(widget->position().x(), widget->position().y(), 0.f));
			}

			ResponseType response = widget->Draw(profile);
			if(response == Accept) return;

			AbstractContainer* parent = dynamic_cast<AbstractContainer*>(widget);
			if (parent) {

				if(parent->scissor_test()) {
					scissor.Push(parent->position().x() + parent->margin().left(),
							parent->position().y() + parent->margin().right(),
							parent->size().width() - parent->margin().left() - parent->margin().right(),
							parent->size().height() - parent->margin().top() - parent->margin().bottom());
				}

				if(scissor.valid()) {
					scissor.Enable();

					for(AbstractWidget* sub = parent->first(); sub; sub = sub->next())
					{
						DispatchDrawEvent(sub, profile, scissor);
					}

				} else {

					for(AbstractWidget* sub = parent->first(); sub; sub = sub->next())
					{
						DispatchDrawEvent(sub, profile, scissor);
					}
				}

				if(parent->scissor_test()) {
					scissor.Pop();
					scissor.Disable();
				}

			}

		}
	}

	void Section::DispatchDrawEvent (AbstractWidget* widget,
	        const Profile& profile)
	{
		if (widget && widget->visiable()) {

			if(widget->drop_shadow() && widget->shadow_) {
				widget->shadow_->Draw(glm::vec3(widget->position().x(), widget->position().y(), 0.f));
			}

			ResponseType response = widget->Draw(profile);
			if(response == Accept) return;

			AbstractContainer* parent = dynamic_cast<AbstractContainer*>(widget);
			if (parent) {

				if(parent->scissor_test()) {
					m_scissor_status.Push(parent->position().x() + parent->margin().left(),
							parent->position().y() + parent->margin().right(),
							parent->size().width() - parent->margin().left() - parent->margin().right(),
							parent->size().height() - parent->margin().top() - parent->margin().bottom());
				}

				if(m_scissor_status.valid()) {
					m_scissor_status.Enable();

					for(AbstractWidget* sub = parent->first(); sub; sub = sub->next())
					{
						DispatchDrawEvent(sub, profile);
					}

				} else {

					for(AbstractWidget* sub = parent->first(); sub; sub = sub->next())
					{
						DispatchDrawEvent(sub, profile);
					}
				}

				if(parent->scissor_test()) {
					m_scissor_status.Pop();
					m_scissor_status.Disable();
				}

			}

		}
	}

	bool Section::CheckAndUpdateHoverWidget (const MouseEvent& event)
	{
		if (last_hover_widget_) {

			if (IsHoverThrough(last_hover_widget_, event.position())) {

				AbstractWidget* orig = last_hover_widget_;
				UpdateHoverWidgetSubs(event);

				if(orig != last_hover_widget_) {
					orig->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);
					events()->connect(last_hover_widget_->destroyed(), this, &Section::OnHoverWidgetDestroyed);
				}

			} else {

				last_hover_widget_->destroyed().disconnectOne(this,
				        &Section::OnHoverWidgetDestroyed);
				last_hover_widget_->set_hover(false);
				last_hover_widget_->CursorEnterEvent(false);

				// find which contianer contains cursor position
				while (last_hover_widget_->container()) {

					if (last_hover_widget_->container() == this) {	// FIXME: the widget may be mvoed to another context
						last_hover_widget_ = 0;
						break;
					} else {
						last_hover_widget_ = last_hover_widget_->container();

						if (IsHoverThrough(last_hover_widget_, event.position())) {
							break;
						}
						/*
						else {
							last_hover_widget_->destroyed().disconnectOne(this,
							        &Section::OnHoverWidgetDestroyed);
							last_hover_widget_->set_hover(false);
							last_hover_widget_->CursorEnterEvent(false);
						}
						*/
					}
				}

				if (last_hover_widget_) {
					UpdateHoverWidgetSubs(event);
					events()->connect(last_hover_widget_->destroyed(), this, &Section::OnHoverWidgetDestroyed);
				}
			}

		} else {

			for(AbstractWidget* p = first(); p; p = p->next())
			{
				if (p->Contain(event.position())) {

					//DBG_PRINT_MSG("Get hover widget: %s", (*it)->name().c_str());
					last_hover_widget_ = p;
					last_hover_widget_->set_hover(true);
					last_hover_widget_->CursorEnterEvent(true);

					UpdateHoverWidgetSubs(event);

					break;
				}
			}

			if(last_hover_widget_) {
				events()->connect(last_hover_widget_->destroyed(), this,
				        &Section::OnHoverWidgetDestroyed);
			}

		}

		return last_hover_widget_ != 0;
	}

	void Section::UpdateHoverWidgetSubs (const MouseEvent& event)
	{
		AbstractContainer* parent = dynamic_cast<AbstractContainer*>(last_hover_widget_);

		if (parent) {

			for(AbstractWidget* p = parent->first(); p; p = p->next())
			{
				if(p->Contain(event.position())) {

					last_hover_widget_ = p;
					last_hover_widget_->set_hover(true);
					last_hover_widget_->CursorEnterEvent(true);

					UpdateHoverWidgetSubs(event);
					break;
				}
			}

		}
	}

	void Section::OnSubWidgetDestroyedInSection(AbstractWidget* widget)
	{
		widget->destroyed().disconnectOne(this, &Section::OnSubWidgetDestroyedInSection);

		if(first() == 0) {

			if(managed() && (reference_count() == 0)) {
				DBG_PRINT_MSG("no sub widgets, delete this section: %s", name().c_str());
				delete this;
			} else {
				DBG_PRINT_MSG("Warning: %s", "the section is empty but it's not set managed"
						", and it's referenced by a smart pointer, it will not be deleted automatically");
			}

			return;
		}

		if((first() == widget) && (last() == widget)) {

			if(managed() && (reference_count() == 0)) {
				DBG_PRINT_MSG("the last widget is removed, delete this section: %s", name().c_str());
				delete this;
			} else {
				DBG_PRINT_MSG("Warning: %s", "the section is empty but it's not set managed"
						", and it's referenced by a smart pointer, it will not be deleted automatically");
			}

		}
	}

	ResponseType Section::DispatchMousePressEvent (AbstractWidget* widget, const MouseEvent& event)
	{
		if(widget == this) {
			return Ignore;
		} else {

			if(widget->container()) {
				if(DispatchMousePressEvent(widget->container(), event) == Ignore) {
					//DBG_PRINT_MSG("mouse press in %s and get ignore", widget->name().c_str());
					return widget->MousePressEvent(event);
				} else {
					return Accept;
				}

			} else {
				//DBG_PRINT_MSG("mouse press in %s", widget->name().c_str());
				return widget->MousePressEvent(event);
			}

		}
	}

	ResponseType Section::DispatchMouseReleaseEvent (AbstractWidget* widget, const MouseEvent& event)
	{
		if(widget == this) {
			return Ignore;
		} else {

			if(widget->container()) {
				if(DispatchMouseReleaseEvent(widget->container(), event) == Ignore) {
					//DBG_PRINT_MSG("mouse press in %s and get ignore", widget->name().c_str());
					return widget->MouseReleaseEvent(event);
				} else {
					return Accept;
				}

			} else {
				DBG_PRINT_MSG("mouse press in %s", widget->name().c_str());
				return widget->MouseReleaseEvent(event);
			}

		}
	}

	void Section::OnHoverWidgetDestroyed (AbstractWidget* widget)
	{
		assert(widget->hover());
		assert(last_hover_widget_ == widget);

		DBG_PRINT_MSG("unset hover status of widget %s", widget->name().c_str());
		widget->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);

		last_hover_widget_ = 0;
	}

	bool Section::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		return true;
	}

	bool Section::PositionUpdateTest (const PositionUpdateRequest& request)
	{
		return true;
	}

	bool Section::RoundTypeUpdateTest (const RoundTypeUpdateRequest& request)
	{
		return true;
	}

	bool Section::RoundRadiusUpdateTest (
	        const RoundRadiusUpdateRequest& request)
	{
		return true;
	}

	void Section::ClearHoverWidgets()
	{
		if(last_hover_widget_) {

			last_hover_widget_->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);

			while (last_hover_widget_ && last_hover_widget_ != this) {
				last_hover_widget_->set_hover(false);
				last_hover_widget_ = last_hover_widget_->container();
			}

			if(last_hover_widget_ == this)
				last_hover_widget_ = 0;

		}
	}

}
