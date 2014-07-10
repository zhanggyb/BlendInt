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

namespace BlendInt
{

	std::set<Context*> Context::context_set;

	Context::Context ()
	: AbstractContainer(),
	  m_context_buffer(0),
	  m_vao(0),
	  m_refresh(false),
	  m_focused_widget(0)
	{
		set_size(640, 480);

		m_redraw_event.set_view_matrix(
				glm::lookAt(glm::vec3(0.f, 0.f, 1.f),
						glm::vec3(0.f, 0.f, 0.f),
		                glm::vec3(0.f, 1.f, 0.f)));
		// default is 640 x 480
		m_redraw_event.set_projection_matrix(
		        glm::ortho(0.f, 640.f, 0.f, 480.f, 100.f, -100.f));

		m_context_buffer = new GLTexture2D;
#ifdef DEBUG
		m_context_buffer->set_name("Main Buffer");
#endif

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

		Section* sections = 0;
		for(std::deque<Section*>::reverse_iterator it = m_sections.rbegin(); it != m_sections.rend(); it++)
		{
			sections = *it;
			sections->destroyed().disconnectOne(this, &Context::OnSubWidgetDestroyed);
			sections->m_container = 0;
			sections->set_hover(false);
			sections->set_focus(false);

			if(sections->managed() && (sections->count() == 0)) {
				delete sections;
			}
		}

		if (m_context_buffer) {
			m_context_buffer->Clear();
			delete m_context_buffer;
		}

		glDeleteVertexArrays(1, &m_vao);

		context_set.erase(this);
	}

	Section* Context::PushBack (AbstractWidget* widget)
	{
		if(!widget) {
			DBG_PRINT_MSG("Error: %s", "widget pointer is 0");
			return 0;
		}

		Section* section = widget->GetSection();

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
			section->Insert(widget);

		}

#ifdef DEBUG
		if(!section->managed()) {
			DBG_PRINT_MSG("Warning: the section %s is not set managed", section->name().c_str());
		}

		if(section->m_set.size() == 0) {
			DBG_PRINT_MSG("Warning: trying to add an emptry section %s in a context, it will not be delete automatically", section->name().c_str());
		}

		char buf[32];
		sprintf(buf, "Section %ld", m_sections.size());
		DBG_SET_NAME(section, buf);
#endif

		m_sections.push_back(section);
		SetContainer(section, this);

		ResizeSubWidget(section, size());

		events()->connect(section->destroyed(), this, &Context::OnSubWidgetDestroyed);

		CheckSubWidgetAddedInContainer(widget);

		return section;
	}

	Section* Context::Remove (AbstractWidget* widget)
	{
		if(!widget) {
			DBG_PRINT_MSG("Error: %s", "widget pointer is 0");
			return 0;
		}

		// if the container is a section, the section will destroy itself if it's empty

		Section* section = widget->GetSection();

		assert(section->container() == this);

		AbstractContainer::RemoveSubWidget(widget->container(), widget);

		if(section->m_set.size() == 0) {

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

	int Context::GetMaxLayer () const
	{
		return m_sections.size() - 1;
	}

	bool Context::Contain (const Point& point) const
	{
		return true;
	}

	void Context::RefreshLayer (int layer)
	{
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

#ifdef DEBUG

	void Context::PrintSections()
	{
		if(m_sections.size() == 0) {
			DBG_PRINT_MSG("%s", "Section deque is empty");
		}

		for(std::deque<Section*>::iterator it = m_sections.begin(); it != m_sections.end(); it++)
		{
			DBG_PRINT_MSG("Section: %s at (%d, %d) - (%d, %d) contains: ", (*it)->name().c_str(),
					(*it)->position().x(), (*it)->position().y(),
					(*it)->size().width(), (*it)->size().height());

			for(std::set<AbstractWidget*>::iterator s_it = (*it)->m_set.begin(); s_it != (*it)->m_set.end(); s_it++)
			{
				DBG_PRINT_MSG("\t %s", (*s_it)->name().c_str());
			}
		}
	}

#endif

	void Context::UpdateContainer(const ContainerUpdateRequest& request)
	{
		switch(request.type()) {

			case ContainerRefresh: {
				//RefreshLayer(request.source()->z());
				break;
			}

			case ContainerSubWidgetAdded: {
				const AbstractWidget* widget =
				        static_cast<const AbstractWidget*>(request.data());
				DBG_PRINT_MSG("Container \"%s\" add sub widget \"%s\"",
				        request.source()->name().c_str(),
				        widget->name().c_str());

				// TODO: check the layer

				//if(request.source()->hover()) {
					//m_layers[request.source()->z()].m_hover_list_valid = false;
				//}

				break;
			}

			case ContainerSubWidgetRemoved: {
				const AbstractWidget* widget =
				        static_cast<const AbstractWidget*>(request.data());
				DBG_PRINT_MSG("Container \"%s\" remove sub widget \"%s\"",
				        request.source()->name().c_str(),
				        widget->name().c_str());

				// TODO: check the layer
				if(request.source()->hover()) {
					//m_layers[request.source()->z()].m_hover_list_valid = false;

					AbstractWidget* w = const_cast<AbstractWidget*>(widget);
					if(w->hover()) {
						//RemoveWidgetFromHoverList(w);
						w->set_hover(false);
					}
				}

				break;
			}

			default:
				break;
		}
	}

	bool Context::UpdateGeometryTest(const GeometryUpdateRequest& request)
	{
		if(request.source()->container() == this) {

			// don't allow section to change any geometry
			return false;
		}

		return true;
	}

	void Context::UpdateGeometry (const GeometryUpdateRequest& request)
	{
		if(request.source () == this) {

			switch (request.type()) {

				case WidgetSize: {

					const Size* size_p = static_cast<const Size*>(request.data());

					GLfloat vertices[] = {
							0.f, 0.f,											0.f, 1.f,
							(GLfloat)size_p->width(), 0.f,						1.f, 1.f,
							0.f, (GLfloat)size_p->height(),						0.f, 0.f,
							(GLfloat)size_p->width(), (GLfloat)size_p->height(),	1.f, 0.f
					};

					m_vbo->Bind();
					m_vbo->UpdateData(vertices, sizeof(vertices));
					m_vbo->Reset();

					m_redraw_event.set_projection_matrix(
									glm::ortho(0.f,
											(GLfloat)size_p->width(),
											0.f,
											(GLfloat)size_p->height(),
											100.f, -100.f));

					for(std::deque<Section*>::iterator it = m_sections.begin(); it != m_sections.end(); it++)
					{
						ResizeSubWidget(*it, *size_p);
					}

					// TODO: redraw
					break;
				}

				case WidgetPosition: {
					// always at (0, 0)
					break;
				}

				default:
					break;
					;
			}

		} else if (request.source()->container() == this) {

			switch(request.type()) {

				case WidgetPosition: {
					//m_layers[request.source()->z()].m_hover_list_valid = false;
					break;
				}

				case WidgetSize: {
					//m_layers[request.source()->z()].m_hover_list_valid = false;

					const Size* size_p = static_cast<const Size*>(request.data());

					if(request.source()->drop_shadow() && request.source()->m_shadow) {
						request.source()->m_shadow->Resize(size_p->width(), size_p->height());
					}

					break;
				}

				default:
					break;

			}

		} else {

			// if(request.target()->container()->hover()) {
			//	m_layers[request.source()->z()].m_hover_list_valid = false;
			// }

			//DBG_PRINT_MSG("%s", "get widget geometry change update");

			//DBG_PRINT_MSG("source widget: %s, target widget: %s", request.source()->name().c_str(), request.target()->name().c_str());

		}
	}

	void Context::BroadcastUpdate (const GeometryUpdateRequest& request)
	{
		if (request.source() == this) {

			switch (request.type()) {

				case WidgetSize: {
					const Size* size_p =
					        static_cast<const Size*>(request.data());
					m_resized.fire(*size_p);
					break;
				}

				default:
					AbstractContainer::BroadcastUpdate(request);

			}

		}
	}

	ResponseType Context::Draw (const RedrawEvent& event)
	{
		//glm::vec3 pos(position().x(), position().y(), z());
		//glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		glClearColor(0.208f, 0.208f, 0.208f, 1.f);

		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT |
		GL_STENCIL_BUFFER_BIT);
		// Here cannot enable depth test -- glEnable(GL_DEPTH_TEST);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glViewport(0, 0, size().width(), size().height());

		for (std::deque<Section*>::iterator it = m_sections.begin();
		        it != m_sections.end(); it++) {
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
		ResponseType response;

		if(m_focused_widget) {
			response = m_focused_widget->KeyPressEvent(event);
		}

		return response;
	}

	ResponseType Context::ContextMenuPressEvent (const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType Context::ContextMenuReleaseEvent (
	        const ContextMenuEvent& event)
	{
		return Accept;
	}

	ResponseType Context::MousePressEvent (const MouseEvent& event)
	{
		ResponseType response;

		AbstractWidget* widget = 0;
		AbstractWidget* original_focused_widget = m_focused_widget;	// mouse press event may change the focused widget

		for(std::deque<Section*>::reverse_iterator it = m_sections.rbegin();
				it != m_sections.rend();
				it++)
		{
			response = (*it)->MousePressEvent(event);

			if (response == Accept) {
				widget = (*it)->m_last_hover_widget;
				break;
			}
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
		ResponseType response;

		// tell the focused widget first
		if(m_focused_widget) {
			response = m_focused_widget->MouseReleaseEvent(event);
		}

		if(response == Accept) {
			return response;
		}

		for(std::deque<Section*>::reverse_iterator it = m_sections.rbegin();
				it != m_sections.rend();
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
		m_redraw_event.set_cursor_position(event.position());

		ResponseType response;

		// tell the focused widget first
		if (m_focused_widget) {
			response = m_focused_widget->MouseMoveEvent(event);
		}

		if(response == Accept) {

			// still set cursor hover
			for(std::deque<Section*>::reverse_iterator it = m_sections.rbegin();
					it != m_sections.rend();
					it++)
			{
				if((*it)->CheckAndUpdateHoverWidget(event)) break;
			}

			return response;	// return Accept
		}

		for(std::deque<Section*>::reverse_iterator it = m_sections.rbegin();
				it != m_sections.rend();
				it++)
		{
			response = (*it)->MouseMoveEvent(event);

			if (response == Accept) {
				break;
			}
		}

		return response;
	}

	bool Context::RemoveSubWidget (AbstractWidget* widget)
	{
		if(!widget) {
			DBG_PRINT_MSG("Warning: %s", "widget pointer is 0");
			return false;
		}

		assert(widget->container() == this);

		std::deque<Section*>::iterator it = std::find(m_sections.begin(),
				m_sections.end(), widget);

		if (it != m_sections.end()) {
			widget->destroyed().disconnectOne(this,
			        &Context::OnSubWidgetDestroyed);

			m_sections.erase(it);
			SetContainer(widget, 0);
		} else {
			DBG_PRINT_MSG("Warning: object %s is not found in container %s",
			        widget->name().c_str(), name().c_str());
			return false;
		}

		CheckSubWidgetRemovedInContainer(widget);

		return true;
	}

	ResponseType Context::FocusEvent (bool focus)
	{
		return Ignore;
	}

	IteratorPtr Context::CreateIterator (const DeviceEvent& event)
	{
		IteratorPtr ret;
		return ret;
	}

	void Context::InitializeContext ()
	{
		using Stock::Shaders;

		glGenVertexArrays(1, &m_vao);

		glBindVertexArray(m_vao);

		m_vbo.reset(new GLArrayBuffer);
		m_vbo->Generate();

		GLfloat vertices [] = {
				0.f, 0.f,											0.f, 1.f,
				(GLfloat)size().width(), 0.f,						1.f, 1.f,
				0.f, (GLfloat)size().height(),						0.f, 0.f,
				(GLfloat)size().width(), (GLfloat)size().height(),	1.f, 0.f
		};

		m_vbo->Bind();
		m_vbo->SetData(sizeof(vertices), vertices);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
		        BUFFER_OFFSET(0));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
		        BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);

		GLArrayBuffer::Reset();
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

	void Context::OnSubWidgetDestroyed (AbstractWidget* widget)
	{
		DBG_PRINT_MSG("Remove section from context: %s", widget->name().c_str());
		RemoveSubWidget(widget);
	}

}
