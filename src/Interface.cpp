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
 * License along with BlendInt.	 If not, see
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

#include <OpenImageIO/imageio.h>
OIIO_NAMESPACE_USING

#include <BlendInt/Interface.hpp>

#include <BlendInt/Core/Size.hpp>

#ifdef USE_FONTCONFIG
#include <BlendInt/Core/FontConfig.hpp>
#endif

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/OpenGL/GLFramebuffer.hpp>
#include <BlendInt/OpenGL/GLRenderbuffer.hpp>

#include <BlendInt/Gui/FontCache.hpp>
#include <BlendInt/Gui/AbstractWidget.hpp>
#include <BlendInt/Gui/AbstractContainer.hpp>

#include <BlendInt/Window/KeyEvent.hpp>
#include <BlendInt/Window/MouseEvent.hpp>
#include <BlendInt/Window/ContextMenuEvent.hpp>

#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>
#include <BlendInt/Stock/Icons.hpp>

namespace BlendInt {

	Interface* Interface::instance = 0;

	bool Interface::Initialize ()
	{
		using Stock::Icons;
		using Stock::Shaders;

		/*
		 glewExperimental = true;	// Needed in core profile
		 if (glewInit() != GLEW_OK) {
		 std::cerr << "Fail to initialize GLEW" << endl;
		 return false;
		 }

		 if (!GLEW_VERSION_2_0) {
		 std::cerr << "No support for OpenGL 2.0 found" << std::endl;
		 return false;
		 }
		 */

		bool success = true;

#ifdef DEBUG
		int major, minor;
		GetGLVersion(&major, &minor);
		DBG_PRINT_MSG("OpenGL version: %d.%d", major, minor);
		GetGLSLVersion(&major, &minor);
		DBG_PRINT_MSG("OpenGL shading language version: %d.%d", major, minor);
#endif

		if (!instance) {
			instance = new Interface();
		}

		if (!instance) {
			success = false;
		}

#ifdef USE_FONTCONFIG
		if (success && FontConfig::initialize()) {

			/*
			 FontConfig* ftconfig = FontConfig::instance();
			 if (!ftconfig->loadDefaultFontToMem()) {
			 cerr << "Cannot load default font into memory" << endl;
			 success = false;
			 }
			 */

		} else {

			DBG_PRINT_MSG("%s", "Cannot initialize FontConfig");
			success = false;

		}
#endif

		if (success && Theme::Initialize()) {
			// do nothing
		} else {
			DBG_PRINT_MSG("%s", "Cannot initialize Themes");
			success = false;
		}

		if (success && Shaders::Initialize()) {
			// do nothing
		} else {
			DBG_PRINT_MSG("%s", "The Shader Manager is not initialized successfully!");
			success = false;
		}

		if (success && Icons::Initialize()) {
			// do nothing
		} else {
			DBG_PRINT_MSG("%s", "Cannot initialize Stock Icons");
			success = false;
		}

#ifdef USE_FONTCONFIG

#ifdef __APPLE__

		// Create a default font
		//FontCache::Create("Sans-Serif", 9, 96, false, false);

#endif

#ifdef __LINUX__

		// Create a default font
		//FontCache::Create("Sans", 9, 96, false, false);

#endif

#endif

		return success;
	}

	void Interface::Release ()
	{
		using Stock::Icons;
		using Stock::Shaders;

		while(Context::context_set.size()) {
			std::set<Context*>::iterator it = Context::context_set.begin();

			if((*it)->managed() && ((*it)->reference_count() == 0)) {
				DBG_PRINT_MSG("%s", "Delete context");
				delete (*it);	// this will erase the context from the set
			} else {
				Context::context_set.erase(it);
			}
		}

		Icons::Release();
		Shaders::Release();
		Theme::Release();
		FontCache::ReleaseAll();

#ifdef USE_FONTCONFIG
		FontConfig::release();
#endif

		if (instance) {
			delete instance;
			instance = 0;
		}
	}

	Interface::Interface ()
	: m_focus_style(FocusOnClick), m_current_context(0)
	{
		m_events.reset(new Cpp::ConnectionScope);
	}

	Interface::~Interface ()
	{
		/*
		if(m_current_context) {
			if(m_current_context->managed() &&
							(m_current_context->reference_count() == 0)) {
				m_current_context->destroyed().disconnectOne(this, &Interface::OnContextDestroyed);
				delete m_current_context;
			}
		}
		*/
	}

	void Interface::Resize (const Size& size)
	{
		if(m_current_context) {
			m_current_context->Resize(size);
		}
	}

	void Interface::ResizeContext(Context* context, const Size& size)
	{
		context->Resize(size);
	}

	void Interface::Resize (int width, int height)
	{
		if(m_current_context) {
			m_current_context->Resize(width, height);
		}
	}

	void Interface::ResizeContext(Context* context, int width, int height)
	{
		context->Resize(width, height);
	}

	void Interface::Draw ()
	{

		if(m_current_context) {
			m_current_context->Draw(m_current_context->profile_);
		}
	}

	void Interface::Draw (const Profile& profile)
	{
		if(m_current_context) {
			m_current_context->Draw(profile);
		}
	}

	void Interface::DrawContext (Context* context, const Profile& profile)
	{
		ResponseType response = context->Draw(profile);

		if(response == Accept) {
			// DO nothing;
		}
	}

	void Interface::DispatchKeyEvent (Context* context, const KeyEvent& event)
	{
		switch (event.action()) {

			case KeyPress: {
				context->KeyPressEvent(event);
				break;
			}

			case KeyRelease: {
				// item->KeyReleaseEvent(dynamic_cast<BlendInt::KeyEvent*>(event));
				//cm->m_focus->KeyReleaseEvent(event);
				break;
			}

			case KeyRepeat: {
				// item->KeyRepeatEvent(&event);
				break;
			}

			default:
			break;
		}
	}

	void Interface::DispatchKeyEvent (const KeyEvent& event)
	{
		/*
		if(AbstractWidget::focused_widget) {
			switch (event.action()) {

				case KeyPress: {
#ifdef DEBUG
					if(event.key() == Key_F6 && event.text().empty()) {
						//DrawToOffScreen();
						//RenderToImage();
					}
#endif
					AbstractWidget::focused_widget->KeyPressEvent(event);
					break;
				}

				case KeyRelease: {
					// item->KeyReleaseEvent(dynamic_cast<BlendInt::KeyEvent*>(event));
					//cm->m_focus->KeyReleaseEvent(event);
					break;
				}

				case KeyRepeat: {
					// item->KeyRepeatEvent(&event);
					break;
				}

				default:
				break;
			}
		}
		*/
		if(m_current_context) {
			DispatchKeyEvent(m_current_context, event);
		}
	}

	void Interface::DispatchMouseEvent (Context* context, const MouseEvent& event)
	{
		switch (event.action()) {

			case MouseMove: {
				//DispatchCursorMoveEvent(event);
				context->MouseMoveEvent(event);
				return;
			}

			case MousePress: {
				//DispatchMousePressEvent(event);
				context->MousePressEvent(event);
				return;
			}

			case MouseRelease: {
				//DispatchMouseReleaseEvent(event);
				context->MouseReleaseEvent(event);
				return;
			}

			default:
				break;
		}
	}


	void Interface::DispatchMouseEvent (const MouseEvent& event)
	{
		if(m_current_context) {
			DispatchMouseEvent(m_current_context, event);
		}
	}

	void Interface::RenderToImage()
	{
        if(!m_current_context) return;

		GLsizei width = m_current_context->size().width();
		GLsizei height = m_current_context->size().height();

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

		//-------------------------

//		//Attach depth buffer to FBO
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				GL_RENDERBUFFER, rb);

		//-------------------------
		//Does the GPU support current FBO configuration?
		GLenum status;
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		switch (status) {
			case GL_FRAMEBUFFER_COMPLETE:
			DBG_PRINT_MSG("%s", "good");
			break;
			default:
			DBG_PRINT_MSG("%s", "Fail to check framebuffer status");
			break;
		}

		//-------------------------
		//and now render to GL_TEXTURE_2D
		fb->Bind();

		Draw();

		//Bind 0, which means render to back buffer
		fb->Reset();

		tex->WriteToFile("output.png");
		tex->Reset();

		//Delete resources
		delete tex; tex = 0;

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glDeleteRenderbuffers(1, &rb);

		//Bind 0, which means render to back buffer, as a result, fb is unbound
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		fb->Reset();
		delete fb; fb = 0;

		//Draw();
	}

	void Interface::GetGLVersion (int *major, int *minor)
	{
		const char* verstr = (const char*) glGetString(GL_VERSION);
		if((verstr == NULL) || (sscanf(verstr, "%d.%d", major, minor) != 2)) {
			*major = *minor = 0;
			fprintf(stderr, "Invalid GL_VERSION format!!!\n");
		}
	}

	void Interface::SetCurrentContext(Context* context)
	{
		if(m_current_context) {
			m_current_context->destroyed().disconnectOne(this, &Interface::OnCurrentContextDestroyed);
		}

		m_current_context = context;
		m_events->connect(m_current_context->destroyed(), this, &Interface::OnCurrentContextDestroyed);
	}

	void Interface::OnCurrentContextDestroyed(AbstractWidget* context)
	{
		if(context == m_current_context) {
			m_current_context->destroyed().disconnectOne(this, &Interface::OnCurrentContextDestroyed);
			m_current_context = 0;
		}
	}

	int Interface::GetCurrentContextWidth() const
	{
		int w = 0;

		if(m_current_context) {
			w = m_current_context->size().width();
		}

		return w;
	}

	int Interface::GetCurrentContextHeight() const
	{
		int h = 0;

		if(m_current_context) {
			h = m_current_context->size().height();
		}

		return h;
	}

	void Interface::GetGLSLVersion (int *major, int *minor)
	{
		int gl_major, gl_minor;
		GetGLVersion(&gl_major, &gl_minor);

		*major = *minor = 0;
		if(gl_major == 1) {
			/* GL v1.x can provide GLSL v1.00 only as an extension */
			const char* extstr = (const char*)glGetString(GL_EXTENSIONS);
			if((extstr != NULL) && (strstr(extstr, "GL_ARB_shading_language_100") != NULL))
			{
				*major = 1;
				*minor = 0;
			}
		} else if (gl_major >= 2) {
			/* GL v2.0 and greater must parse the version string */
			const char* verstr = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

			if((verstr == NULL) || (sscanf(verstr, "%d.%d", major, minor) != 2))
			{
				*major = *minor = 0;
				fprintf(stderr, "Invalid GL_SHADING_LANGUAGE_VERSION format!!!\n");
			}
		}
	}

}
