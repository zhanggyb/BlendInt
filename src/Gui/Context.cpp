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

#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt
{

	ContextLayer::ContextLayer ()
			: refresh(true), widgets(0), buffer(0)
	{
	}

	ContextLayer::~ContextLayer ()
	{
		if (buffer) {
			DBG_PRINT_MSG("%s", "Delete texture buffer in context layer");
			delete buffer;
		}

		if (widgets) {
			DBG_PRINT_MSG("%s", "Delete widget set in context layer");
			assert(widgets->size() == 0);
			widgets->clear();
			delete widgets;
		}
	}

	bool Context::refresh_once = false;

	bool Context::force_refresh_all = true;

	ScissorStatus Context::scissor_status;

	Context::Context ()
	: AbstractContainer(), m_main_buffer(0), m_vao(0), m_focused_widget(0)
	{
		set_size(640, 480);

		m_redraw_event.set_view_matrix(
						glm::lookAt(glm::vec3(0.f, 0.f, 1.f),
										glm::vec3(0.f, 0.f, 0.f),
										glm::vec3(0.f, 1.f, 0.f)));
		// default is 640 x 480
		m_redraw_event.set_projection_matrix(
						glm::ortho(0.f, 640.f, 0.f, 480.f, 100.f, -100.f));
		m_hover_deque.reset(new std::deque<AbstractWidget*>);
		m_main_buffer = new GLTexture2D;

		InitOnce();
	}

	Context::~Context ()
	{
		if(m_focused_widget) {
			SetFocusedWidget(0);
		}

		std::map<int, ContextLayer>::iterator layer_iter;
		std::set<AbstractWidget*>::iterator widget_iter;
		std::set<AbstractWidget*>* widget_set_p = 0;

		for (layer_iter = m_layers.begin(); layer_iter != m_layers.end();
		        layer_iter++)
		{
			widget_set_p = layer_iter->second.widgets;
			for (widget_iter = widget_set_p->begin();
			        widget_iter != widget_set_p->end(); widget_iter++)
			{
				(*widget_iter)->destroyed().disconnectOne(this,
				        &Context::OnSubWidgetDestroyed);
				(*widget_iter)->m_container = 0;

				if ((*widget_iter)->managed() && ((*widget_iter)->count() == 0)) {
					delete *widget_iter;
				}
			}

			widget_set_p->clear();
		}

		m_deque.clear();

		m_layers.clear();
		m_index.clear();

		if (m_main_buffer) {
			m_main_buffer->Clear();
			delete m_main_buffer;
		}

		glDeleteVertexArrays(1, &m_vao);
	}

	bool Context::Add (AbstractWidget* widget)
	{
		return AddSubWidget(widget);
	}

	bool Context::Remove (AbstractWidget* widget)
	{
		return RemoveSubWidget(widget);
	}

	int Context::GetMaxLayer () const
	{
		map<int, ContextLayer>::const_reverse_iterator rit = m_layers.rbegin();

		return rit->first;
	}

	void Context::RefreshLayer (int layer)
	{
		map<int, ContextLayer>::iterator layer_iter;

		layer_iter = m_layers.find(layer);

		if (layer_iter != m_layers.end()) {
			m_layers[layer].refresh = true;
			refresh_once = true;
		}
	}

	void Context::SetFocusedWidget (AbstractWidget* widget)
	{
		if(m_focused_widget == widget) {
			return;
		}

		if (m_focused_widget) {
			m_focused_widget->m_flag.reset(
			        AbstractWidget::WidgetFlagFocus);
			m_focused_widget->FocusEvent(false);
		}

		m_focused_widget = widget;
		if (m_focused_widget) {
			m_focused_widget->m_flag.set(
			        AbstractWidget::WidgetFlagFocus);
			m_focused_widget->FocusEvent(true);
		}
	}

	void Context::SetCursor (void* cursor)
	{
	}

	void Context::ResetCursor ()
	{

	}

	void* Context::GetCursor () const
	{
		return 0;
	}

	void Context::Update (const UpdateRequest& request)
	{
		if (request.source() == Predefined) {

			switch(request.type()) {

				case FormSize: {

					const Size* size_p = static_cast<const Size*>(request.data());

					GLfloat vertices[] = {
						0.f, 0.f,
						static_cast<GLfloat>(size_p->width()), 0.f,
						0.f, static_cast<GLfloat>(size_p->height()),
						static_cast<GLfloat>(size_p->width()), static_cast<GLfloat>(size_p->height())
					};

					glBindVertexArray(m_vao);
					m_vbo->Bind();
					m_vbo->UpdateData(vertices, sizeof(vertices));
					m_vbo->Reset();

					glBindVertexArray(0);

					m_redraw_event.set_projection_matrix(
									glm::ortho(0.f,
													static_cast<float>(size_p->width()),
													0.f,
													static_cast<float>(size_p->height()),
													100.f, -100.f));

					// TODO: redraw
					force_refresh_all = true;
					break;
				}

				case FormPosition: {
					// always at (0, 0)
					break;
				}

				case ContextRefresh: {

					const AbstractWidget* widget_p = static_cast<const AbstractWidget*>(request.data());
					// DBG_PRINT_MSG("widget %s call refresh: %d", widget_p->name().c_str(), widget_p->z());
					RefreshLayer(widget_p->z());
					break;
				}

				default:
					break;;
			}

		}
	}

	ResponseType Context::Draw (const RedrawEvent& event)
	{
		glm::vec3 pos(position().x(), position().y(), z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		m_deque.clear();

		if (force_refresh_all) {

			std::map<int, ContextLayer>::iterator layer_iter;
			unsigned int count = 0;
			std::set<AbstractWidget*>* widget_set_p = 0;

			for (layer_iter = m_layers.begin(); layer_iter != m_layers.end();
			        layer_iter++) {
				widget_set_p = layer_iter->second.widgets;

				DBG_PRINT_MSG("layer need to be refreshed: %d", layer_iter->first);

				if (!layer_iter->second.buffer) {
					layer_iter->second.buffer = new GLTexture2D;
					layer_iter->second.buffer->Generate();
				}

				RenderLayer(event,
								layer_iter->first,
								widget_set_p,
								layer_iter->second.buffer);
				m_deque.push_back(layer_iter->second.buffer);

				count++;

				layer_iter->second.refresh = false;
			}

			RenderMainBuffer(event);

			refresh_once = false;
			force_refresh_all = false;

		} else if (refresh_once) {

			std::map<int, ContextLayer>::iterator layer_iter;
			unsigned int count = 0;
			std::set<AbstractWidget*>* widget_set_p = 0;

			for (layer_iter = m_layers.begin(); layer_iter != m_layers.end();
			        layer_iter++) {
				widget_set_p = layer_iter->second.widgets;

				if (layer_iter->second.refresh) {

					// DBG_PRINT_MSG("layer need to be refreshed: %d", layer_iter->first);

					if (!layer_iter->second.buffer) {
						layer_iter->second.buffer = new GLTexture2D;
						layer_iter->second.buffer->Generate();
					}
					RenderLayer(event,
									layer_iter->first,
									widget_set_p,
									layer_iter->second.buffer);

				} else {

					if (!layer_iter->second.buffer) {
						layer_iter->second.buffer = new GLTexture2D;
						layer_iter->second.buffer->Generate();

						RenderLayer(event,
										layer_iter->first,
										widget_set_p,
										layer_iter->second.buffer);
					} else if (layer_iter->second.buffer->id() == 0) {
						layer_iter->second.buffer->Generate();

						RenderLayer(event,
										layer_iter->first,
										widget_set_p,
										layer_iter->second.buffer);
					}
				}

				count++;

				m_deque.push_back(layer_iter->second.buffer);
				layer_iter->second.refresh = false;
			}

			RenderMainBuffer(event);

			refresh_once = false;
		}

		DrawMainBuffer(mvp);

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
		if(m_hover_deque->size() == 0) {
			SetFocusedWidget(0);
		} else {
			ResponseType response;
			AbstractWidget* original_focused = m_focused_widget;
			AbstractWidget* widget_pressed = 0;
			bool focus_set_manually = false;

			m_focused_widget = 0;
			for (std::deque<AbstractWidget*>::reverse_iterator it =
							m_hover_deque->rbegin();
							it != m_hover_deque->rend(); it++) {

				response = (*it)->MousePressEvent(event);

				if(m_focused_widget && (original_focused != m_focused_widget)) {
					focus_set_manually = true;
				}

				if ( response == Accept || response == AcceptAndBreak) {
					widget_pressed = *it;
					break;
				}

				if (response == AcceptAndContinue) {
					widget_pressed = *it;
				}
			}

			if(focus_set_manually) {

				if(original_focused) {
					original_focused->m_flag.reset(
							AbstractWidget::WidgetFlagFocus);
					original_focused->FocusEvent(false);
				}

			} else {

				m_focused_widget = original_focused;
				SetFocusedWidget(widget_pressed);

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
		}

		for(std::deque<AbstractWidget*>::reverse_iterator it = m_hover_deque->rbegin();
						it != m_hover_deque->rend(); it++)
		{
			response = (*it)->MouseReleaseEvent(event);

			if(response == AcceptAndBreak) break;
		}

		return Accept;
	}

	ResponseType Context::MouseMoveEvent (const MouseEvent& event)
	{
		AbstractWidget* widget = 0;
		ResponseType response;

		// search which widget in stack contains the cursor
		while (m_hover_deque->size()) {

			if (m_hover_deque->back()->visiable() &&
							m_hover_deque->back()->Contain(event.position())) {
				widget = m_hover_deque->back();
				break;
			} else {
				m_hover_deque->back()->CursorEnterEvent(false);
				m_hover_deque->back()->m_flag.reset(AbstractWidget::WidgetFlagContextHoverList);
			}

			m_hover_deque->pop_back();
		}

		BuildCursorHoverList(event, widget);

		/*
		for (std::deque<AbstractWidget*>::iterator it =
				m_hover_deque->begin(); it != m_hover_deque->end();
				it++)
		{
			DBG_PRINT_MSG("cursor on: %s", (*it)->name().c_str());
		}
		*/

		// tell the focused widget first
		if(m_focused_widget) {
			response = m_focused_widget->MouseMoveEvent(event);

			if(response == AcceptAndBreak)
				return Accept;
			// check the event status
		}

		for (std::deque<AbstractWidget*>::reverse_iterator it =
						m_hover_deque->rbegin();
						it != m_hover_deque->rend();
				it++)
		{
			response = (*it)->MouseMoveEvent(event);
			// check the event status
			if (response == Accept) {
				// TODO: do sth
			}
		}

		return Accept;
	}

	bool Context::AddSubWidget (AbstractWidget* widget)
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

		map<int, ContextLayer>::iterator layer_iter;
		layer_iter = m_layers.find(widget->z());
		if (layer_iter != m_layers.end()) {
			layer_iter->second.widgets->insert(widget);
		} else {
			std::set<AbstractWidget*>* new_widget_set_p = new std::set<AbstractWidget*>;
			new_widget_set_p->insert(widget);
			m_layers[widget->z()].widgets = new_widget_set_p;

			// Refresh this layer in the render loop
			m_layers[widget->z()].refresh = true;
			refresh_once = true;
		}

		m_index[widget] = widget->z();

		SetContainer(widget, this);
		events()->connect(widget->destroyed(), this, &Context::OnSubWidgetDestroyed);

		return true;
	}

	bool Context::RemoveSubWidget (AbstractWidget* widget)
	{
		if(!widget) return false;

		assert(widget->container() == this);

		widget->destroyed().disconnectOne(this,
				&Context::OnSubWidgetDestroyed);

		if(widget->hover()) {
			RemoveWidgetFromHoverList(this);
		}

		if(widget->focused() && widget == m_focused_widget) {
			m_focused_widget = 0;
		}

		map<AbstractWidget*, int>::iterator index_iter;

		index_iter = m_index.find(widget);

		if (index_iter != m_index.end()) {

			int z = index_iter->second;

			std::set<AbstractWidget*>* widget_set_p = m_layers[z].widgets;
			std::set<AbstractWidget*>::iterator widget_iter = widget_set_p->find(widget);
			if (widget_iter != widget_set_p->end()) {
				widget_set_p->erase(widget_iter);
			} else {
				DBG_PRINT_MSG("Error: object %s is not recorded in set",
						widget->name().c_str());
			}

			if (widget_set_p->empty()) {
				DBG_PRINT_MSG("layer %d is empty, delete it", z);
				//delete widget_set_p; widget_set_p = 0;
				delete m_layers[z].widgets;
				m_layers[z].widgets = 0;

				if(m_layers[z].buffer) {
					m_layers[z].buffer->Clear();
					delete m_layers[z].buffer;
					m_layers[z].buffer = 0;
				}
				m_layers.erase(z);
			}

			m_index.erase(widget);

			SetContainer(widget, 0);

		} else {
			DBG_PRINT_MSG("Error: object %s is not recorded in map",
					widget->name().c_str());
			return false;
		}

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

	void Context::InitOnce ()
	{
		m_program = ShaderManager::instance->default_context_program();

		glGenVertexArrays(1, &m_vao);

		glBindVertexArray(m_vao);

		m_vbo.reset(new GLArrayBuffer);
		m_vbo->Generate();

		GLfloat vertices [] = {
				0.f, 0.f,
				static_cast<GLfloat>(size().width()), 0.f,
				0.f, static_cast<GLfloat>(size().height()),
				static_cast<GLfloat>(size().width()), static_cast<GLfloat>(size().height())
		};

		m_vbo->Bind();
		m_vbo->SetData(sizeof(vertices), vertices);
		m_vbo->Reset();

		GLfloat uv[] = {
			0.0, 1.0,
			1.0, 1.0,
			0.0, 0.0,
			1.0, 0.0
		};

		m_tbo.reset(new GLArrayBuffer);
		m_tbo->Generate();
		m_tbo->Bind();
		m_tbo->SetData(sizeof(uv), uv);
		m_tbo->Reset();

		glBindVertexArray(0);
	}

	void Context::DrawMainBuffer (const glm::mat4& mvp)
	{
		glViewport(0, 0, size().width(), size().height());

		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(m_vao);

		m_program->Use();

		m_program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));

		glActiveTexture(GL_TEXTURE0);
		m_main_buffer->Bind();

		m_program->SetUniform1i("TexID", 0);

		glEnableVertexAttribArray(0);
		m_vbo->Bind();

		glVertexAttribPointer(
				0,
				2,
				GL_FLOAT,
				GL_FALSE,
				0,
				BUFFER_OFFSET(0)
				);

		glEnableVertexAttribArray(1);
		m_tbo->Bind();
		glVertexAttribPointer(
				1,
				2,
				GL_FLOAT,
				GL_FALSE,
				0,
				BUFFER_OFFSET(0)
				);

		m_vbo->Bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		m_vbo->Reset();
		m_main_buffer->Reset();
		m_program->Reset();

		glBindVertexArray(0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Context::RenderLayer (const RedrawEvent& event,
					int layer,
					std::set<AbstractWidget*>* widgets,
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
		GL_TEXTURE_2D, tex->id(), 0);
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

			std::set<AbstractWidget*>::iterator widget_iter;

			for (widget_iter = widgets->begin(); widget_iter != widgets->end();
			        widget_iter++) {
				//(*set_it)->Draw();
				DispatchDrawEvent(*widget_iter, event);
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
	
	void Context::RenderMainBuffer (const RedrawEvent& event)
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
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->id(), 0);
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

			PreDrawContext(true);

			glViewport(0, 0, width, height);

			glm::mat4 mvp = event.projection_matrix() * event.view_matrix();

#ifdef DEBUG
			//DrawTriangle(false);
			//DrawGrid(width, height);
#endif

			GLTexture2D* texture = 0;
			std::deque<GLTexture2D*>::iterator it;
			for (it = m_deque.begin(); it != m_deque.end(); it++) {

				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

				glBindVertexArray(m_vao);

				m_program->Use();

				m_program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));

				glActiveTexture(GL_TEXTURE0);
				texture = *it;
				texture->Bind();

				m_program->SetUniform1i("TexID", 0);

				glEnableVertexAttribArray(0);
				m_vbo->Bind();

				glVertexAttribPointer(
						0,
						2,
						GL_FLOAT,
						GL_FALSE,
						0,
						BUFFER_OFFSET(0)
						);

				glEnableVertexAttribArray(1);
				m_tbo->Bind();
				glVertexAttribPointer(
						1,
						2,
						GL_FLOAT,
						GL_FALSE,
						0,
						BUFFER_OFFSET(0)
						);

				m_vbo->Bind();
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				glDisableVertexAttribArray(1);
				glDisableVertexAttribArray(0);

				m_vbo->Reset();
				texture->Reset();
				m_program->Reset();

				glBindVertexArray(0);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			}

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
		glClearColor(0.447, 0.447, 0.447, 1.00);

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

			/*
			DBG_PRINT_MSG("draw widget: %s at %d, %d, size: %u, %u",
							widget->name().c_str(),
							widget->position().x(),
							widget->position().y(),
							widget->size().width(),
							widget->size().height());
			*/

			widget->Draw(event);

			AbstractContainer* p = dynamic_cast<AbstractContainer*>(widget);
			if (p) {

				if(p->m_flag[AbstractWidget::WidgetFlagScissorTest]) {
					scissor_status.Push(p->position().x() + p->margin().left(),
							p->position().y() + p->margin().right(),
							p->size().width() - p->margin().left() - p->margin().right(),
							p->size().height() - p->margin().top() - p->margin().bottom());
				}

				IteratorPtr it = p->CreateIterator(event);

				if(scissor_status.valid()) {
					scissor_status.Enable();

					for (it->First(); !it->End(); it->Next()) {
						DispatchDrawEvent(it->GetWidget(), event);
					}

				} else {

					for (it->First(); !it->End(); it->Next()) {
						DispatchDrawEvent(it->GetWidget(), event);
					}
				}

				if(p->m_flag[AbstractWidget::WidgetFlagScissorTest]) {
					scissor_status.Pop();
					scissor_status.Disable();
				}

			}

		}
	}
	
	void Context::BuildCursorHoverList (const MouseEvent& event, AbstractWidget* parent)
	{
		if (parent) {
			parent->m_flag.set(AbstractWidget::WidgetFlagContextHoverList);

			AbstractContainer* p = dynamic_cast<AbstractContainer*>(parent);
			if(p) {

				IteratorPtr it = p->CreateIterator(event);

				for (it->First(); !it->End(); it->Next()) {
					if (it->GetWidget()->visiable() && it->GetWidget()->Contain(event.position())) {
						m_hover_deque->push_back(it->GetWidget());
						m_hover_deque->back()->CursorEnterEvent(true);
						BuildCursorHoverList(event, it->GetWidget());
						break;	// if break or continue the loop?
					}
				}
			} else {
			}
		} else {
			m_hover_deque->clear();

			std::map<int, ContextLayer>::reverse_iterator map_it;
			std::set<AbstractWidget*>::iterator set_it;
			std::set<AbstractWidget*>* set_p = 0;

			bool stop = false;

			for (map_it = m_layers.rbegin();
							map_it != m_layers.rend();
							map_it++) {
				set_p = map_it->second.widgets;
				for (set_it = set_p->begin(); set_it != set_p->end();
						set_it++) {
					if ((*set_it)->visiable() && (*set_it)->Contain(event.position())) {
						m_hover_deque->push_back(*set_it);
						m_hover_deque->back()->CursorEnterEvent(true);
						BuildCursorHoverList(event, *set_it);
						stop = true;
					}

					if (stop)
					break;
				}
				if (stop)
				break;
			}
		}
	}

	void Context::RemoveWidgetFromHoverList (AbstractWidget* widget)
	{
		while (m_hover_deque->size()) {
			m_hover_deque->back()->m_flag.reset(
			        AbstractWidget::WidgetFlagContextHoverList);

			if (m_hover_deque->back() == widget) {
				m_hover_deque->pop_back();
				break;
			}

			m_hover_deque->pop_back();
		}
	}

#ifdef DEBUG

	void Context::PrintLayers ()
	{
		std::map<int, ContextLayer>::iterator layer_iter;
		std::set<AbstractWidget*>::iterator widget_iter;

		std::set<AbstractWidget*>* widget_set_p;
		std::cout << std::endl;

		std::cerr << "size of index map:" << m_index.size() << std::endl;
		std::cerr << "size of layer map:" << m_layers.size() << std::endl;

		for (layer_iter = m_layers.begin(); layer_iter != m_layers.end();
		        layer_iter++) {
			std::cerr << "Layer: " << layer_iter->first << std::endl;
			widget_set_p = layer_iter->second.widgets;
			for (widget_iter = widget_set_p->begin();
			        widget_iter != widget_set_p->end(); widget_iter++) {
				std::cerr << (*widget_iter)->name() << " ";
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
