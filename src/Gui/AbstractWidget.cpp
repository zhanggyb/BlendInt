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

#include <BlendInt/Service/ContextManager.hpp>
#include <BlendInt/Interface.hpp>

namespace BlendInt {

	using namespace std;

	// --------------------------------------------------------------------

	AbstractWidget* AbstractWidget::focused_widget = 0;

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
		if(hover()) {
			// ContextManager::instance->RemoveWidgetFromHoverDeque(this);
		}

		if(focused()) {
			//ContextManager::instance->SetFocusedWidget(0);
		}

		m_destroyed.fire(this);
	}

	void AbstractWidget::Resize (unsigned int width, unsigned int height)
	{
		// If the object is managed by a layout, disallow position setting
		if(locked()) return;

		if(size().width() == width && size().height() == height) return;

		Size new_size (width, height);

		if(Update(UpdateRequest(Predefined, FormSize, &new_size))) {
			set_size(width, height);
			fire_property_changed_event(FormSize);
		}
	}

	void AbstractWidget::Resize (const Size& size)
	{
		// If the object is managed by a layout, disallow position setting
		if(locked()) return;

		if(AbstractWidget::size() == size) return;

		if(Update(UpdateRequest(Predefined, FormSize, &size))) {
			set_size(size);
			fire_property_changed_event(FormSize);
		}
	}

	void AbstractWidget::SetPosition (int x, int y)
	{
		// If the object is managed by a layout, disallow position setting
		if(locked()) return;

		if(position().x() == x && position().y() == y) return;

		Point new_pos (x, y);

		if(Update(UpdateRequest(Predefined, FormPosition, &new_pos))) {
			set_position(x, y);
			fire_property_changed_event(FormPosition);
		}
	}

	void AbstractWidget::SetPosition (const Point& pos)
	{
		// If the object is managed by a layout, disallow position setting
		if(locked()) return;

		if(position() == pos) return;

		if(Update(UpdateRequest(Predefined, FormPosition, &pos))) {
			set_position(pos);
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

		if(Update(UpdateRequest(Predefined, FormPreferredSize, &new_pref_size))) {
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

		if(Update(UpdateRequest(Predefined, FormPreferredSize, &size))) {
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

		if(Update(UpdateRequest(Predefined, FormMinimalSize, &new_min_size))) {
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

		if(Update(UpdateRequest(Predefined, FormMinimalSize, &size))) {
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

		if(Update(UpdateRequest(Predefined, FormMaximalSize, &new_max_size))) {
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

		if(Update(UpdateRequest(Predefined, FormMaximalSize, &size))) {
			set_maximal_size(size);
			fire_property_changed_event(FormMaximalSize);
		}
	}

	void AbstractWidget::SetLayer (int z)
	{
		// If the object is managed by a layout, disallow position setting
		if(locked()) return;

		if(m_z == z) return;

		if(m_flag[WidgetFlagInContextManager]) {
			if(Update (UpdateRequest(Predefined, WidgetLayer, &z))) {
				//ContextManager::instance->Unregister(this);
				m_z = z;
				//ContextManager::instance->AddSubWidget(this);
				fire_property_changed_event(WidgetLayer);
			}
		} else {
			if(Update (UpdateRequest(Predefined, WidgetLayer, &z))) {
				m_z = z;
				fire_property_changed_event(WidgetLayer);
			}
		}
	}

	void AbstractWidget::SetVisible (bool visible)
	{
		if(m_flag[WidgetFlagVisibility] == visible)
			return;

		if(Update (UpdateRequest(Predefined, WidgetVisibility, &visible))) {
			m_flag[WidgetFlagVisibility] = visible ? 1 : 0;
			fire_property_changed_event(WidgetVisibility);
		}
	}

	void AbstractWidget::Refresh()
	{
		//if(!m_flag[WidgetFlagInContextManager])
		//return;

		ContextManager::instance->RefreshLayer(m_z);
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

			glViewport(0, 0, width, height);

			Draw(event);

			glViewport(0,
					0,
					Interface::instance->GetCurrentContextWidth(),
					Interface::instance->GetCurrentContextHeight());
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

			glViewport(0, 0, width, height);

			Draw(event);

			glViewport(0,
					0,
					Interface::instance->GetCurrentContextWidth(),
					Interface::instance->GetCurrentContextHeight());

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

