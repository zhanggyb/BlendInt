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
	  m_main_buffer(0),
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

		m_main_buffer = new GLTexture2D;
#ifdef DEBUG
		m_main_buffer->set_name("Main Buffer");
#endif

		InitializeContext();

		context_set.insert(this);
	}

	Context::~Context ()
	{
		if(m_focused_widget) {
			SetFocusedWidget(0);
		}

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
			layer_iter->second.m_hover_list->m_widgets.clear();
		}

		m_deque.clear();

		m_layers.clear();

		if (m_main_buffer) {
			m_main_buffer->Clear();
			delete m_main_buffer;
		}

		glDeleteVertexArrays(1, &m_vao);

		context_set.erase(this);
	}

	bool Context::Add (AbstractWidget* widget)
	{
		return InsertSubWidget(widget);
	}

	bool Context::Remove (AbstractWidget* widget)
	{
		if(widget->container() != this)
			return false;

		return RemoveSubWidget(widget);
	}

	int Context::GetMaxLayer () const
	{
		std::map<int, ContextLayer>::const_reverse_iterator rit = m_layers.rbegin();

		return rit->first;
	}

	void Context::RefreshLayer (int layer)
	{
		std::map<int, ContextLayer>::iterator layer_iter;

		layer_iter = m_layers.find(layer);

		if (layer_iter != m_layers.end()) {
			m_layers[layer].m_refresh = true;
			refresh_once = true;
		}
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
				RefreshLayer(request.source()->z());
				break;
			}

			case ContainerSubWidgetAdded: {
				const AbstractWidget* widget =
				        static_cast<const AbstractWidget*>(request.data());
				DBG_PRINT_MSG("Container \"%s\" add sub widget \"%s\"",
				        request.source()->name().c_str(),
				        widget->name().c_str());

				// TODO: check the layer

				break;
			}

			case ContainerSubWidgetRemoved: {
				const AbstractWidget* widget =
				        static_cast<const AbstractWidget*>(request.data());
				DBG_PRINT_MSG("Container \"%s\" remove sub widget \"%s\"",
				        request.source()->name().c_str(),
				        widget->name().c_str());

				// TODO: check the layer

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
		DBG_PRINT_MSG("source widget: %s, target widget: %s", request.source()->name().c_str(), request.target()->name().c_str());

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
					break;
				}

				case WidgetSize: {
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

			DBG_PRINT_MSG("%s", "get widget geometry change update");

			DBG_PRINT_MSG("source widget: %s, target widget: %s", request.source()->name().c_str(), request.target()->name().c_str());

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
		glm::vec3 pos(position().x(), position().y(), z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		m_deque.clear();

		if (force_refresh_all) {

			for (std::map<int, ContextLayer>::iterator it = m_layers.begin();
					it != m_layers.end();
			        it++)
			{
				//DBG_PRINT_MSG("layer need to be refreshed: %d", it->first);
				RenderToLayerBuffer(event,
						it->first,
						it->second.m_widget_set->m_widgets,
						it->second.m_texture_buffer.get());
				m_deque.push_back(it->second.m_texture_buffer.get());
				it->second.m_refresh = false;
			}

			//if(m_deque.size() >= minimal_composite_layer_number) {
				RenderToMainBuffer(event);
			//}

			refresh_once = false;
			force_refresh_all = false;

		} else if (refresh_once) {

			for (std::map<int, ContextLayer>::iterator it = m_layers.begin();
					it != m_layers.end();
			        it++)
			{
				if (it->second.m_refresh) {

					// DBG_PRINT_MSG("layer need to be refreshed: %d", layer_iter->first);
					RenderToLayerBuffer(event,
									it->first,
									it->second.m_widget_set->m_widgets,
									it->second.m_texture_buffer.get());

				}
				m_deque.push_back(it->second.m_texture_buffer.get());
				it->second.m_refresh = false;
			}

			//if(m_deque.size() >= minimal_composite_layer_number) {
				RenderToMainBuffer(event);
			//}

			refresh_once = false;
		}

		//if(m_deque.size() >= minimal_composite_layer_number) {
			DrawMainBuffer(mvp);
		//} else {
		//	DrawLayers(mvp);
		//}

		return Accept;
	}

	ResponseType Context::CursorEnterEvent (bool entered)
	{
		return Ignore;
	}

	ResponseType Context::KeyPressEvent (const KeyEvent& event)
	{
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
		//bool ret = true;

		for(std::map<int, ContextLayer>::reverse_iterator it = m_layers.rbegin();
				it != m_layers.rend();
				it++)
		{
			if(DispatchMousePressEvent(it->first, event)) {
				break;
			}
		}

		return Accept;
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

		for(std::map<int, ContextLayer>::reverse_iterator it = m_layers.rbegin();
				it != m_layers.rend();
				it++)
		{
			if(DispatchMouseReleaseEvent(it->first, event)) break;
		}

		return Accept;
	}

	ResponseType Context::MouseMoveEvent (const MouseEvent& event)
	{
		ResponseType response;

		m_redraw_event.set_cursor_position(event.position());

		// tell the focused widget first
		if(m_focused_widget) {
			response = m_focused_widget->MouseMoveEvent(event);

			if(response == AcceptAndBreak)
				return Accept;
			// check the event status
		}

		for(std::map<int, ContextLayer>::reverse_iterator it = m_layers.rbegin();
				it != m_layers.rend();
				it++)
		{

			if(DispatchMouseMoveEvent(it->first, event)) {
				return Accept;
			}
		}

		return Ignore;
	}

	bool Context::InsertSubWidget (AbstractWidget* widget)
	{
		if(!widget)
			return false;

		if (widget->container()) {

			if (widget->container() == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s",
				        widget->name().c_str(),
				        widget->container()->name().c_str());
				return false;
			} else {
				// Set widget's container to 0
				AbstractContainer::RemoveSubWidget(widget->container(),
				        widget);
			}
		}

		// TODO: the widget new added may be under the cursor

		std::map<int, ContextLayer>::iterator it = m_layers.find(widget->z());

		if (it != m_layers.end()) {

			it->second.m_widget_set->m_widgets.insert(widget);

		} else {

			// Create a new Context Layer

			ContextLayer new_layer;

			new_layer.m_widget_set->m_widgets.insert(widget);
			if(m_tex_buffer_cache.size()) {
				new_layer.m_texture_buffer = m_tex_buffer_cache.top();
				m_tex_buffer_cache.pop();
			} else {
				new_layer.m_texture_buffer->Generate();
			}

			m_layers[widget->z()] = new_layer;

#ifdef DEBUG
			char name[32];
			sprintf(name, "layer %d buffer", widget->z());
			m_layers[widget->z()].m_texture_buffer->set_name(name);
#endif

			refresh_once = true;
		}

		// set shadow
		if(widget->drop_shadow()) {
			if(!widget->m_shadow) {
				widget->m_shadow.reset(new Shadow);
			}

			widget->m_shadow->Resize(widget->size());
		}

		SetContainer(widget, this);
		events()->connect(widget->destroyed(), this, &Context::OnSubWidgetDestroyed);

		return true;
	}

	bool Context::RemoveSubWidget (AbstractWidget* widget)
	{
		if(!widget) return false;

		assert(widget->container() == this);
		assert(m_layers.count(widget->z()));

		widget->destroyed().disconnectOne(this,
				&Context::OnSubWidgetDestroyed);

		if(widget->hover()) {
			RemoveWidgetFromHoverList(this);
		}

		if(widget->focused()) {
			widget->set_focus(false);

			if(widget == m_focused_widget) {
				m_focused_widget = 0;
			}
		}

		int z = widget->z();

		ContextLayer& layer = m_layers[z];
		if(layer.m_widget_set->m_widgets.erase(widget) == 0) {
			DBG_PRINT_MSG("Error: object %s is not recorded in context layer", widget->name().c_str());
		}

		if(layer.m_widget_set->m_widgets.empty()) {

			DBG_PRINT_MSG("layer %d is empty, delete it", z);

			// Check and clear hover list
			for(AbstractWidgetDeque::reverse_iterator it = layer.m_hover_list->m_widgets.rbegin();
					it != layer.m_hover_list->m_widgets.rend();
					it++)
			{
				(*it)->set_hover(false);
			}

			if(m_tex_buffer_cache.size() < m_max_tex_buffer_cache_size) {
				m_tex_buffer_cache.push(layer.m_texture_buffer);
			}

			m_layers.erase(z);

		}

		SetContainer(widget, 0);

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

	void Context::DrawMainBuffer (const glm::mat4& mvp)
	{
		RefPtr<GLSLProgram> program = Stock::Shaders::instance->default_context_program();

		glClearColor(0.208f, 0.208f, 0.208f, 1.f);

		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT |
						GL_DEPTH_BUFFER_BIT |
						GL_STENCIL_BUFFER_BIT);
		// Here cannot enable depth test -- glEnable(GL_DEPTH_TEST);

		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glViewport(0, 0, size().width(), size().height());

		program->Use();
		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));

		glActiveTexture(GL_TEXTURE0);
		m_main_buffer->Bind();

		program->SetUniform1i("TexID", 0);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		GLArrayBuffer::Reset();
		m_main_buffer->Reset();

		program->Reset();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Context::DrawLayers (const glm::mat4& mvp)
	{
		RefPtr<GLSLProgram> program = Stock::Shaders::instance->default_context_program();

		glViewport(0, 0, size().width(), size().height());

		glClearColor(0.447, 0.447, 0.447, 1.00);
		glClearDepth(1.0);

		glClear(GL_COLOR_BUFFER_BIT |
						GL_DEPTH_BUFFER_BIT |
						GL_STENCIL_BUFFER_BIT);

		// Here cannot enable depth test -- glEnable(GL_DEPTH_TEST);

		//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glEnable(GL_BLEND);

		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));

		glActiveTexture(GL_TEXTURE0);

		program->SetUniform1i("TexID", 0);
		for(std::deque<GLTexture2D*>::iterator it = m_deque.begin(); it != m_deque.end(); it++)
		{
			(*it)->Bind();

			glBindVertexArray(m_vao);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);

			GLArrayBuffer::Reset();

			(*it)->Reset();
		}

		program->Reset();

		glBindVertexArray(0);
	}

	void Context::RenderToLayerBuffer (const RedrawEvent& event,
					int layer,
					const std::set<AbstractWidget*>& widgets,
					GLTexture2D* texture)
	{
		GLsizei width = size().width();
		GLsizei height = size().height();

		// Create and set texture to render to.
		GLTexture2D* tex = texture;
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

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width,
		        height);

		//Attach depth buffer to FBO
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, rb);

		if (GLFramebuffer::CheckStatus()) {

			fb->Bind();

			glClearColor(0.0, 0.0, 0.0, 0.0);

			glClearDepth(1.0);
			glClear(GL_COLOR_BUFFER_BIT |
					GL_DEPTH_BUFFER_BIT |
					GL_STENCIL_BUFFER_BIT);

			// Here cannot enable depth test -- glEnable(GL_DEPTH_TEST);
			glBlendFuncSeparate(GL_SRC_ALPHA,
								GL_ONE_MINUS_SRC_ALPHA,
								GL_ONE,
								GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_BLEND);

			glViewport(0, 0, width, height);

			for (std::set<AbstractWidget*>::iterator it = widgets.begin();
					it != widgets.end();
			        it++)
			{
				//(*set_it)->Draw();
				DispatchDrawEvent(*it, event);
			}

			// uncomment the code below to test the layer buffer (texture)
			/*
			 std::string str("layer");
			 char buf[4];
			 sprintf(buf, "%d", layer);
			 str = str + buf + ".png";
			 tex->WriteToFile(str);
			 */
		}

		fb->Reset();
		tex->Reset();

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glDeleteRenderbuffers(1, &rb);

		fb->Reset();
		delete fb;
		fb = 0;
	}
	
	void Context::RenderToMainBuffer (const RedrawEvent& event)
	{
		GLsizei width = size().width();
		GLsizei height = size().height();

		// Create and set texture to render to.
		GLTexture2D* tex = m_main_buffer;
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
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->texture(), 0);
		//fb->Attach(*tex, GL_COLOR_ATTACHMENT0);

		GLuint rb = 0;
		glGenRenderbuffers(1, &rb);

		glBindRenderbuffer(GL_RENDERBUFFER, rb);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width,
		        height);

		//Attach depth buffer to FBO
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, rb);

		if (GLFramebuffer::CheckStatus()) {

			RefPtr<GLSLProgram> program = Stock::Shaders::instance->default_context_program();
			fb->Bind();

			glClearColor(0.0, 0.0, 0.0, 0.0);

			glClearDepth(1.0);
			glClear(GL_COLOR_BUFFER_BIT |
							GL_DEPTH_BUFFER_BIT |
							GL_STENCIL_BUFFER_BIT);
			// Here cannot enable depth test -- glEnable(GL_DEPTH_TEST);

			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);

			glViewport(0, 0, width, height);

			glm::mat4 mvp = event.projection_matrix() * event.view_matrix();

#ifdef DEBUG
			//DrawTriangle(false);
			//DrawGrid(width, height);
#endif

			program->Use();
			glActiveTexture(GL_TEXTURE0);

			program->SetUniform1i("TexID", 0);

			glBindVertexArray(m_vao);

			for (std::deque<GLTexture2D*>::iterator it = m_deque.begin(); it != m_deque.end(); it++) {
				program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
				(*it)->Bind();
				//(*it)->WriteToFile("layer2.png");
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				mvp = glm::translate(mvp, glm::vec3(0.0, 0.0, 10.0));
			}

			glBindVertexArray(0);

			GLTexture2D::Reset();
			GLArrayBuffer::Reset();
			program->Reset();

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		fb->Reset();
		tex->Reset();

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glDeleteRenderbuffers(1, &rb);

		fb->Reset();
		delete fb;
		fb = 0;
	}
	
	void Context::PreDrawContext (bool fbo)
	{
		glClearColor(0.447, 0.447, 0.447, 0.00);

		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT |
						GL_DEPTH_BUFFER_BIT |
						GL_STENCIL_BUFFER_BIT);

		// Here cannot enable depth test -- glEnable(GL_DEPTH_TEST);

		if (fbo) {
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE,
			        GL_ONE_MINUS_SRC_ALPHA);
		} else {
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		glEnable(GL_BLEND);
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
	
	bool Context::DispatchMousePressEvent (int layer, const MouseEvent& event)
	{
		// TODO: the widgets under cursor may be changed before mouse press

		std::deque<AbstractWidget*>& deque = m_layers[layer].m_hover_list->m_widgets;

		ResponseType response;
		AbstractWidget* widget = 0;
		AbstractWidget* original_focused_widget = m_focused_widget;
		bool focus_widget_changed = false;

		if (deque.size()) {
			std::cout << "-------------------------------------------------" << std::endl;
			for (std::deque<AbstractWidget*>::iterator it =
			        deque.begin(); it != deque.end(); it++) {
				DBG_PRINT_MSG("cursor on: %s", (*it)->name().c_str());
			}
		}

		for(std::deque<AbstractWidget*>::reverse_iterator it = deque.rbegin();
				it != deque.rend();
				it++)
		{
			widget = *it;
			response = widget->MousePressEvent(event);

			if(original_focused_widget != m_focused_widget) {
				focus_widget_changed = true;
			}

			if(response == Accept || response == AcceptAndBreak) {
				break;
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

		return widget != 0;
	}

	bool Context::DispatchMouseReleaseEvent (int layer, const MouseEvent& event)
	{
		return false;
	}

	bool Context::DispatchMouseMoveEvent(int layer, const MouseEvent& event)
	{
		std::deque<AbstractWidget*>& deque = m_layers[layer].m_hover_list->m_widgets;
		AbstractWidget* widget = 0;

		// search which widget in stack contains the cursor
		while (deque.size()) {

			if (deque.back()->visiable() &&
					deque.back()->Contain(event.position())) {
				widget = deque.back();
				break;
			} else {
				deque.back()->CursorEnterEvent(false);
				deque.back()->set_hover(false);
			}

			deque.pop_back();
		}

		if(widget) {
			AppendCursorHoverList(deque, widget);
		} else {
			BuildCursorHoverList(layer);
		}

		// DEBUG:
		/*
		if (deque.size()) {
			std::cout << "-------------------------------------------------" << std::endl;
			for (std::deque<AbstractWidget*>::iterator it =
			        deque.begin(); it != deque.end(); it++) {
				DBG_PRINT_MSG("cursor on: %s", (*it)->name().c_str());
			}
		}
		*/

		ResponseType response;
		for (std::deque<AbstractWidget*>::iterator it = deque.begin();
		        it != deque.end(); it++)
		{
			response = (*it)->MouseMoveEvent(event);
			// check the event status
			if (response == AcceptAndBreak || response == Ignore) {
				// TODO: do sth
			}
		}

		return (deque.size() > 0);
	}

	void Context::BuildCursorHoverList(int layer)
	{
		if(!m_layers.count(layer)) return;

		std::set<AbstractWidget*>& widget_set = m_layers[layer].m_widget_set->m_widgets;
		std::deque<AbstractWidget*>& hover_list = m_layers[layer].m_hover_list->m_widgets;

		hover_list.clear();

		for(std::set<AbstractWidget*>::iterator it = widget_set.begin();
				it != widget_set.end();
				it++)
		{
			if((*it)->visiable() && (*it)->Contain(m_redraw_event.cursor_position())) {
				hover_list.push_back(*it);
				(*it)->CursorEnterEvent(true);

				// append
				break;
			}
		}
	}

	void Context::AppendCursorHoverList (std::deque<AbstractWidget*>& deque,
	        AbstractWidget* parent)
	{
		parent->set_hover(true);

		AbstractContainer* p = dynamic_cast<AbstractContainer*>(parent);

		if(p) {

			IteratorPtr it = p->CreateIterator(m_redraw_event);
			AbstractWidget* widget = 0;

			for (it->GoToFirst(); !it->IsEnd(); it->GoNext()) {

				widget = it->GetWidget();

				if (widget->visiable() && widget->Contain(m_redraw_event.cursor_position())) {
					deque.push_back(widget);
					widget->CursorEnterEvent(true);
					AppendCursorHoverList(deque, widget);
					break;	// if break or continue the loop?
				}

			}

		}
	}


	AbstractWidget* Context::GetWidgetUnderCursor(const MouseEvent& event, AbstractWidget* parent)
	{
		AbstractContainer* p = dynamic_cast<AbstractContainer*>(parent);

		if(p) {

			AbstractWidget* widget = 0;
			IteratorPtr it = p->CreateIterator(event);

			for (it->GoToFirst(); !it->IsEnd(); it->GoNext()) {
				widget = it->GetWidget();
				if (widget->visiable() && widget->Contain(event.position())) {
					break;
				} else {
					widget = 0;
				}
			}

			if(widget) {
				return GetWidgetUnderCursor(event, widget);
			} else {
				return parent;
			}

		} else {

			return parent;

		}
	}

	void Context::RemoveWidgetFromHoverList (AbstractWidget* widget,
	        bool cursor_event)
	{
		std::deque<AbstractWidget*>& deque = m_layers[widget->z()].m_hover_list->m_widgets;

		while(deque.size()) {

			if(deque.back() == widget) {

				if(cursor_event) widget->CursorEnterEvent(false);
				widget->set_hover(false);
				deque.pop_back();
				break;
			}

			if(cursor_event) deque.back()->CursorEnterEvent(false);
			deque.back()->set_hover(false);
			deque.pop_back();
		}
	}

	void Context::RemoveSubWidgetFromHoverList (AbstractContainer* container,
	        bool cursor_event)
	{
		std::deque<AbstractWidget*>& deque = m_layers[container->z()].m_hover_list->m_widgets;

		while(deque.size()) {

			if(deque.back() == container) {
				break;
			}

			if(cursor_event) deque.back()->CursorEnterEvent(false);
			deque.back()->set_hover(false);
			deque.pop_back();
		}
	}

#ifdef DEBUG

	void Context::PrintLayers ()
	{
		std::map<int, ContextLayer>::iterator layer_iter;
		std::set<AbstractWidget*>::iterator widget_iter;

		std::cout << std::endl;

		std::cerr << "size of layer map:" << m_layers.size() << std::endl;

		for (layer_iter = m_layers.begin(); layer_iter != m_layers.end();
		        layer_iter++) {
			std::cerr << "Layer: " << layer_iter->first << std::endl;
			for (std::set<AbstractWidget*>::iterator it = layer_iter->second.m_widget_set->m_widgets.begin();
			        it != layer_iter->second.m_widget_set->m_widgets.end();
			        it++)
			{
				std::cerr << (*it)->name() << " ";
			}
			std::cerr << std::endl;
		}
	}

#endif

	void Context::OnSubWidgetDestroyed (AbstractWidget* widget)
	{
		RemoveSubWidget(widget);
	}

}
