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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/OpenGL/GLFramebuffer.hpp>
#include <BlendInt/OpenGL/GLRenderbuffer.hpp>

#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Theme.hpp>

namespace BlendInt
{
	using Stock::Shaders;

	glm::mat4 Context::default_view_matrix = glm::lookAt(glm::vec3(0.f, 0.f, 1.f),
			glm::vec3(0.f, 0.f, 0.f),
            glm::vec3(0.f, 1.f, 0.f));

	std::set<Context*> Context::context_set;

	Context::Context ()
	: AbstractContainer(),
	  m_refresh(false),
	  m_focused_widget(0)
	{
		set_size(640, 480);

		InitializeContext();

		context_set.insert(this);
	}

	Context::~Context ()
	{
		if(container() != 0) {
			DBG_PRINT_MSG("Error: %s", "Context MUST NOT be in any other container");
		}

		if (m_focused_widget) {
			m_focused_widget->set_focus(false);
			m_focused_widget->destroyed().disconnectOne(this, &Context::OnFocusedWidgetDestroyed);
			m_focused_widget = 0;
		}

		context_set.erase(this);
	}

	Section* Context::PushBack (AbstractWidget* widget)
	{
		if(!widget) {
			DBG_PRINT_MSG("Error: %s", "widget pointer is 0");
			return 0;
		}

		Section* section = Section::GetSection(widget);

		if(section) {

			if(section->container() == this) {
				DBG_PRINT_MSG("Widget %s is already in context %s",
									widget->name().c_str(),
									name().c_str());
				return section;
			} else {
				AbstractContainer::RemoveSubWidget(section->container(), section);
			}
		} else {
			section = Manage(new Section);
			section->PushBack(widget);
		}

#ifdef DEBUG
		if(!section->managed()) {
			DBG_PRINT_MSG("Warning: the section %s is not set managed", section->name().c_str());
		}

		if(section->m_deque.size() == 0) {
			DBG_PRINT_MSG("Warning: trying to add an emptry section %s in a context, it will not be delete automatically", section->name().c_str());
		}

		char buf[32];
		sprintf(buf, "Section %ld", deque().size());
		DBG_SET_NAME(section, buf);
#endif

		PushBackSubWidget(section);

		ResizeSubWidget(section, size());

		return section;
	}

	Section* Context::Remove (AbstractWidget* widget)
	{
		if(!widget) {
			DBG_PRINT_MSG("Error: %s", "widget pointer is 0");
			return 0;
		}

		// if the container is a section, the section will destroy itself if it's empty
		Section* section = Section::GetSection(widget);
		assert(section->container() == this);

		AbstractContainer::RemoveSubWidget(widget->container(), widget);

		if(widget->focused()) {

			assert(m_focused_widget == widget);

			widget->set_focus(false);
			widget->destroyed().disconnectOne(this, &Context::OnFocusedWidgetDestroyed);
			m_focused_widget = 0;

		}

		if(section->m_deque.size() == 0) {
			DBG_PRINT_MSG("no sub widgets, delete this section: %s", section->name().c_str());
			if(section->managed() && (section->count() == 0)) {
				delete section;
				section = 0;
			} else {
				DBG_PRINT_MSG("Warning: %s", "the section is empty but it's not set managed"
						", and it's referenced by a smart pointer, it will not be deleted automatically");
			}
		}

		return section;
	}

	void Context::MoveToTop(const Section* section)
	{
		if((section == 0) || (section->container() != this)) return;

		AbstractWidgetDeque::iterator it = std::find(m_deque.begin(), m_deque.end(), section);

		if(it == m_deque.end()) return;

		AbstractWidget* sect = *it;

		m_deque.erase(it);
		m_deque.push_back(sect);
	}

	void Context::MoveToBottom(const Section* section)
	{
		if((section == 0) || (section->container() != this)) return;

		AbstractWidgetDeque::iterator it = std::find(m_deque.begin(), m_deque.end(), section);

		if(it == m_deque.end()) return;

		AbstractWidget* sect = *it;

		m_deque.erase(it);
		m_deque.push_front(sect);
	}

	void Context::MoveUp (const Section* section)
	{
		if((section == 0) || (section->container() != this)) return;

		AbstractWidgetDeque::iterator it = std::find(m_deque.begin(), m_deque.end(), section);

		if(it == m_deque.end()) return;

		AbstractWidgetDeque::iterator next = it;
		std::advance(next, 1);

		if(next == m_deque.end()) return;

		AbstractWidget* tmp = *it;
		*it = *next;
		*next = tmp;
	}

	void Context::MoveDown(const Section* section)
	{
		if((section == 0) || (section->container() != this)) return;

		AbstractWidgetDeque::iterator it = std::find(m_deque.begin(), m_deque.end(), section);

		if(it == m_deque.end()) return;

		AbstractWidgetDeque::iterator prev = it;
		std::advance(prev, -1);

		if(it == m_deque.begin()) return;

		AbstractWidget* tmp = *it;
		*it = *prev;
		*prev = tmp;
	}

	bool Context::Contain (const Point& point) const
	{
		return true;
	}

	void Context::SetFocusedWidget (AbstractWidget* widget)
	{
		if(m_focused_widget == widget) {
			return;
		}

		if (m_focused_widget) {
			m_focused_widget->set_focus(false);
			m_focused_widget->destroyed().disconnectOne(this, &Context::OnFocusedWidgetDestroyed);
			m_focused_widget->FocusEvent(false);
		}

		m_focused_widget = widget;
		if (m_focused_widget) {
			m_focused_widget->set_focus(true);
			events()->connect(m_focused_widget->destroyed(), this, &Context::OnFocusedWidgetDestroyed);
			m_focused_widget->FocusEvent(true);
		}
	}

	void Context::SetCursor (int cursor_type)
	{
		// TODO: overwrite this
	}

	int Context::GetCursor () const
	{
		// TODO: overwrite this
		return ArrowCursor;
	}

	void Context::PushCursor (int cursor_type)
	{
		// TODO: overwrite this
	}

	int Context::PopCursor ()
	{
		// TODO:: overwrite this
		return ArrowCursor;
	}

	Context* Context::GetContext (AbstractWidget* widget)
	{
		AbstractContainer* container = widget->container();

		if(container == 0) {
			return dynamic_cast<Context*>(widget);
		} else {

			while(container->container()) {
				container = container->container();
			}

		}

		return dynamic_cast<Context*>(container);
	}

	void Context::RenderToTexture (AbstractWidget* widget,
	        GLTexture2D* texture)
	{
		Section::RenderToTexture(widget, texture);
	}

	void Context::RenderToFile (AbstractWidget* widget,
	        const char* filename)
	{
		Section::RenderToFile(widget, filename);
	}

	bool Context::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		if(request.source()->container() == this) {
			// don't allow section to change any geometry
			return false;
		}

		return true;
	}

	bool Context::PositionUpdateTest (const PositionUpdateRequest& request)
	{
		if(request.source()->container() == this) {

			// don't allow section to change any geometry
			return false;
		}

		return true;
	}

	bool Context::RoundTypeUpdateTest (const RoundTypeUpdateRequest& request)
	{
		if(request.source()->container() == this) {

			// don't allow section to change any geometry
			return false;
		}

		return true;
	}

	bool Context::RoundRadiusUpdateTest (
	        const RoundRadiusUpdateRequest& request)
	{
		if(request.source()->container() == this) {

			// don't allow section to change any geometry
			return false;
		}

		return true;
	}

	void Context::PerformPositionUpdate (const PositionUpdateRequest& request)
	{
		// nothing need to do.
	}

	void Context::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.source() == this) {

			glm::mat4 projection = glm::ortho(0.f, (GLfloat) request.size()->width(),
			        0.f, (GLfloat) request.size()->height(), 100.f, -100.f);

			RefPtr<GLSLProgram> program =
			        Shaders::instance->default_triangle_program();
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

			program->Reset();

			for (AbstractWidgetDeque::iterator it = m_deque.begin();
			        it != m_deque.end(); it++) {
				ResizeSubWidget(*it, *request.size());
			}

			set_size(*request.size());

			m_resized.fire(size());

		} else if (request.source()->container() == this) {

			if (request.source()->drop_shadow() && request.source()->m_shadow) {
				request.source()->m_shadow->Resize(request.size()->width(),
				        request.size()->height());
			}

		} else {

			// if(request.target()->container()->hover()) {
			//	m_layers[request.source()->z()].m_hover_list_valid = false;
			// }

			//DBG_PRINT_MSG("%s", "get widget geometry change update");

			//DBG_PRINT_MSG("source widget: %s, target widget: %s", request.source()->name().c_str(), request.target()->name().c_str());

		}

	}

	void Context::PerformRoundTypeUpdate (const RoundTypeUpdateRequest& request)
	{
	}

	void Context::PerformRoundRadiusUpdate (
	        const RoundRadiusUpdateRequest& request)
	{
	}

	ResponseType Context::Draw (const RedrawEvent& event)
	{
		//glm::vec3 pos(position().x(), position().y(), z());
		//glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);
		const_cast<RedrawEvent&>(event).m_context = this;

		glClearColor(0.208f, 0.208f, 0.208f, 1.f);

		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT |
		GL_STENCIL_BUFFER_BIT);
		// Here cannot enable depth test -- glEnable(GL_DEPTH_TEST);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glViewport(0, 0, size().width(), size().height());

		for (AbstractWidgetDeque::iterator it = m_deque.begin();
		        it != m_deque.end(); it++) {
			(*it)->Draw(event);
		}

		return Accept;
	}

	ResponseType Context::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Context::KeyPressEvent (const KeyEvent& event)
	{
		const_cast<KeyEvent&>(event).m_context = this;

		ResponseType response;

		if(m_focused_widget) {
			response = m_focused_widget->KeyPressEvent(event);
		}

		return response;
	}

	ResponseType Context::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		const_cast<ContextMenuEvent&>(event).m_context = this;

		return Ignore;
	}

	ResponseType Context::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		const_cast<ContextMenuEvent&>(event).m_context = this;

		return Ignore;
	}

	ResponseType Context::MousePressEvent (const MouseEvent& event)
	{
		const_cast<MouseEvent&>(event).m_context = this;

		ResponseType response;

		AbstractWidget* widget = 0;
		AbstractWidget* original_focused_widget = m_focused_widget;	// mouse press event may change the focused widget

		const_cast<MouseEvent&>(event).m_context = this;

		AbstractWidget* section = 0;

		for(AbstractWidgetDeque::reverse_iterator it = m_deque.rbegin();
				it != m_deque.rend();
				it++)
		{
			response = (*it)->MousePressEvent(event);

			if (response == Accept) {
				section = *it;
				break;
			}
		}

		// The Section may be deleted in this event, e.g, a popup menu may delete itself when click a menuitem
		// Have to check if the section is still exist in deque
		AbstractWidgetDeque::iterator it = std::find (m_deque.begin(), m_deque.end(), section);
		if(it != m_deque.end()) {
			widget = dynamic_cast<Section*>(*it)->m_last_hover_widget;
		}

		if(original_focused_widget != m_focused_widget) {

			if(original_focused_widget && original_focused_widget->focused()) {
				original_focused_widget->set_focus(false);
				original_focused_widget->destroyed().disconnectOne(this, &Context::OnFocusedWidgetDestroyed);
				original_focused_widget->FocusEvent(false);
			}

		} else {

			m_focused_widget = original_focused_widget;
			SetFocusedWidget(widget);

		}

		/*
		if(m_focused_widget) {
			DBG_PRINT_MSG("focus widget: %s", m_focused_widget->name().c_str());
		} else {
			DBG_PRINT_MSG("%s", "focus widget unset");
		}
		*/

		return response;
	}

	ResponseType Context::MouseReleaseEvent (const MouseEvent& event)
	{
		const_cast<MouseEvent&>(event).m_context = this;

		ResponseType response;

		// tell the focused widget first
		if(m_focused_widget) {
			response = m_focused_widget->MouseReleaseEvent(event);
		}

		if(response == Accept) {
			return response;
		}

		for(AbstractWidgetDeque::reverse_iterator it = m_deque.rbegin();
				it != m_deque.rend();
				it++)
		{
			response = (*it)->MouseReleaseEvent(event);
			if (response == Accept) {
				break;
			}
		}

		return response;
	}

	ResponseType Context::MouseMoveEvent (const MouseEvent& event)
	{
		const_cast<MouseEvent&>(event).m_context = this;

		ResponseType response;

		// tell the focused widget first
		if (m_focused_widget) {
			response = m_focused_widget->MouseMoveEvent(event);
		}

		if(response == Accept) {

			// still set cursor hover
			for(AbstractWidgetDeque::reverse_iterator it = m_deque.rbegin();
					it != m_deque.rend();
					it++)
			{
				if(dynamic_cast<Section*>(*it)->CheckAndUpdateHoverWidget(event)) break;
			}

			return response;	// return Accept
		}

		for(AbstractWidgetDeque::reverse_iterator it = m_deque.rbegin();
				it != m_deque.rend();
				it++)
		{
			response = (*it)->MouseMoveEvent(event);

			if (response == Accept) {
				break;
			}
		}

		return response;
	}

	ResponseType Context::FocusEvent (bool focus)
	{
		return Ignore;
	}

	void Context::InitializeContext ()
	{
		using Stock::Shaders;

		glm::mat4 projection = glm::ortho(0.f, 640.f, 0.f, 480.f, 100.f, -100.f);

		RefPtr<GLSLProgram> program = Shaders::instance->default_triangle_program();
		program->Use();
		program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE, glm::value_ptr(projection));
		program->SetUniformMatrix4fv("u_view", 1, GL_FALSE, glm::value_ptr(default_view_matrix));

		program = Shaders::instance->default_line_program();
		program->Use();
		program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE, glm::value_ptr(projection));
		program->SetUniformMatrix4fv("u_view", 1, GL_FALSE, glm::value_ptr(default_view_matrix));

		program = Shaders::instance->default_text_program();
		program->Use();
		program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE, glm::value_ptr(projection));
		program->SetUniformMatrix4fv("u_view", 1, GL_FALSE, glm::value_ptr(default_view_matrix));

		program = Shaders::instance->default_image_program();
		program->Use();
		program->SetUniformMatrix4fv("u_projection", 1, GL_FALSE, glm::value_ptr(projection));
		program->SetUniformMatrix4fv("u_view", 1, GL_FALSE, glm::value_ptr(default_view_matrix));

		program->Reset();
	}

	AbstractWidget* Context::GetWidgetUnderCursor(const MouseEvent& event, AbstractWidget* parent)
	{
		return 0;
	}

	void Context::OnFocusedWidgetDestroyed (AbstractWidget* widget)
	{
		DBG_PRINT_MSG("focused widget %s destroyed", widget->name().c_str());

		assert(m_focused_widget == widget);

		if(widget->focused()) {
			widget->set_focus(false);
			widget->destroyed().disconnectOne(this, &Context::OnFocusedWidgetDestroyed);
		}

		m_focused_widget = 0;
	}

}
