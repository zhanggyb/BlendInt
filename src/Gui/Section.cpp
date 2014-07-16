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

	Section::Section ()
	: AbstractContainer(),
	  m_focused_widget(0),
	  m_last_hover_widget(0)
	{
		set_size(800, 600);
	}

	Section::~Section ()
	{
		AbstractWidget* widget = 0;

		for(std::set<AbstractWidget*>::iterator it = m_set.begin();
				it != m_set.end();
				it++)
		{
			widget = *it;

//			if(widget->focused()) {
//				assert(widget == m_focused_widget);
//				widget->set_focus(false);
//				m_focused_widget = 0;
//			}

			// disconnect events

			if(widget->hover()) {

				while (m_last_hover_widget && m_last_hover_widget != widget) {
					m_last_hover_widget->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);
					m_last_hover_widget->set_hover(false);
					m_last_hover_widget = m_last_hover_widget->container();
				}

				if(m_last_hover_widget) {
					assert(m_last_hover_widget == widget);
					m_last_hover_widget->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);
					m_last_hover_widget->set_hover(false);
				}

			}

			widget->destroyed().disconnectOne(this, &Section::OnSubWidgetDestroyed);

			widget->m_container = 0;

			if(widget->managed() && (widget->count() == 0)) {
				delete widget;
			}
		}

		m_set.clear();
	}

	void Section::Insert (AbstractWidget* widget)
	{
		if (!widget) {
			DBG_PRINT_MSG("Error: %s", "widget pointer is 0");
			return;
		}

		if (widget->container()) {
			if (widget->container() == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s",
				        widget->name().c_str(), name().c_str());
				return;
			} else {
				AbstractContainer::RemoveSubWidget(widget->container(), widget);
			}
		}

		m_set.insert(widget);
		SetContainer(widget, this);

		// set shadow
		EnableShadow(widget);

		events()->connect(widget->destroyed(), this, &Section::OnSubWidgetDestroyed);

		CheckSubWidgetAddedInContainer(widget);
	}

	void Section::Remove (AbstractWidget* widget)
	{
		RemoveSubWidget(widget);

		if(m_set.size() == 0) {

			if(managed() && (count() == 0)) {
				DBG_PRINT_MSG("no sub widgets, delete this section: %s", name().c_str());
				delete this;
			} else {
				DBG_PRINT_MSG("Warning: %s", "the section is empty but it's not set managed"
						", and it's referenced by a smart pointer, it will not be deleted automatically");
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
		if(widget->m_shadow) {
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
			        Shaders::instance->default_triangle_program();
			program->GetUniformfv("u_projection", glm::value_ptr(origin));
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
			        glm::value_ptr(projection));
			program = Shaders::instance->default_line_program();
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
			        glm::value_ptr(projection));
			program = Shaders::instance->default_text_program();
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
			        glm::value_ptr(projection));
			program = Shaders::instance->default_image_program();
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
			        glm::value_ptr(projection));

			RedrawEvent event;
			ScissorStatus scissor;

            GLint vp[4];
            glGetIntegerv(GL_VIEWPORT, vp);
			glViewport(0, 0, width, height);

			DispatchDrawEvent(widget, event, scissor);

			// Restore the viewport setting and projection matrix
			glViewport(vp[0], vp[1], vp[2], vp[3]);
			program = Shaders::instance->default_triangle_program();
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
					glm::value_ptr(origin));
			program = Shaders::instance->default_line_program();
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
					glm::value_ptr(origin));
			program = Shaders::instance->default_text_program();
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
					glm::value_ptr(origin));
			program = Shaders::instance->default_image_program();
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
		if(widget->m_shadow) {
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
			        Shaders::instance->default_triangle_program();
			program->GetUniformfv("u_projection", glm::value_ptr(origin));
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
			        glm::value_ptr(projection));
			program = Shaders::instance->default_line_program();
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
			        glm::value_ptr(projection));
			program = Shaders::instance->default_text_program();
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
			        glm::value_ptr(projection));
			program = Shaders::instance->default_image_program();
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
			        glm::value_ptr(projection));

			RedrawEvent event;
			ScissorStatus scissor;

            GLint vp[4];
            glGetIntegerv(GL_VIEWPORT, vp);
			glViewport(0, 0, width, height);

			DispatchDrawEvent(widget, event, scissor);

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// Restore the viewport setting and projection matrix
			glViewport(vp[0], vp[1], vp[2], vp[3]);
			program = Shaders::instance->default_triangle_program();
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
					glm::value_ptr(origin));
			program = Shaders::instance->default_line_program();
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
					glm::value_ptr(origin));
			program = Shaders::instance->default_text_program();
			program->Use();
			program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE,
					glm::value_ptr(origin));
			program = Shaders::instance->default_image_program();
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

	void Section::UpdateContainer (const ContainerUpdateRequest& request)
	{
		ReportContainerUpdate(request);
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
			if (request.source()->drop_shadow() && request.source()->m_shadow) {
				request.source()->m_shadow->Resize(*request.size());
			}
		}

		ReportSizeUpdate(request);
	}

	void Section::PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request)
	{
		if (request.source()->container() == this) {
			if (request.source()->drop_shadow() && request.source()->m_shadow) {
				request.source()->m_shadow->SetRoundType(*request.round_type());
			}
		}

		ReportRoundTypeUpdate(request);
	}

	void Section::PerformRoundRadiusUpdate (
	        const RoundRadiusUpdateRequest& request)
	{
		if (request.source()->container() == this) {
			if (request.source()->drop_shadow() && request.source()->m_shadow) {
				request.source()->m_shadow->SetRadius(*request.round_radius());
			}
		}

		ReportRoundRadiusUpdate(request);
	}

	ResponseType Section::Draw (const RedrawEvent& event)
	{
		const_cast<RedrawEvent&>(event).m_section = this;

		for(std::set<AbstractWidget*>::iterator it = m_set.begin(); it != m_set.end(); it++)
		{
			DispatchDrawEvent(*it, event);
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

		if(m_last_hover_widget) {

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

			return DispatchMousePressEvent(m_last_hover_widget, event);

		} else {
			return Ignore;
		}
	}

	ResponseType Section::MouseReleaseEvent (const MouseEvent& event)
	{
		const_cast<MouseEvent&>(event).m_section = this;

		CheckAndUpdateHoverWidget(event);

		if(m_last_hover_widget) {

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

			return DispatchMouseReleaseEvent(m_last_hover_widget, event);

		} else {
			return Ignore;
		}
	}

	ResponseType Section::MouseMoveEvent (const MouseEvent& event)
	{
		const_cast<MouseEvent&>(event).m_section = this;

		CheckAndUpdateHoverWidget(event);

		if(m_last_hover_widget) {
			return m_last_hover_widget->MouseMoveEvent(event);
		}

		return Ignore;
	}

	bool Section::RemoveSubWidget (AbstractWidget* widget)
	{
		if(!widget) {
			DBG_PRINT_MSG("Warning: %s", "widget pointer is 0");
			return false;
		}

		assert(widget->container() == this);

		std::set<AbstractWidget*>::iterator it = std::find(m_set.begin(), m_set.end(), widget);

		if(it == m_set.end()) {
			DBG_PRINT_MSG("Warning: object %s is not found in container %s",
					widget->name().c_str(),
					name().c_str());
			return false;
		}

		widget->destroyed().disconnectOne(this, &Section::OnSubWidgetDestroyed);
		m_set.erase(it);
		SetContainer(widget, 0);

		if(widget->hover()) {

			while (m_last_hover_widget && m_last_hover_widget != widget) {
				m_last_hover_widget->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);
				m_last_hover_widget->set_hover(false);
				m_last_hover_widget = m_last_hover_widget->container();
			}

			if(m_last_hover_widget) {
				assert(m_last_hover_widget == widget);
				m_last_hover_widget->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);
				m_last_hover_widget->set_hover(false);
				m_last_hover_widget = 0;
			}

		}

		CheckSubWidgetRemovedInContainer(widget);

		return true;
	}

	IteratorPtr Section::CreateIterator (const DeviceEvent& event)
	{
		IteratorPtr ret;
		return ret;
	}

	void Section::DispatchDrawEvent (AbstractWidget* widget,
	        const RedrawEvent& event, ScissorStatus& scissor)
	{
		if (widget->visiable()) {

			if(widget->drop_shadow() && widget->m_shadow) {
				widget->m_shadow->Draw(glm::vec3(widget->position().x(), widget->position().y(), 0.f));
			}

			ResponseType response = widget->Draw(event);
			if(response == Accept) return;

			AbstractContainer* p = dynamic_cast<AbstractContainer*>(widget);
			if (p) {

				if(p->scissor_test()) {
					scissor.Push(p->position().x() + p->margin().left(),
							p->position().y() + p->margin().right(),
							p->size().width() - p->margin().left() - p->margin().right(),
							p->size().height() - p->margin().top() - p->margin().bottom());
				}

				IteratorPtr it = p->CreateIterator(event);

				if(scissor.valid()) {
					scissor.Enable();

					for (it->GoToFirst(); !it->IsEnd(); it->GoNext()) {
						DispatchDrawEvent(it->GetWidget(), event, scissor);
					}

				} else {

					for (it->GoToFirst(); !it->IsEnd(); it->GoNext()) {
						DispatchDrawEvent(it->GetWidget(), event, scissor);
					}
				}

				if(p->scissor_test()) {
					scissor.Pop();
					scissor.Disable();
				}

			}

		}
	}

	void Section::DispatchDrawEvent (AbstractWidget* widget,
	        const RedrawEvent& event)
	{
		if (widget->visiable()) {

			if(widget->drop_shadow() && widget->m_shadow) {
				widget->m_shadow->Draw(glm::vec3(widget->position().x(), widget->position().y(), 0.f));
			}

			ResponseType response = widget->Draw(event);
			if(response == Accept) return;

			AbstractContainer* p = dynamic_cast<AbstractContainer*>(widget);
			if (p) {

				if(p->scissor_test()) {
					m_scissor_status.Push(p->position().x() + p->margin().left(),
							p->position().y() + p->margin().right(),
							p->size().width() - p->margin().left() - p->margin().right(),
							p->size().height() - p->margin().top() - p->margin().bottom());
				}

				IteratorPtr it = p->CreateIterator(event);

				if(m_scissor_status.valid()) {
					m_scissor_status.Enable();

					for (it->GoToFirst(); !it->IsEnd(); it->GoNext()) {
						DispatchDrawEvent(it->GetWidget(), event);
					}

				} else {

					for (it->GoToFirst(); !it->IsEnd(); it->GoNext()) {
						DispatchDrawEvent(it->GetWidget(), event);
					}
				}

				if(p->scissor_test()) {
					m_scissor_status.Pop();
					m_scissor_status.Disable();
				}

			}

		}
	}

	bool Section::CheckAndUpdateHoverWidget (const MouseEvent& event)
	{
		if (m_last_hover_widget) {

			if (IsHoverThrough(m_last_hover_widget, event.position())) {
				//DBG_PRINT_MSG("last widget %s is under cursor", m_last_hover_widget->name().c_str());
				UpdateHoverWidgetSubs(event);
			} else {
				m_last_hover_widget->destroyed().disconnectOne(this,
				        &Section::OnHoverWidgetDestroyed);
				m_last_hover_widget->set_hover(false);
				m_last_hover_widget->CursorEnterEvent(false);

				// find which contianer contains cursor position
				while (m_last_hover_widget->container()) {

					if (m_last_hover_widget->container() == this) {	// FIXME: the widget may be mvoed to another context
						//DBG_PRINT_MSG("last widget %s is not under cursor and container is context", m_last_hover_widget->name().c_str());
						m_last_hover_widget = 0;
						break;
					} else {
						//DBG_PRINT_MSG("last widget %s is not under cursor but container is", m_last_hover_widget->name().c_str());
						m_last_hover_widget = m_last_hover_widget->container();

						if (IsHoverThrough(m_last_hover_widget, event.position())) {
							break;
						} else {
							m_last_hover_widget->destroyed().disconnectOne(this,
							        &Section::OnHoverWidgetDestroyed);
							m_last_hover_widget->set_hover(false);
							m_last_hover_widget->CursorEnterEvent(false);
						}
					}
				}

				if (m_last_hover_widget) {
					UpdateHoverWidgetSubs(event);
				}
			}

		} else {
			for (std::set<AbstractWidget*>::iterator it = m_set.begin();
			        it != m_set.end(); it++) {
				if ((*it)->Contain(event.position())) {

					//DBG_PRINT_MSG("Get hover widget: %s", (*it)->name().c_str());
					m_last_hover_widget = *it;
					events()->connect(m_last_hover_widget->destroyed(), this,
					        &Section::OnHoverWidgetDestroyed);
					m_last_hover_widget->set_hover(true);
					m_last_hover_widget->CursorEnterEvent(true);

					UpdateHoverWidgetSubs(event);
					break;
				}
			}
		}

		return m_last_hover_widget != 0;
	}

	void Section::UpdateHoverWidgetSubs (const MouseEvent& event)
	{
		AbstractContainer* p = dynamic_cast<AbstractContainer*>(m_last_hover_widget);

		if (p) {

			IteratorPtr it = p->CreateIterator(event);
			AbstractWidget* widget = 0;
			for (it->GoToFirst(); !it->IsEnd(); it->GoNext()) {

				widget = it->GetWidget();
				if(widget->Contain(event.position())) {
					m_last_hover_widget = widget;
					events()->connect(m_last_hover_widget->destroyed(), this, &Section::OnHoverWidgetDestroyed);
					m_last_hover_widget->set_hover(true);
					m_last_hover_widget->CursorEnterEvent(true);

					UpdateHoverWidgetSubs(event);
					break;
				}
			}

		}
	}

	void Section::OnSubWidgetDestroyed(AbstractWidget* widget)
	{
		RemoveSubWidget(widget);

		if(m_set.size() == 0) {

			if(managed() && (count() == 0)) {
				DBG_PRINT_MSG("no sub widgets, delete this section: %s", name().c_str());
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
		DBG_PRINT_MSG("%s", "HERE");
		while (m_last_hover_widget != widget) {
			m_last_hover_widget->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);
			m_last_hover_widget = m_last_hover_widget->container();
		}

		m_last_hover_widget->destroyed().disconnectOne(this, &Section::OnHoverWidgetDestroyed);

		m_last_hover_widget = m_last_hover_widget->container();
		if(m_last_hover_widget == this)
			m_last_hover_widget = 0;

		DBG_PRINT_MSG("%s", "HERE");
	}

}
