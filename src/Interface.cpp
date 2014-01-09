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

#include <BlendInt/Interface.hpp>

#include <BlendInt/FontConfig.hpp>
#include <BlendInt/FontCache.hpp>
#include <BlendInt/Theme.hpp>
#include <BlendInt/ShaderManager.hpp>
#include <BlendInt/Size.hpp>
#include <BlendInt/AbstractWidget.hpp>
#include <BlendInt/KeyEvent.hpp>
#include <BlendInt/MouseEvent.hpp>
#include <BlendInt/ContextMenuEvent.hpp>
#include <BlendInt/ContextManager.hpp>
#include <BlendInt/StockIcon.hpp>
#include <BlendInt/GLTexture2D.hpp>

#include <BlendInt/GLFramebuffer.hpp>

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

		bool result = true;
		if (!FontConfig::initialize()) {
			std::cerr << "Cannot initialize FontConfig" << std::endl;
			result = false;
		} else {
			FontConfig* ftconfig = FontConfig::instance();
			if (!ftconfig->loadDefaultFontToMem()) {
				cerr << "Cannot load default font into memory" << endl;
				result = false;
			}
		}

		if (!ThemeManager::initialize()) {
			std::cerr << "Cannot initialize Themes" << std::endl;
			result = false;
		}

		if (!ShaderManager::Initialize()) {
			std::cerr << "Cannot initialize Shader Manager" << std::endl;
			result = false;
		}

		if (!StockIcon::Initialize()) {
			std::cerr << "Cannot initialize Stock Icons" << std::endl;
			result = false;
		}

		if (!ContextManager::Initialize()) {
			std::cerr << "Cannot initialize Context Manager" << std::endl;
			result = false;
		}



		if (!interface) {
			interface = new Interface();
		}

		if (!interface)
			result = false;

		return result;
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
	: m_focus_style(FocusOnClick)
	{
		m_events.reset(new Cpp::ConnectionScope);
	}

	Interface::~Interface ()
	{

	}

	const Size& Interface::size () const
	{
		return m_size;
	}

	void Interface::Resize (const Size& size)
	{
		m_size = size;
		m_resized.fire(m_size.width(), m_size.height());
	}

	void Interface::Draw ()
	{
		int width = m_size.width();
		int height = m_size.height();
		// float ratio = width / (float) height;

		// float bg = 114.0 / 255;	// the default blender background color
		glClearColor(0.447, 0.447, 0.447, 1.00);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glColor4f(1.00, 1.00, 1.00, 1.00);

		// enable anti-alias
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		//glEnable(GL_DEPTH_TEST);
		//glEnable (GL_POINT_SMOOTH);
		//glEnable (GL_LINE_SMOOTH);
		//glEnable (GL_POLYGON_SMOOTH);

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.f, (float) width, 0.f, (float) height, 100.f, -100.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

#ifdef DEBUG
		draw_grid(width, height);
#endif

		map<int, set<AbstractWidget*>* >::iterator map_it;
		set<AbstractWidget*>::iterator set_it;
		ContextManager* cm = ContextManager::Instance();

		for(map_it = cm->m_layers.begin(); map_it != cm->m_layers.end(); map_it++)
		{
			set<AbstractWidget*>* pset = map_it->second;
			for (set_it = pset->begin(); set_it != pset->end(); set_it++)
			{
				if((*set_it)->visible()) {
					(*set_it)->Draw();
				}
			}
		}
		// m_ticktack = m_ticktack ? 0 : 1;

		//glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
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
#endif

	void Interface::Resize (unsigned int width, unsigned int height)
	{
		m_size.set_width(width);
		m_size.set_height(height);

		m_resized.fire(m_size.width(), m_size.height());
	}

	void Interface::GLFWKeyEvent (int key, int scancode, int action, int mods)
	{
#ifdef DEBUG
		if(key == GLFW_KEY_F6 && action == GLFW_PRESS)
			RenderToImage();
#endif

		if (key == Key_Menu) {
			ContextMenuEvent event(ContextMenuEvent::Keyboard, mods);

			map<int, set<AbstractWidget*>* >::reverse_iterator map_it;
			set<AbstractWidget*>::reverse_iterator set_it;
			ContextManager* cm = ContextManager::Instance();
			set<AbstractWidget*>* pset = 0;

			for(map_it = cm->m_layers.rbegin(); map_it != cm->m_layers.rend(); map_it++)
			{
				pset = map_it->second;
				for (set_it = pset->rbegin(); set_it != pset->rend(); set_it++)
				{
					// TODO: only the focused widget can dispose key event
					switch (action) {
					case KeyPress:
						(*set_it)->ContextMenuPressEvent(&event);
						break;
					case KeyRelease:
						(*set_it)->ContextMenuReleaseEvent(&event);
						break;
					default:
						break;
					}
					if (event.ignored()) break;
					if (event.accepted()) {
						// TODO: do sth needed
						break;
					}
				}
				if(event.ignored()) break;
				if(event.accepted()) {
					// TODO: do sth needed
					break;
				}
			}

		} else {

			KeyAction key_action = KeyNone;

			switch(action) {
				case GLFW_PRESS:
					key_action = KeyPress;
					break;
				case GLFW_RELEASE:
					key_action = KeyRelease;
					break;
				case GLFW_REPEAT:
					key_action = KeyRepeat;
					break;
				default:
					break;
			}

			KeyEvent event(key_action, key, scancode, mods);

			map<int, set<AbstractWidget*>* >::reverse_iterator map_it;
			set<AbstractWidget*>::reverse_iterator set_it;
			ContextManager* cm = ContextManager::Instance();
			set<AbstractWidget*>* pset = 0;
			for(map_it = cm->m_layers.rbegin(); map_it != cm->m_layers.rend(); map_it++)
			{
				pset = map_it->second;
				for (set_it = pset->rbegin(); set_it != pset->rend(); set_it++)
				{
					//if(!(*set_it)->visible()) break;
					// TODO: only the focused widget can dispose key event
					switch (action) {
					case KeyPress:
						dispatch_key_press_event((*set_it), &event);
						break;
					case KeyRelease:
						// item->KeyReleaseEvent(dynamic_cast<BlendInt::KeyEvent*>(event));
						break;
					case KeyRepeat:
						// item->KeyRepeatEvent(&event);
						break;
					default:
						break;
					}
					if(event.ignored()) break;
					if(event.accepted()) {
						// TODO: do sth needed
						break;
					}

				}
				if(event.ignored())	break;
				if (event.accepted()) {
					// TODO: do sth needed
					break;
				}
			}
		}

	}

	void Interface::GLFWMouseButtonEvent (int button, int action, int mods)
	{
		MouseButton mouseclick = MouseButtonNone;
		switch (button) {
		case GLFW_MOUSE_BUTTON_1:
			mouseclick = MouseButtonLeft;
			break;
		case GLFW_MOUSE_BUTTON_2:
			mouseclick = MouseButtonRight;
			break;
		case GLFW_MOUSE_BUTTON_3:
			mouseclick = MouseButtonMiddle;
			break;
		case GLFW_MOUSE_BUTTON_4:
			mouseclick = MouseButtonScrollUp;
			break;
		case GLFW_MOUSE_BUTTON_5:
			mouseclick = MouseButtonScrollDown;
			break;
		default:
			break;
		}

		MouseAction mouse_action = MouseNone;
		switch (action) {
		case GLFW_PRESS:
			mouse_action = MousePress;
			break;
		case GLFW_RELEASE:
			mouse_action = MouseRelease;
			break;
		default:
			break;
		}

		MouseEvent event(mouse_action, mouseclick, m_cursor);

		map<int, set<AbstractWidget*>* >::reverse_iterator map_it;
		set<AbstractWidget*>::reverse_iterator set_it;
		ContextManager* cm = ContextManager::Instance();

		//event.set_position(m_cursor);

		for(map_it = cm->m_layers.rbegin(); map_it != cm->m_layers.rend(); map_it++)
		{
			set<AbstractWidget*>* pset = map_it->second;
			for (set_it = pset->rbegin(); set_it != pset->rend(); set_it++)
			{
				//if(!(*set_it)->visible()) break;

				switch (action) {
				case GLFW_PRESS:
					dispatch_mouse_press_event((*set_it), &event);
					break;
				case GLFW_RELEASE:
					(*set_it)->MouseReleaseEvent(&event);
					break;
				default:
					break;
				}
				if(event.ignored())	break;

				if(event.accepted()) {
					// TODO: do sth needed
					break;
				}
			}
			if (event.ignored()) break;
			if (event.accepted()) {
				// TODO: do sth needed
				break;
			}

		}
	}

	void Interface::GLFWCursorPosEvent (double xpos, double ypos)
	{
		m_cursor.set_x(static_cast<int>(xpos));
		m_cursor.set_y(static_cast<int>(ypos));

		MouseEvent event(MouseNone, MouseButtonNone, m_cursor);

		map<int, set<AbstractWidget*>* >::reverse_iterator map_it;
		set<AbstractWidget*>::reverse_iterator set_it;
		ContextManager* cm = ContextManager::Instance();

		//event.SetPosition(cursor_pos_x_, cursor_pos_y_);

		for(map_it = cm->m_layers.rbegin(); map_it != cm->m_layers.rend(); map_it++)
		{
			set<AbstractWidget*>* pset = map_it->second;
			for (set_it = pset->rbegin(); set_it != pset->rend(); set_it++)
			{
				//if(!(*set_it)->visible()) break;

				dispatch_mouse_move_event((*set_it), &event);

				if(event.ignored()) break;

				if (event.accepted()) {
					// TODO: do sth needed
					break;
				}
			}
			if(event.ignored()) break;
			if(event.accepted()) {
				// TODO: do sth needed
				break;
			}
		}
	}

#ifdef DEBUG
	void Interface::DispatchRender(AbstractForm* form)
	{
		form->Draw();
	}
#endif

	void Interface::DispatchKeyEvent (KeyEvent* event)
	{
		if(!event) return;

		ContextManager* cm = ContextManager::Instance();

		if(cm->m_focus) {
			switch (event->action()) {
				case KeyPress:
					cm->m_focus->KeyPressEvent(event);
					break;
				case KeyRelease:
					// item->KeyReleaseEvent(dynamic_cast<BlendInt::KeyEvent*>(event));
					break;
				case KeyRepeat:
					// item->KeyRepeatEvent(&event);
					break;
				default:
					break;
			}
		}

		/*
		map<int, set<AbstractWidget*>* >::reverse_iterator map_it;
		set<AbstractWidget*>::reverse_iterator set_it;
		set<AbstractWidget*>* pset = 0;

		for(map_it = cm->m_layers.rbegin(); map_it != cm->m_layers.rend(); map_it++)
		{
			pset = map_it->second;
			for (set_it = pset->rbegin(); set_it != pset->rend(); set_it++)
			{
				//if(!(*set_it)->visible()) break;
				// TODO: only the focused widget can dispose key event
				switch (event->action()) {
				case KeyPress:
					(*set_it)->KeyPressEvent(event);
					break;
				case KeyRelease:
					// item->KeyReleaseEvent(dynamic_cast<BlendInt::KeyEvent*>(event));
					break;
				case KeyRepeat:
					// item->KeyRepeatEvent(&event);
					break;
				default:
					break;
				}
				if(event->ignored()) break;
				if(event->accepted()) {
					// TODO: do sth needed
					//break;
				}

			}
			if(event->ignored())	break;
			if (event->accepted()) {
				// TODO: do sth needed
				//break;
			}
		}
		*/

	}

	void Interface::DispatchMouseEvent (MouseEvent* event)
	{
		if (!event)
			return;

		map<int, set<AbstractWidget*>*>::reverse_iterator map_it;
		set<AbstractWidget*>::reverse_iterator set_it;
		ContextManager* cm = ContextManager::Instance();

		AbstractWidget* widget = 0;

		for (map_it = cm->m_layers.rbegin(); map_it != cm->m_layers.rend();
		        map_it++) {

			set<AbstractWidget*>* pset = map_it->second;
			for (set_it = pset->rbegin(); set_it != pset->rend(); set_it++) {
				widget = *set_it;

				if (!widget->visible())
					continue;

				if (!widget->contain(event->position()))
					continue;

				switch (event->action()) {
					case MousePress: {
						if (m_focus_style == FocusOnClick) {
							// TODO: send unfocus event
							if(cm->m_focus != widget) {
								if(cm->m_focus) {
									cm->m_focus->m_flag.reset(
									        AbstractWidget::WidgetFocus);
								}
								cm->m_focus = widget;
								cm->m_focus->m_flag.set(
								        AbstractWidget::WidgetFocus);
							}
						}

						widget->MousePressEvent(event);

						break;
					}
					case MouseRelease:
						widget->MouseReleaseEvent(event);
						break;

					default: {
						if (m_focus_style == FocusOnHover) {
							// TODO: send unfocus event first
							if (cm->m_focus) {
								cm->m_focus->m_flag.reset(
								        AbstractWidget::WidgetFocus);
							}
							cm->m_focus = widget;
							cm->m_focus->m_flag.set(
							        AbstractWidget::WidgetFocus);
						}
						(*set_it)->MouseMoveEvent(event);
						break;
					}
				}

				if (event->ignored())
					break;

				if (event->accepted()) {
					// TODO: do sth needed
					break;
				}
			}
			if (event->ignored())
				break;
			if (event->accepted()) {
				// TODO: do sth needed
				break;
			}

		}
	}

	void Interface::RenderToImage()
	{
		std::cout << "Render to Image" << std::endl;

		// Create and set texture to render to.
		GLTexture2D* tex = new GLTexture2D;
		tex->Generate();
		tex->Bind();
		tex->SetWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		tex->SetMinFilter(GL_NEAREST);
		tex->SetMagFilter(GL_NEAREST);
		tex->SetImage(m_size.width(), m_size.height(), 0);

		// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
		GLFramebuffer* fb = new GLFramebuffer;
		fb->Generate();
		fb->Bind();

		// Set "renderedTexture" as our colour attachement #0
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		        GL_TEXTURE_2D, tex->id(), 0);

		/*
		GLuint rb = 0;
		glGenRenderbuffers(1, &rb);

		glBindRenderbuffer(GL_RENDERBUFFER_EXT, rb);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
		        m_size.width(), m_size.height());

		//-------------------------

		//Attach depth buffer to FBO
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
		*/

		//-------------------------
		//and now render to GL_TEXTURE_2D
		fb->Bind();

		Draw();

		//Bind 0, which means render to back buffer
		fb->Unbind();

		tex->WriteToFile("output.png");
		tex->Unbind();

		//Delete resources
		delete tex;
		tex = 0;

		//glDeleteRenderbuffers(1, &rb);

		//Bind 0, which means render to back buffer, as a result, fb is unbound
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		delete fb; fb = 0;

		Draw();
	}

	void Interface::dispatch_key_press_event (AbstractWidget* obj, KeyEvent* event)
	{
		obj->KeyPressEvent(event);
	}

	void Interface::dispatch_mouse_press_event (AbstractWidget* obj, MouseEvent* event)
	{
		obj->MousePressEvent(event);
	}

	void Interface::dispatch_mouse_release_event (AbstractWidget* obj, MouseEvent* event)
	{
		obj->MouseReleaseEvent(event);
	}

	void Interface::dispatch_mouse_move_event (AbstractWidget* obj, MouseEvent* event)
	{
		obj->MouseMoveEvent(event);
	}

}
