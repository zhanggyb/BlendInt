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
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <iostream>

#include <OpenImageIO/imageio.h>
OIIO_NAMESPACE_USING

#include <BlendInt/Interface.hpp>

#include <BlendInt/Core/Size.hpp>
#include <BlendInt/Core/FontConfig.hpp>

#include <BlendInt/OpenGL/GLTexture2D.hpp>
#include <BlendInt/OpenGL/GLFramebuffer.hpp>
#include <BlendInt/OpenGL/GLRenderbuffer.hpp>

#include <BlendInt/Gui/FontCache.hpp>
#include <BlendInt/Gui/AbstractWidget.hpp>

#include <BlendInt/Service/Theme.hpp>
#include <BlendInt/Service/ShaderManager.hpp>
#include <BlendInt/Window/KeyEvent.hpp>
#include <BlendInt/Window/MouseEvent.hpp>
#include <BlendInt/Window/ContextMenuEvent.hpp>
#include <BlendInt/Service/ContextManager.hpp>
#include <BlendInt/Service/StockIcon.hpp>

#include "Intern/ScreenBuffer.hpp"

namespace BlendInt {

	Interface* Interface::interface = 0;

	Interface* Interface::Instance ()
	{
		if (!interface) {
			std::cerr
				<< "The Interface Library is not initialized successfully! Exit"
				<< std::endl;
			exit(EXIT_FAILURE);
		}

		return interface;
	}

	bool Interface::Initialize ()
	{
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

		if (!interface) {
			interface = new Interface();
		}

		if (!interface)
			success = false;

		if (success && FontConfig::initialize()) {

			/*
			FontConfig* ftconfig = FontConfig::instance();
			if (!ftconfig->loadDefaultFontToMem()) {
				cerr << "Cannot load default font into memory" << endl;
				success = false;
			}
			*/

		} else {

			std::cerr << "Cannot initialize FontConfig" << std::endl;
			success = false;

		}

		if (success && ThemeManager::initialize()) {
			// do nothing
		} else {
			std::cerr << "Cannot initialize Themes" << std::endl;
			success = false;
		}

		if (success && ShaderManager::Initialize()) {
			// do nothing
		} else {
			std::cerr << "Cannot initialize Shader Manager" << std::endl;
			success = false;
		}

		if (success && StockIcon::Initialize()) {
			// do nothing
		} else {
			std::cerr << "Cannot initialize Stock Icons" << std::endl;
			success = false;
		}

		if (success && ContextManager::Initialize()) {
			// do nothing
		} else {
			std::cerr << "Cannot initialize Context Manager" << std::endl;
			success = false;
		}

		return success;
	}

	void Interface::Release ()
	{

		ContextManager::Release();
		StockIcon::Release();
		ShaderManager::Release();
		ThemeManager::release();
		FontCache::releaseAll();
		FontConfig::release();

		if (interface) {
			delete interface;
			interface = 0;
		}
	}

	Interface::Interface ()
	: m_main(0), m_focus_style(FocusOnClick)
	{
		m_events.reset(new Cpp::ConnectionScope);
		m_screenbuffer = new ScreenBuffer;
	}

	Interface::~Interface ()
	{
		if(m_screenbuffer) delete m_screenbuffer;
	}

	const Size& Interface::size () const
	{
		return m_size;
	}

	void Interface::Resize (const Size& size)
	{
		m_size = size;
		m_resized.fire(m_size.width(), m_size.height());

		m_screenbuffer->Resize((float)size.width(), (float)size.height());

		if(m_main) {
			AbstractWidget::Resize(m_main, size);
		}

		AbstractWidget::refresh_all = true;
	}

	void Interface::Resize (unsigned int width, unsigned int height)
	{
		m_size.set_width(width);
		m_size.set_height(height);

		m_resized.fire(m_size.width(), m_size.height());

		m_screenbuffer->Resize((float)width, (float)height);

		if(m_main) {
			AbstractWidget::Resize(m_main, width, height);
		}

		AbstractWidget::refresh_all = true;
	}

	void Interface::Draw ()
	{
		if(AbstractWidget::refresh_all) {
			RenderToScreenBuffer();
		}

		m_screenbuffer->Render();

		AbstractWidget::refresh_all = false;
	}

	void Interface::PreDrawContext (bool fbo)
	{
		glClearColor(0.447, 0.447, 0.447, 1.00);

		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Here cannot enable depth test -- glEnable(GL_DEPTH_TEST);

		if(fbo) {
			glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		} else {
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		glEnable(GL_BLEND);
	}

	void Interface::DrawContext()
	{
		int width = m_size.width();
		int height = m_size.height();

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.f, (float) width, 0.f, (float) height, 100.f, -100.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

#ifdef DEBUG
		//DrawTriangle(false);
		draw_grid(width, height);
#endif

		map<int, set<AbstractWidget*>* >::iterator map_it;
		set<AbstractWidget*>::iterator set_it;

		for(map_it = ContextManager::context_manager->m_layers.begin();
				map_it != ContextManager::context_manager->m_layers.end();
				map_it++)
		{
			set<AbstractWidget*>* pset = map_it->second;
			for (set_it = pset->begin(); set_it != pset->end(); set_it++)
			{
				//(*set_it)->Draw();
				DispatchDrawEvent(*set_it);
			}
		}
		// m_ticktack = m_ticktack ? 0 : 1;

		//glDisable(GL_BLEND);
	}

#ifdef DEBUG

	void Interface::draw_grid (int width, int height)
	{
		// Draw grid for debug
		const int small_step = 20;
		const int big_step = 100;

		glLineWidth(1);
		glEnable(GL_LINE_STIPPLE);

		glColor4f(1.0f, 1.0f, 1.0f, 0.1f);
		glLineStipple(1, 0xAAAA);

		for (int num = 1; num < width; num++) {
			int step = num * small_step;
			glBegin(GL_LINES);
			glVertex2i(0, step);
			glVertex2i(width, step);
			glEnd();

		}

		for (int num = 1; num < height; num++) {
			int step = num * small_step;
			glBegin(GL_LINES);
			glVertex2i(step, 0);
			glVertex2i(step, height);
			glEnd();
		}

		glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
		glLineStipple(1, 0xAAAA);

		for (int num = 1; num < width; num++) {
			int step = num * big_step;
			glBegin(GL_LINES);
			glVertex2i(0, step);
			glVertex2i(width, step);
			glEnd();
		}

		for (int num = 1; num < height; num++) {
			int step = num * big_step;
			glBegin(GL_LINES);
			glVertex2i(step, 0);
			glVertex2i(step, height);
			glEnd();
		}

		glDisable(GL_LINE_STIPPLE);

	}

#endif	// DEBUG

	void Interface::SetMainWidget(AbstractWidget* widget)
	{
		m_main = widget;

		if(m_main) {
			AbstractWidget::Resize(m_main, m_size);
		}
	}

	void Interface::DispatchKeyEvent (KeyEvent* event)
	{
		if(!event) return;

		ContextManager* cm = ContextManager::context_manager;

		if(AbstractWidget::focused_widget) {
			switch (event->action()) {

				case KeyPress: {
#ifdef DEBUG
					if(event->key() == Key_F6 && event->text().empty()) {
						DrawToOffScreen();
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
	}

	void Interface::DispatchMouseEvent (MouseEvent* event)
	{
		if (!event)
			return;

		switch (event->action()) {

			case MouseMove: {
				DispatchCursorMoveEvent(event);
				return;
			}

			case MousePress: {
				DispatchMousePressEvent(event);
				return;
			}

			case MouseRelease: {
				DispatchMouseReleaseEvent(event);
				return;
			}

			default:
				break;
		}
	}

	void Interface::RenderToImage()
	{
		GLsizei width = m_size.width();
		GLsizei height = m_size.height();

		// Create and set texture to render to.
		GLTexture2D* tex = new GLTexture2D;
		tex->Generate();
		tex->Bind();
		tex->SetWrapMode(GL_REPEAT, GL_REPEAT);
		tex->SetMinFilter(GL_NEAREST);
		tex->SetMagFilter(GL_NEAREST);
		tex->SetImage(width, height, 0);

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
				std::cout << "good" << std::endl;
				break;
			default:
				std::cerr << "Fail to check framebuffer status" << std::endl;
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

	void Interface::RenderToScreenBuffer ()
	{
		GLsizei width = m_size.width();
		GLsizei height = m_size.height();

		// Create and set texture to render to.
		GLTexture2D* tex = m_screenbuffer->m_texture;
		tex->Generate();
		tex->Bind();
		tex->SetWrapMode(GL_REPEAT, GL_REPEAT);
		tex->SetMinFilter(GL_NEAREST);
		tex->SetMagFilter(GL_NEAREST);
		tex->SetImage(width, height, 0);

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
			PreDrawContext(true);
			DrawContext();

		}

		fb->Reset();
		tex->Reset();

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glDeleteRenderbuffers(1, &rb);

		fb->Reset();
		delete fb; fb = 0;

	}

	void Interface::DrawToOffScreen()
	{
		GLsizei width = m_size.width();
		GLsizei height = m_size.height();

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
		//Does the GPU support current FBO configuration?
		GLenum status;
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		switch (status) {
			case GL_FRAMEBUFFER_COMPLETE:
				std::cout << "good" << std::endl;
				break;
			default:
				break;
		}

		//-------------------------
		//and now you can render to the FBO (also called RenderBuffer)
		glBindFramebuffer(GL_FRAMEBUFFER, fb);

		PreDrawContext(true);
		DrawContext();

		//-------------------------
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

		//----------------
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		//Bind 0, which means render to back buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDeleteRenderbuffers(1, &depth_rb);
		glDeleteRenderbuffers(1, &color_rb);
		glDeleteFramebuffers(1, &fb);
	}

	void Interface::DispatchCursorMoveEvent (MouseEvent* event)
	{
		AbstractWidget* widget = 0;
		ContextManager* cm = ContextManager::context_manager;

		// build a stack contians the mouse cursor
		if (cm->m_hover_deque->size()) {

			// search which widget in stack contains the cursor
			while (cm->m_hover_deque->size()) {

				if (cm->m_hover_deque->back()->contain(event->position())) {
					widget = cm->m_hover_deque->back();
					break;
				} else {
					cm->m_hover_deque->back()->CursorEnterEvent(false);
					cm->m_hover_deque->back()->m_flag.reset(AbstractWidget::WidgetFlagContextHoverList);
				}

				cm->m_hover_deque->pop_back();
			}
		}

		BuildWidgetListAtCursorPoint(event->position(), widget);

		for (std::deque<AbstractWidget*>::reverse_iterator it =
		        cm->m_hover_deque->rbegin(); it != cm->m_hover_deque->rend();
		        it++) {
			(*it)->MouseMoveEvent(event);
		}

		if (event->accepted()) {
			// TODO: do sth
		}
	}

	void Interface::DispatchMousePressEvent(MouseEvent* event)
	{
		ContextManager* cm = ContextManager::context_manager;

		for(std::deque<AbstractWidget*>::reverse_iterator it = cm->m_hover_deque->rbegin(); it != cm->m_hover_deque->rend(); it++)
		{
			(*it)->MousePressEvent(event);

			if(event->accepted()) {
				cm->SetFocusedWidget(*it);
				std::cout << "widget is focused" << (*it)->name() << std::endl;
				break;
			}
		}
	}

	void Interface::DispatchMouseReleaseEvent(MouseEvent* event)
	{
		ContextManager* cm = ContextManager::context_manager;

		for(std::deque<AbstractWidget*>::reverse_iterator it = cm->m_hover_deque->rbegin(); it != cm->m_hover_deque->rend(); it++)
		{
			(*it)->MouseReleaseEvent(event);

			if(event->accepted()) break;
		}
	}

	void Interface::DispatchDrawEvent(AbstractWidget* widget)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(widget->position().x(),
					 widget->position().y(),
					 widget->z());

		widget->Draw();

		glPopMatrix();

		for(std::set<AbstractWidget*>::iterator it = widget->m_children.begin(); it != widget->m_children.end(); it++)
		{
			DispatchDrawEvent(*it);
		}
	}

	void Interface::BuildWidgetListAtCursorPoint (const Point& cursor_point,
	        AbstractWidget* parent)
	{
		if (parent) {
			parent->m_flag.set(AbstractWidget::WidgetFlagContextHoverList);
			for (std::set<AbstractWidget*>::iterator it =
			        parent->m_children.begin(); it != parent->m_children.end();
			        it++) {
				if ((*it)->contain(cursor_point)) {
					ContextManager::context_manager->m_hover_deque->push_back(*it);
					ContextManager::context_manager->m_hover_deque->back()->CursorEnterEvent(true);
					BuildWidgetListAtCursorPoint(cursor_point, *it);
					break;	// if break or continue the loop?
				}
			}
		} else {
			ContextManager::context_manager->m_hover_deque->clear();

			map<int, set<AbstractWidget*>*>::reverse_iterator map_it;
			set<AbstractWidget*>::iterator set_it;
			set<AbstractWidget*>* set_p = 0;

			bool stop = false;

			for (map_it = ContextManager::context_manager->m_layers.rbegin(); map_it != ContextManager::context_manager->m_layers.rend();
			        map_it++) {
				set_p = map_it->second;
				for (set_it = set_p->begin(); set_it != set_p->end();
				        set_it++) {
					if ((*set_it)->contain(cursor_point)) {
						ContextManager::context_manager->m_hover_deque->push_back(*set_it);
						ContextManager::context_manager->m_hover_deque->back()->CursorEnterEvent(true);
						BuildWidgetListAtCursorPoint(cursor_point, *set_it);
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

}

