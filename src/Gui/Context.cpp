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

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/OpenGL/GLFramebuffer.hpp>
#include <BlendInt/OpenGL/GLRenderbuffer.hpp>

#include <BlendInt/Gui/Context.hpp>

#include <BlendInt/Service/ShaderManager.hpp>

#include "../Intern/ScreenBuffer.hpp"

namespace BlendInt
{

	ContextLayerExt::ContextLayerExt ()
			: refresh(true), widgets(0), buffer(0)
	{
	}

	ContextLayerExt::~ContextLayerExt ()
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
	: AbstractContainerExt(), m_main_buffer(0), m_screenbuffer(0), m_vao(0)
	{
		set_size(640, 480);

		InitOnce();
	}

	Context::~Context ()
	{
		if (AbstractWidgetExt::focused_widget) {
			AbstractWidgetExt::focused_widget->m_flag.reset(
			        AbstractWidgetExt::WidgetFlagFocusExt);
			AbstractWidgetExt::focused_widget = 0;
		}

		std::map<int, ContextLayerExt>::iterator layer_iter;
		std::set<AbstractWidgetExt*>::iterator widget_iter;
		std::set<AbstractWidgetExt*>* widget_set_p = 0;

		for (layer_iter = m_layers.begin(); layer_iter != m_layers.end();
		        layer_iter++)
		{
			widget_set_p = layer_iter->second.widgets;
			for (widget_iter = widget_set_p->begin();
			        widget_iter != widget_set_p->end(); widget_iter++)
			{
				(*widget_iter)->destroyed().disconnectOne(this,
				        &Context::OnSubWidgetDestroyed);

				if ((*widget_iter)->managed()) {

					// Delete the widget if it's not referenced by any RefPtr
					if((*widget_iter)->count() == 0)
						delete *widget_iter;
				} else {

					(*widget_iter)->destroyed().disconnectOne(this,
					        &Context::OnSubWidgetDestroyed);
					(*widget_iter)->m_container = 0;
					//(*widget_iter)->m_flag.reset(
					   //     AbstractWidget::WidgetFlagInContextManager);

				}
			}

			widget_set_p->clear();
		}

		m_deque.clear();

		if (m_screenbuffer) {
			delete m_screenbuffer;
		}

		m_layers.clear();
		m_index.clear();

		if (m_main_buffer) {
			m_main_buffer->Clear();
			delete m_main_buffer;
		}

		glDeleteVertexArrays(1, &m_vao);
	}

	bool Context::Update (const UpdateRequest& request)
	{
		if (request.source() == Predefined) {

			switch(request.type()) {

				case FormSize: {

					// Nothing to do but accept
					return true;
				}

				case FormPosition: {
					// Nothing to do but accept
					return true;
				}

				default:
					return false;
			}

		} else {

			return false;
		}
	}

	ResponseType Context::Draw (const RedrawEvent& event)
	{
		glClearColor(0.125, 0.25, 0.45, 1.00);

		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT |
						GL_DEPTH_BUFFER_BIT |
						GL_STENCIL_BUFFER_BIT);

		/*
		glm::vec3 pos((float) position().x(), (float) position().y(),
						(float) z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);


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
		*/

		/*
		m_deque.clear();

		if (force_refresh_all) {

			std::map<int, ContextLayerExt>::iterator layer_iter;
			unsigned int count = 0;
			std::set<AbstractWidgetExt*>* widget_set_p = 0;

			for (layer_iter = m_layers.begin(); layer_iter != m_layers.end();
			        layer_iter++) {
				widget_set_p = layer_iter->second.widgets;

				DBG_PRINT_MSG("layer need to be refreshed: %d", layer_iter->first);

				if (!layer_iter->second.buffer) {
					layer_iter->second.buffer = new GLTexture2D;
					layer_iter->second.buffer->Generate();
				}

				OffScreenRenderToTexture(event,
								layer_iter->first,
								widget_set_p,
								layer_iter->second.buffer);
				m_deque.push_back(layer_iter->second.buffer);

				count++;

				layer_iter->second.refresh = false;
			}

			RenderToScreenBuffer(event);

			refresh_once = false;
			force_refresh_all = false;

		} else if (refresh_once) {

			std::map<int, ContextLayerExt>::iterator layer_iter;
			unsigned int count = 0;
			std::set<AbstractWidgetExt*>* widget_set_p = 0;

			for (layer_iter = m_layers.begin(); layer_iter != m_layers.end();
			        layer_iter++) {
				widget_set_p = layer_iter->second.widgets;

				if (layer_iter->second.refresh) {

					// DBG_PRINT_MSG("layer need to be refreshed: %d", layer_iter->first);

					if (!layer_iter->second.buffer) {
						layer_iter->second.buffer = new GLTexture2D;
						layer_iter->second.buffer->Generate();
					}
					OffScreenRenderToTexture(event,
									layer_iter->first,
									widget_set_p,
									layer_iter->second.buffer);

				} else {

					if (!layer_iter->second.buffer) {
						layer_iter->second.buffer = new GLTexture2D;
						layer_iter->second.buffer->Generate();

						OffScreenRenderToTexture(event,
										layer_iter->first,
										widget_set_p,
										layer_iter->second.buffer);
					} else if (layer_iter->second.buffer->id() == 0) {
						layer_iter->second.buffer->Generate();

						OffScreenRenderToTexture(event,
										layer_iter->first,
										widget_set_p,
										layer_iter->second.buffer);
					}
				}

				count++;

				m_deque.push_back(layer_iter->second.buffer);
				layer_iter->second.refresh = false;
			}

			RenderToScreenBuffer(event);

			refresh_once = false;
		}

		m_screenbuffer->Render(event.projection_matrix() * event.view_matrix(),
							   m_main_buffer);

		*/
		return Accept;
	}

	ResponseType Context::CursorEnterEvent (bool entered)
	{
		return Accept;
	}

	ResponseType Context::KeyPressEvent (const KeyEvent& event)
	{
		return Accept;
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
		return Accept;
	}

	ResponseType Context::MouseReleaseEvent (const MouseEvent& event)
	{
		return Accept;
	}

	ResponseType Context::MouseMoveEvent (const MouseEvent& event)
	{
		return Accept;
	}

	bool Context::AddSubWidget (AbstractWidgetExt* widget)
	{
		if(!widget)
			return false;

		if (widget->container()) {

			if (widget->container() == this) {
				DBG_PRINT_MSG("Widget %s is already in container %s",
				        widget->name().c_str(),
				        widget->container()->name().c_str());
				return true;
			} else {
				// Set widget's container to 0
				AbstractContainerExt::RemoveSubWidget(widget->container(),
				        widget);
			}
		}

		map<int, ContextLayerExt>::iterator layer_iter;
		layer_iter = m_layers.find(widget->z());
		if (layer_iter != m_layers.end()) {
			layer_iter->second.widgets->insert(widget);
		} else {
			std::set<AbstractWidgetExt*>* new_widget_set_p = new std::set<AbstractWidgetExt*>;
			new_widget_set_p->insert(widget);
			m_layers[widget->z()].widgets = new_widget_set_p;

			// Refresh this layer in the render loop
			m_layers[widget->z()].refresh = true;
			refresh_once = true;
		}

		m_index[widget] = widget->z();

		events()->connect(widget->destroyed(), this, &Context::OnSubWidgetDestroyed);

		return true;
	}

	bool Context::RemoveSubWidget (AbstractWidgetExt* widget)
	{
		if(!widget) return false;

		assert(widget->container() == this);

		widget->destroyed().disconnectOne(this,
				&Context::OnSubWidgetDestroyed);

		if (AbstractWidgetExt::focused_widget == widget) {
			widget->m_flag.reset(AbstractWidgetExt::WidgetFlagFocusExt);
			AbstractWidgetExt::focused_widget = 0;
		}

		map<AbstractWidgetExt*, int>::iterator index_iter;

		index_iter = m_index.find(widget);

		if (index_iter != m_index.end()) {

			int z = index_iter->second;

			std::set<AbstractWidgetExt*>* widget_set_p = m_layers[z].widgets;
			std::set<AbstractWidgetExt*>::iterator widget_iter = widget_set_p->find(widget);
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

		} else {
			DBG_PRINT_MSG("Error: object %s is not recorded in map",
					widget->name().c_str());
			return false;
		}

		return true;
	}

	void Context::InitOnce ()
	{
		m_program = ShaderManager::instance->default_context_program();

		m_screenbuffer = new ScreenBuffer;
		m_hover_deque.reset(new std::deque<AbstractWidgetExt*>);

		m_main_buffer = new GLTexture2D;

		glGenVertexArrays(1, &m_vao);

		glBindVertexArray(m_vao);

		m_vbo.reset(new GLArrayBuffer);
		m_vbo->Generate();

		GLfloat vertices [] = {
				0.f, 0.f,
				size().width(), 0.f,
				0.f, size().height(),
				size().width(), size().height()
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

	RefPtr<AbstractContainerIterator> Context::First (const DeviceEvent& event)
	{
		// A Context object only interacts with Interface
		RefPtr<AbstractContainerIterator> ret;

		return ret;
	}

	bool Context::End (const DeviceEvent& event, AbstractContainerIterator* iter)
	{
		// A Context object only interacts with Interface
		return false;
	}

	int Context::GetMaxLayer () const
	{
		map<int, ContextLayerExt>::const_reverse_iterator rit = m_layers.rbegin();

		return rit->first;
	}

	void Context::RefreshLayer (int layer)
	{
		map<int, ContextLayerExt>::iterator layer_iter;

		layer_iter = m_layers.find(layer);

		if (layer_iter != m_layers.end()) {
			m_layers[layer].refresh = true;
			refresh_once = true;
		}
	}
	
	void Context::OffScreenRenderToTexture (const RedrawEvent& event,
					int layer,
					std::set<AbstractWidgetExt*>* widgets,
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

			std::set<AbstractWidgetExt*>::iterator widget_iter;

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
	
	void Context::RenderToScreenBuffer (const RedrawEvent& event)
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

			DBG_PRINT_MSG("viewport size: %u, %u", width, height);

			glViewport(0, 0, width, height);

			glm::mat4 mvp = event.projection_matrix() * event.view_matrix();

#ifdef DEBUG
			//DrawTriangle(false);
			//DrawGrid(width, height);
#endif

			std::deque<GLTexture2D*>::iterator it;
			for (it = m_deque.begin(); it != m_deque.end(); it++) {
				m_screenbuffer->Render(mvp, *it);
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
	
	void Context::DispatchDrawEvent (AbstractWidgetExt* widget,
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

			AbstractContainerExt* p = dynamic_cast<AbstractContainerExt*>(widget);
			if (p) {

				if(p->m_flag[AbstractWidgetExt::WidgetFlagScissorTestExt]) {
					scissor_status.Push(p->position().x() + p->margin().left(),
							p->position().y() + p->margin().right(),
							p->size().width() - p->margin().left() - p->margin().right(),
							p->size().height() - p->margin().top() - p->margin().bottom());
				}

				if(scissor_status.valid()) {
					scissor_status.Enable();

					for (RefPtr<AbstractContainerIterator> it =
					        p->First(event); !(p->End(event, it.get()));
					        it->Next()) {
						DispatchDrawEvent(it->GetWidget(), event);
					}

				} else {
					for (RefPtr<AbstractContainerIterator> it =
									p->First(event); !(p->End(event, it.get()));
					        it->Next()) {
						DispatchDrawEvent(it->GetWidget(), event);
					}
				}

				if(p->m_flag[AbstractWidgetExt::WidgetFlagScissorTestExt]) {
					scissor_status.Pop();
					scissor_status.Disable();
				}

			}

		}
	}
	
	bool Context::Add (AbstractWidgetExt* widget)
	{
		return AddSubWidget(widget);
	}
	
	bool Context::Remove (AbstractWidgetExt* widget)
	{
		return RemoveSubWidget(widget);
	}

	void Context::OnSubWidgetDestroyed (AbstractWidgetExt* widget)
	{
		DBG_PRINT_MSG("Sub widget %s is destroyed outside of the context manager", widget->name().c_str());

		RemoveSubWidget(widget);

		//widget->m_flag.reset(AbstractWidget::WidgetFlagInContextManager);
		//widget->destroyed().disconnectOne(this, &Context::OnSubWidgetDestroyed);
	}

}
