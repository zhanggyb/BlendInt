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
	  m_focused_widget(0),
	  m_max_tex_buffer_cache_size(6),
	  refresh_once(false),
	  force_refresh_all(true)
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

		if(m_focused_widget) {
			SetFocusedWidget(0);
		}

		AbstractWidget* widget = 0;
		for(AbstractWidgetDeque::reverse_iterator it = m_widgets.rbegin(); it != m_widgets.rend(); it++)
		{
			widget = *it;
			widget->destroyed().disconnectOne(this, &Context::OnSubWidgetDestroyed);
			widget->m_container = 0;
			widget->set_hover(false);
			widget->set_focus(false);

			if(widget->managed() && (widget->count() == 0)) {
				delete widget;
			}
		}

		/*
		std::map<int, ContextLayer>::iterator layer_iter;
		std::set<AbstractWidget*>::iterator widget_iter;

		AbstractWidget* widget = 0;
		for (layer_iter = m_layers.begin(); layer_iter != m_layers.end();
		        layer_iter++)
		{
			for (widget_iter = layer_iter->second.m_widget_set->m_widgets.begin();
			        widget_iter != layer_iter->second.m_widget_set->m_widgets.end();
			        widget_iter++)
			{
				widget = *widget_iter;
				widget->destroyed().disconnectOne(this,
				        &Context::OnSubWidgetDestroyed);
				widget->m_container = 0;
				widget->set_hover(false);
				widget->set_focus(false);

				if (widget->managed() && (widget->count() == 0)) {
					delete *widget_iter;
				}
			}

			layer_iter->second.m_widget_set->m_widgets.clear();
			layer_iter->second.m_hover_cache->m_widgets.clear();
		}

		m_deque.clear();

		m_layers.clear();
		*/

		if (m_context_buffer) {
			m_context_buffer->Clear();
			delete m_context_buffer;
		}

		glDeleteVertexArrays(1, &m_vao);

		context_set.erase(this);
	}

	void Context::Add (AbstractWidget* widget)
	{
		if(!widget) {
			DBG_PRINT_MSG("Error: %s", "widget pointer is 0");
			return;
		}

		if(widget->container()) {

			if(widget->container() == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s",
									widget->name().c_str(),
									widget->container()->name().c_str());
				return;
			} else {
				AbstractContainer::RemoveSubWidget(widget->container(), widget);
			}
		}

		m_widgets.push_back(widget);

		// set shadow
		if(widget->drop_shadow()) {
			if(!widget->m_shadow) {
				widget->m_shadow.reset(new Shadow);
			}

			widget->m_shadow->Resize(widget->size());
		}

		SetContainer(widget, this);
		events()->connect(widget->destroyed(), this, &Context::OnSubWidgetDestroyed);

		CheckSubWidgetAddedInContainer(widget);

		return;
	}

	void Context::Remove (AbstractWidget* widget)
	{
		RemoveSubWidget(widget);
	}

	int Context::GetMaxLayer () const
	{
		return m_widgets.size() - 1;
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
			m_focused_widget->FocusEvent(false);
		}

		m_focused_widget = widget;
		if (m_focused_widget) {
			m_focused_widget->set_focus(true);
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
						RemoveWidgetFromHoverList(w);
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

					// TODO: redraw
					force_refresh_all = true;
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

	void Context::BroadcastUpdate(const GeometryUpdateRequest& request)
	{

		if(request.source() == this) {

			switch (request.type()) {

				case WidgetSize: {
					const Size* size_p = static_cast<const Size*>(request.data());
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

		for (AbstractWidgetDeque::iterator it = m_widgets.begin();
				it != m_widgets.end();
				it++)
		{
			//(*set_it)->Draw();
			DispatchDrawEvent(*it, event);
		}

		return Accept;
	}

	ResponseType Context::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Context::KeyPressEvent (const KeyEvent& event)
	{
		return Ignore;

		if(m_focused_widget) {
#ifdef DEBUG
			if(event.key() == Key_F6 && event.text().empty()) {
				//DrawToOffScreen();
				//RenderToImage();
			}
#endif
			m_focused_widget->KeyPressEvent(event);

			return Accept;
		} else {
			return Ignore;
		}
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
		ResponseType response = Ignore;
		AbstractWidget* widget = 0;

		AbstractWidget* original_focused_widget = m_focused_widget;
		bool focus_widget_changed = false;

		for(std::deque<AbstractWidget*>::reverse_iterator it = m_widgets.rbegin();
				it != m_widgets.rend();
				it++)
		{
			widget = *it;

			if(widget->Contain(event.position())) {
				response = widget->MousePressEvent(event);

				// a widget may change the focused widget
				if(original_focused_widget != m_focused_widget) {
					focus_widget_changed = true;
				}

				if(response == Accept || response == AcceptAndBreak) {
					break;
				}

			}

			widget = 0;
		}

		if(focus_widget_changed) {

			/*
			if(original_focused_widget) {
				original_focused_widget->set_focus(false);
				original_focused_widget->FocusEvent(false);
			}
			*/

		} else {

			m_focused_widget = original_focused_widget;
			SetFocusedWidget(widget);

		}

		if(m_focused_widget) {
			DBG_PRINT_MSG("focus widget: %s", m_focused_widget->name().c_str());
		}

		if(widget != 0)
			return Accept;

		return Ignore;
	}

	ResponseType Context::MouseReleaseEvent (const MouseEvent& event)
	{
		ResponseType response;

		// tell the focused widget first
		if(m_focused_widget) {
			response = m_focused_widget->MouseReleaseEvent(event);

			// Check the event status
			if(response == Accept) {

			}
		}

		return Accept;
	}

	ResponseType Context::MouseMoveEvent (const MouseEvent& event)
	{
		return Ignore;

		ResponseType response;

		m_redraw_event.set_cursor_position(event.position());

		// tell the focused widget first
		if(m_focused_widget) {
			response = m_focused_widget->MouseMoveEvent(event);

			if(response == AcceptAndBreak)
				return Accept;
			// check the event status
		}

		return Ignore;
	}

	bool Context::RemoveSubWidget (AbstractWidget* widget)
	{
		if(!widget) {
			DBG_PRINT_MSG("Warning: %s", "widget pointer is 0");
			return false;
		}

		assert(widget->container() == this);

		AbstractWidgetDeque::iterator it = std::find(m_widgets.begin(),
						m_widgets.end(), widget);

		if (it != m_widgets.end()) {

			widget->destroyed().disconnectOne(this,
							&Context::OnSubWidgetDestroyed);

			m_widgets.erase(it);
			SetContainer(widget, 0);
		} else {
			DBG_PRINT_MSG("Warning: object %s is not found in container %s",
							widget->name().c_str(), name().c_str());
			return false;
		}

		if(widget->hover()) {
			RemoveWidgetFromHoverList(this);
		}

		if(widget->focused()) {
			widget->set_focus(false);

			if(widget == m_focused_widget) {
				m_focused_widget = 0;
			}
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

		glVertexAttribPointer(0, 2,	GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(0));
		glVertexAttribPointer(1, 2,	GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, BUFFER_OFFSET(2 * sizeof(GLfloat)));

		glBindVertexArray(0);

		GLArrayBuffer::Reset();
	}

	void Context::DispatchDrawEvent (AbstractWidget* widget,
					const RedrawEvent& event)
	{
		if (widget->visiable()) {

			if(widget->drop_shadow() && widget->m_shadow) {
				widget->m_shadow->DrawAt(event.projection_matrix() * event.view_matrix(), widget->position());
			}

			widget->Draw(event);

			AbstractContainer* p = dynamic_cast<AbstractContainer*>(widget);
			if (p) {

				if(p->scissor_test()) {
					scissor_status.Push(p->position().x() + p->margin().left(),
							p->position().y() + p->margin().right(),
							p->size().width() - p->margin().left() - p->margin().right(),
							p->size().height() - p->margin().top() - p->margin().bottom());
				}

				IteratorPtr it = p->CreateIterator(event);

				if(scissor_status.valid()) {
					scissor_status.Enable();

					for (it->GoToFirst(); !it->IsEnd(); it->GoNext()) {
						DispatchDrawEvent(it->GetWidget(), event);
					}

				} else {

					for (it->GoToFirst(); !it->IsEnd(); it->GoNext()) {
						DispatchDrawEvent(it->GetWidget(), event);
					}
				}

				if(p->scissor_test()) {
					scissor_status.Pop();
					scissor_status.Disable();
				}

			}

		}
	}
	
	void Context::BuildCursorHoverList ()
	{

	}

	void Context::AppendCursorHoverList (std::deque<AbstractWidget*>& deque,
	        AbstractWidget* parent)
	{

	}


	AbstractWidget* Context::GetWidgetUnderCursor(const MouseEvent& event, AbstractWidget* parent)
	{
		return 0;
	}

	void Context::RemoveWidgetFromHoverList (AbstractWidget* widget,
	        bool cursor_event)
	{

	}

	void Context::RemoveSubWidgetFromHoverList (AbstractContainer* container,
	        bool cursor_event)
	{

	}

	void Context::OnSubWidgetDestroyed (AbstractWidget* widget)
	{
		Remove(widget);
	}

}
