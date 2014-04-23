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
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <algorithm>
#include <iostream>
#include <set>
#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <OpenImageIO/imageio.h>
OIIO_NAMESPACE_USING

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/OpenGL/GLFramebuffer.hpp>

#include <BlendInt/Gui/AbstractWidget.hpp>
#include <BlendInt/Gui/AbstractLayout.hpp>

#include <BlendInt/Interface.hpp>

namespace BlendInt {

	SubWidgetProxy::SubWidgetProxy (AbstractWidget* widget)
	: m_widget(widget)
	{

	}

	SubWidgetProxy::~SubWidgetProxy ()
	{
	}

	void SubWidgetProxy::Resize (const Size& size)
	{
		if(m_widget->size() == size) return;

		UpdateRequest request(Predefined, FormSize, &size);

		if(m_widget->UpdateTest(request)) {
			m_widget->Update(request);
			m_widget->set_size(size);
		}
	}

	void SubWidgetProxy::Resize (unsigned int width, unsigned int height)
	{
		if(m_widget->size().width() == width &&
						m_widget->size().height() == height)
			return;

		Size new_size (width, height);
		UpdateRequest request(Predefined, FormSize, &new_size);

		if(m_widget->UpdateTest(request)) {
			m_widget->Update(request);
			m_widget->set_size(width, height);
		}
	}

	void SubWidgetProxy::SetPosition (int x, int y)
	{
		if(m_widget->position().x() == x && m_widget->position().y() == y) return;

		Point new_pos (x, y);
		UpdateRequest request(Predefined, FormPosition, &new_pos);

		if(m_widget->UpdateTest(request)) {
			m_widget->Update(request);
			m_widget->set_position(x, y);
		}
	}

	void SubWidgetProxy::SetPosition (const Point& position)
	{
		if(m_widget->position() == position) return;
		UpdateRequest request(Predefined, FormPosition, &position);

		if(m_widget->UpdateTest(request)) {
			m_widget->Update(request);
			m_widget->set_position(position);
		}
	}

	ContainerProxy::ContainerProxy (AbstractContainer* container)
	: m_container(container)
	{
	}

	ContainerProxy::~ContainerProxy ()
	{
	}

	bool ContainerProxy::RequestRefreshTest (AbstractWidget* widget)
	{
		UpdateRequest request(Predefined, WidgetRefresh, widget);
		return m_container->UpdateTest(request);
	}

	void ContainerProxy::RequestRefresh (AbstractWidget* widget)
	{
		UpdateRequest request(Predefined, WidgetRefresh, widget);
		m_container->Update(request);
	}

	bool ContainerProxy::SubwidgetPositionUpdateTest(AbstractWidget* widget, const Point& pos)
	{
		SubWidgetPositionData data = {widget, &pos};
		UpdateRequest request (Predefined, SubWidgetPosition, &data);

		return m_container->UpdateTest(request);
	}

	bool ContainerProxy::SubWidgetSizeUpdateTest(AbstractWidget* widget, const Size& size)
	{
		SubWidgetSizeData data = {widget, &size};
		UpdateRequest request (Predefined, SubWidgetSize, &data);

		return m_container->UpdateTest(request);
	}

	void ContainerProxy::SubWidgetPositionUpdate(AbstractWidget* widget, const Point& pos)
	{
		SubWidgetPositionData data = {widget, &pos};
		UpdateRequest request (Predefined, SubWidgetPosition, &data);
		m_container->Update(request);
	}

	void ContainerProxy::SubWidgetSizeUpdate(AbstractWidget* widget, const Size& size)
	{
		SubWidgetSizeData data = {widget, &size};
		UpdateRequest request (Predefined, SubWidgetSize, &data);
		m_container->Update (request);
	}

	// --------------------------------------------------------------------

	AbstractWidget::AbstractWidget ()
	: AbstractExtraForm(),
	m_z(0),
	m_container(0)
	{
		m_events.reset(new Cpp::ConnectionScope);
		m_flag.set(WidgetFlagVisibility);
	}

	AbstractWidget::~AbstractWidget ()
	{
		m_destroyed.fire(this);
	}

	void AbstractWidget::Resize (unsigned int width, unsigned int height)
	{
		if(size().width() == width && size().height() == height) return;

		Size new_size (width, height);
		UpdateRequest request(Predefined, FormSize, &new_size);

		// TODO: ask for container
		if(ResizeTestInContainer(new_size) && UpdateTest(request)) {
			Update(request);
			set_size(width, height);
			ResizeUpdateInContainer(new_size);
			fire_property_changed_event(FormSize);
		}
	}

	void AbstractWidget::Resize (const Size& size)
	{
		if(AbstractWidget::size() == size) return;

		UpdateRequest request(Predefined, FormSize, &size);

		if(ResizeTestInContainer(size) && UpdateTest(request)) {
			Update(request);
			set_size(size);
			ResizeUpdateInContainer(size);
			fire_property_changed_event(FormSize);
		}
	}

	void AbstractWidget::SetPosition (int x, int y)
	{
		if(position().x() == x && position().y() == y) return;

		Point new_pos (x, y);
		UpdateRequest request(Predefined, FormPosition, &new_pos);

		if(PositionTestInContainer(new_pos) && UpdateTest(request)) {
			Update(request);
			set_position(x, y);
			PositionUpdateInContainer(new_pos);
			fire_property_changed_event(FormPosition);
		}
	}

	void AbstractWidget::SetPosition (const Point& pos)
	{
		if(position() == pos) return;

		UpdateRequest request(Predefined, FormPosition, &pos);

		if(PositionTestInContainer(pos) && UpdateTest(request)) {
			Update(request);
			set_position(pos);
			PositionUpdateInContainer(pos);
			fire_property_changed_event(FormPosition);
		}
	}

	void AbstractWidget::SetPreferredSize(unsigned int width, unsigned int height)
	{
		// check the param first
		if (width < minimal_size().width() ||
				height < minimal_size().height() ||
				width > maximal_size().width() ||
				height > maximal_size().height())
		return;

		if(preferred_size().width() == width && preferred_size().height() == height) return;

		Size new_pref_size(width, height);
		UpdateRequest request(Predefined, FormPreferredSize, &new_pref_size);

		if(UpdateTest(request)) {
			Update(request);
			set_preferred_size(width, height);
			fire_property_changed_event(FormPreferredSize);
		}
	}

	void AbstractWidget::SetPreferredSize(const Size& size)
	{
		if (size.width() < minimal_size().width() ||
				size.height() < minimal_size().height()||
				size.width() > maximal_size().width() ||
				size.height() > maximal_size().height())
		return;

		if(preferred_size() == size) return;
		UpdateRequest request(Predefined, FormPreferredSize, &size);

		if(UpdateTest(request)) {
			Update(request);
			set_preferred_size(size);
			fire_property_changed_event(FormPreferredSize);
		}
	}

	void AbstractWidget::SetMinimalSize(unsigned int width, unsigned int height)
	{
		if(width > preferred_size().width() ||
				height > preferred_size().height())
		return;

		if(minimal_size().width() == width && minimal_size().height() == height) return;

		Size new_min_size(width, height);
		UpdateRequest request (Predefined, FormMinimalSize, &new_min_size);

		if(UpdateTest(request)) {
			Update(request);
			set_minimal_size(width, height);
			fire_property_changed_event(FormMinimalSize);
		}
	}

	void AbstractWidget::SetMinimalSize(const Size& size)
	{
		if(size.width() > preferred_size().width() ||
				size.height() > preferred_size().height())
		return;

		if (minimal_size() == size) return;
		UpdateRequest request (Predefined, FormMinimalSize, &size);

		if(UpdateTest(request)) {
			Update(request);
			set_minimal_size(size);
			fire_property_changed_event(FormMinimalSize);
		}
	}

	void AbstractWidget::SetMaximalSize(unsigned int width, unsigned int height)
	{
		if(width < preferred_size().width() ||
				height < preferred_size().height())
		return;

		if(maximal_size().width() == width && maximal_size().height() == height) return;

		Size new_max_size (width, height);
		UpdateRequest request(Predefined, FormMaximalSize, &new_max_size);

		if(UpdateTest(request)) {
			Update(request);
			set_maximal_size(new_max_size);
			fire_property_changed_event(FormMaximalSize);
		}
	}

	void AbstractWidget::SetMaximalSize(const Size& size)
	{
		if(size.width() < preferred_size().width() ||
				size.height() < preferred_size().height())
		return;

		if(maximal_size() == size) return;
		UpdateRequest request(Predefined, FormMaximalSize, &size);

		if(UpdateTest(request)) {
			Update(request);
			set_maximal_size(size);
			fire_property_changed_event(FormMaximalSize);
		}
	}

	void AbstractWidget::SetLayer (int z)
	{
		if(m_z == z) return;

		UpdateRequest request(Predefined, WidgetLayer, &z);

		if(UpdateTest (request)) {
			Update(request);

			Context* context = GetContext();
			if(context) {
				context->Remove(this);
				m_z = z;
				context->Add(this);
			} else {
				m_z = z;
			}

			fire_property_changed_event(WidgetLayer);
		}
	}

	void AbstractWidget::SetVisible (bool visible)
	{
		if(m_flag[WidgetFlagVisibility] == visible)
			return;

		UpdateRequest request(Predefined, WidgetVisibility, &visible);

		if(UpdateTest (request)) {
			Update(request);
			m_flag[WidgetFlagVisibility] = visible ? 1 : 0;
			fire_property_changed_event(WidgetVisibility);
		}
	}

	void AbstractWidget::Refresh()
	{
		if(RefreshTestInContainer()) {
			RefreshInContainer();
		}
	}

	void AbstractWidget::RenderToTexture (size_t border, GLTexture2D* texture)
	{
		if(!texture) return;

		GLsizei width = size().width() + border * 2;
		GLsizei height = size().height() + border * 2;

		// Create and set texture to render to.
		GLTexture2D* tex = texture;
		if(!tex->id())
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
				GL_TEXTURE_2D, tex->id(), 0);
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

			glm::mat4 view = glm::lookAt(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
			glm::mat4 projection = glm::ortho(0.f, (float)width, 0.f, (float)height, 100.f, -100.f);
			glm::mat4 offset = glm::translate(glm::mat4(1.0), glm::vec3(border, border, 0.0));

			RedrawEvent event;
			event.set_projection_matrix(projection);
			event.set_view_matrix(view * offset);

            GLint vp[4];
            glGetIntegerv(GL_VIEWPORT, vp);
			glViewport(0, 0, width, height);

			Draw(event);

			glViewport(vp[0], vp[1], vp[2], vp[3]);
		}

		fb->Reset();

		tex->Reset();
		delete tex; tex = 0;

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glDeleteRenderbuffers(1, &rb);

		fb->Reset();
		delete fb; fb = 0;
	}


	void AbstractWidget::RenderToFile(const char* filename, unsigned int border)
	{
		GLsizei width = size().width() + border * 2;
		GLsizei height = size().height() + border * 2;

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
				GL_TEXTURE_2D, tex->id(), 0);
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

			glm::mat4 view = glm::lookAt(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
			glm::mat4 projection = glm::ortho(0.f, (float)width, 0.f, (float)height, 100.f, -100.f);
			glm::mat4 offset = glm::translate(glm::mat4(1.0), glm::vec3(border, border, 0.0));

			RedrawEvent event;
			event.set_projection_matrix(projection);
			event.set_view_matrix(view * offset);

            GLint vp[4];
            glGetIntegerv(GL_VIEWPORT, vp);

			glViewport(0, 0, width, height);

			Draw(event);

			glViewport(vp[0], vp[1], vp[2], vp[3]);

			// ---------------------------------------------
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

	/*
	void AbstractWidget::SetPosition(AbstractWidget* obj, int x, int y)
	{
		if(obj->position().x() == x && obj->position().y() == y) return;

		Point new_pos (x, y);

		if(obj->Update(UpdateRequest(Predefined, FormPosition, &new_pos))) {
			obj->set_position(x, y);
		}
	}

	void AbstractWidget::SetPosition(AbstractWidget* obj, const Point& pos)
	{
		if(obj->position() == pos) return;

		if(obj->Update(UpdateRequest(Predefined, FormPosition, &pos))) {
			obj->set_position(pos);
		}
	}

	void AbstractWidget::Resize (AbstractWidget* obj, unsigned int width, unsigned int height)
	{
		if(obj->size().width() == width && obj->size().height() == height) return;

		Size new_size (width, height);

		if(obj->Update(UpdateRequest(Predefined, FormSize, &new_size))) {
			obj->set_size(width, height);
		}
	}

	void AbstractWidget::Resize (AbstractWidget* obj, const Size& size)
	{
		if(obj->size() == size) return;

		if(obj->Update(UpdateRequest(Predefined, FormSize, &size))) {
			obj->set_size(size);
		}
	}
	*/

	void AbstractWidget::DispatchRender(AbstractWidget* other)
	{
		//other->Draw();
	}

	ResponseType AbstractWidget::dispatch_key_press_event (AbstractWidget* obj,
			const KeyEvent& event)
	{
		return obj->KeyPressEvent(event);
	}

	ResponseType AbstractWidget::dispatch_mouse_move_event (AbstractWidget* obj,
			const MouseEvent& event)
	{
		return obj->MouseMoveEvent(event);
	}

	ResponseType AbstractWidget::dispatch_mouse_press_event (AbstractWidget* obj,
			const MouseEvent& event)
	{
		return obj->MousePressEvent(event);
	}

	ResponseType AbstractWidget::dispatch_mouse_release_event (AbstractWidget* obj,
			const MouseEvent& event)
	{
		return obj->MouseReleaseEvent(event);
	}

	bool AbstractWidget::ResizeTestInContainer (const Size& size)
	{
		if(m_container) {
			ContainerProxy test(m_container);

			return test.SubWidgetSizeUpdateTest(this, size);
		} else {
			return true;
		}
	}

	bool AbstractWidget::PositionTestInContainer(const Point& point)
	{
		if(m_container) {

			ContainerProxy test(m_container);
			return test.SubwidgetPositionUpdateTest(this, point);

		} else {
			return true;
		}
	}

	void AbstractWidget::ResizeUpdateInContainer(const Size& size)
	{
		if(m_container) {
			ContainerProxy proxy(m_container);
			proxy.SubWidgetSizeUpdate(this, size);
		}
	}

	void AbstractWidget::PositionUpdateInContainer(const Point& pos)
	{
		if(m_container) {
			ContainerProxy proxy(m_container);
			proxy.SubWidgetPositionUpdate(this, pos);
		}
	}

	Context* AbstractWidget::GetContext()
	{
		AbstractContainer* container = m_container;

		if(container == 0) {
			return dynamic_cast<Context*>(this);
		} else {

			while(container->container()) {
				container = container->container();
			}

		}

		return dynamic_cast<Context*>(container);
	}

	bool AbstractWidget::RefreshTestInContainer()
	{
		bool ret = false;

		if(m_container) {
			ContainerProxy proxy(m_container);
			ret = proxy.RequestRefreshTest(this);
		}

		return ret;
	}

	void AbstractWidget::RefreshInContainer()
	{
		if(m_container) {
			ContainerProxy proxy(m_container);
			proxy.RequestRefresh(this);
		}
	}

	bool AbstractWidget::CompositeToScreenBuffer (GLTexture2D* tex,
			unsigned int border)
	{
		bool result = false;

		GLsizei width = size().width() + border * 2;
		GLsizei height = size().height() + border * 2;

		GLuint fb;
		//RGBA8 RenderBuffer, 24 bit depth RenderBuffer, 256x256
		glGenFramebuffers(1, &fb);
		glBindFramebuffer(GL_FRAMEBUFFER, fb);

		//Create and attach a color buffer
		GLuint color_rb;
		glGenRenderbuffers(1, &color_rb);
		//We must bind color_rb before we call glRenderbufferStorageEXT
		glBindRenderbuffer(GL_RENDERBUFFER, color_rb);
		//The storage format is RGBA8
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
		//Attach color buffer to FBO
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color_rb);

		//-------------------------
		GLuint depth_rb;
		glGenRenderbuffers(1, &depth_rb);
		glBindRenderbuffer(GL_RENDERBUFFER, depth_rb);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

		//-------------------------
		//Attach depth buffer to FBO
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,
				GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_rb);

		//-------------------------

		if(GLFramebuffer::CheckStatus()) {

			//and now you can render to the FBO (also called RenderBuffer)
			glBindFramebuffer(GL_FRAMEBUFFER, fb);

			glClearColor(0.0, 0.0, 0.0, 0.0);

			glClearDepth(1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_BLEND);

			glViewport(0, 0, width, height);

			/*
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0.f, (float) width, 0.f, (float) height, 100.f, -100.f);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glPushMatrix();

			glTranslatef(border, border, 0);
			*/

			//Draw();

			/*
			 GLubyte pixels[width * height * 4];
			 glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

			 ImageOutput* out = ImageOutput::create("image.png");

			 if(out) {
			 ImageSpec spec (width, height, 4, TypeDesc::UINT8);
			 out->open("image.png", spec);
			 out->write_image(TypeDesc::UINT8, pixels);
			 out->close();
			 delete out;
			 }
			 */
			tex->Bind();
			tex->CopySubimage(0, position().x(), position().y(), 0, 0, width, height);
			tex->Reset();

			//glPopMatrix();

			result = true;
		}

		//----------------
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		//Bind 0, which means render to back buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDeleteRenderbuffers(1, &depth_rb);
		glDeleteRenderbuffers(1, &color_rb);
		glDeleteFramebuffers(1, &fb);

		return result;
	}

} /* namespace BlendInt */
