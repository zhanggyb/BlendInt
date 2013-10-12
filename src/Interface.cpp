/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>
#include <iostream>

#include <BlendInt/Interface.hpp>

#include <BlendInt/FontConfig.hpp>
#include <BlendInt/FontCache.hpp>
#include <BlendInt/Theme.hpp>
#include <BlendInt/ShaderManager.hpp>
#include <BlendInt/Size.hpp>
#include <BlendInt/Drawable.hpp>
#include <BlendInt/KeyEvent.hpp>
#include <BlendInt/MouseEvent.hpp>
#include <BlendInt/ContextMenuEvent.hpp>
#include <BlendInt/ContextManager.hpp>
#include <BlendInt/Drawable.hpp>

namespace BlendInt {

	Interface* Interface::interface = 0;

	Interface* Interface::instance ()
	{
		if (!interface) {
			std::cerr
			        << "The Interface Library is not initialized successfully! Exit"
			        << std::endl;
			exit(EXIT_FAILURE);
		}

		return interface;
	}

	bool Interface::initialize ()
	{
		glewExperimental = true;	// Needed in core profile
		if (glewInit() != GLEW_OK) {
			std::cerr << "Fail to initialize GLEW" << endl;
			return false;
		}

		if (!GLEW_VERSION_2_0) {
			std::cerr << "No support for OpenGL 2.0 found" << std::endl;
			return false;
		}

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

		if (!ShaderManager::initialize()) {
			std::cerr << "Cannot initialize Shader Manager" << std::endl;
			result = false;
		}

		if (!ContextManager::initialize()) {
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

	void Interface::release ()
	{
		ContextManager::release();
		ShaderManager::release();
		ThemeManager::release();
		FontCache::releaseAll();
		FontConfig::release();

		if (interface) {
			delete interface;
			interface = 0;
		}
	}

	Interface::Interface ()
			: cursor_pos_x_(0.0), cursor_pos_y_(0.0), m_ticktack(0)
	{

	}

	Interface::~Interface ()
	{

	}

	bool Interface::bind (Drawable* object)
	{
		return ContextManager::instance()->bind(object);
	}

	bool Interface::unbind (Drawable* object)
	{
		return ContextManager::instance()->unbind(object);
	}

	const Size& Interface::size () const
	{
		return size_;
	}

	void Interface::resize (int width, int height)
	{
		size_.set_width(width);
		size_.set_height(height);
	}

	void Interface::resize (const Size& size)
	{
		size_ = size;
	}

	void Interface::render ()
	{
		int width = size_.width();
		int height = size_.height();
		// float ratio = width / (float) height;

		// float bg = 114.0 / 255;	// the default blender background color
		glClearColor(0.447, 0.447, 0.447, 1.00);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glColor4f(1.00, 1.00, 1.00, 1.00);

		// enable anti-alias
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
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

		map<int, set<Drawable*>* >::iterator map_it;
		set<Drawable*>::iterator set_it;
		ContextManager* cm = ContextManager::instance();

		for(map_it = cm->m_layers.begin(); map_it != cm->m_layers.end(); map_it++)
		{
			set<Drawable*>* pset = map_it->second;
			for (set_it = pset->begin(); set_it != pset->end(); set_it++)
			{
				if((*set_it)->m_ticktack == m_ticktack && (*set_it)->visible()) {
					(*set_it)->render();
					(*set_it)->m_ticktack = (*set_it)->m_ticktack ? 0 : 1;
				}
			}
		}
		m_ticktack = m_ticktack ? 0 : 1;

		glDisable(GL_BLEND);
	}

	void Interface::dispatch_render_event (Drawable* obj)
	{
		// ticktack makes sure only render once, the ticktack of Interface reversed in Interface::render()
		if(obj->m_ticktack == m_ticktack && obj->visible()) {
			obj->render();
			obj->m_ticktack = obj->m_ticktack ? 0 : 1;
		}
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

	void Interface::resizeEvent (int width, int height)
	{
		// TODO: resize all widgets/layouts in children
		size_.set_width(width);
		size_.set_height(height);
	}

	void Interface::keyEvent (int key, int scancode, int action, int mods)
	{
		if (key == Key_Menu) {
			ContextMenuEvent event(ContextMenuEvent::Keyboard, mods);

			map<int, set<Drawable*>* >::reverse_iterator map_it;
			set<Drawable*>::reverse_iterator set_it;
			ContextManager* cm = ContextManager::instance();
			set<Drawable*>* pset = 0;

			for(map_it = cm->m_layers.rbegin(); map_it != cm->m_layers.rend(); map_it++)
			{
				pset = map_it->second;
				for (set_it = pset->rbegin(); set_it != pset->rend(); set_it++)
				{
					if (!(*set_it)->visible()) break;

					// TODO: only the focused widget can dispose key event
					switch (action) {
						case KeyPress:
							(*set_it)->press_context_menu(&event);
							break;
						case KeyRelease:
							(*set_it)->release_context_menu(&event);
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

			KeyEvent event(key, scancode, action, mods);

			map<int, set<Drawable*>* >::reverse_iterator map_it;
			set<Drawable*>::reverse_iterator set_it;
			ContextManager* cm = ContextManager::instance();
			set<Drawable*>* pset = 0;
			for(map_it = cm->m_layers.rbegin(); map_it != cm->m_layers.rend(); map_it++)
			{
				pset = map_it->second;
				for (set_it = pset->rbegin(); set_it != pset->rend(); set_it++)
				{
					if(!(*set_it)->visible()) break;
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

	void Interface::mouseButtonEvent (int button, int action, int mods)
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

		MouseEvent event(mouse_action, mouseclick);

		map<int, set<Drawable*>* >::reverse_iterator map_it;
		set<Drawable*>::reverse_iterator set_it;
		ContextManager* cm = ContextManager::instance();

		for(map_it = cm->m_layers.rbegin(); map_it != cm->m_layers.rend(); map_it++)
		{
			set<Drawable*>* pset = map_it->second;
			for (set_it = pset->rbegin(); set_it != pset->rend(); set_it++)
			{
				if(!(*set_it)->visible()) break;

				event.set_position(cursor_pos_x_, cursor_pos_y_);
				switch (action) {
					case GLFW_PRESS:
						dispatch_mouse_press_event((*set_it), &event);
						break;
					case GLFW_RELEASE:
						(*set_it)->release_mouse(&event);
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

	void Interface::cursorPosEvent (double xpos, double ypos)
	{
		cursor_pos_x_ = xpos;
		cursor_pos_y_ = size_.height() - ypos;

		MouseEvent event(MouseNone, MouseButtonNone);

		map<int, set<Drawable*>* >::reverse_iterator map_it;
		set<Drawable*>::reverse_iterator set_it;
		ContextManager* cm = ContextManager::instance();

		for(map_it = cm->m_layers.rbegin(); map_it != cm->m_layers.rend(); map_it++)
		{
			set<Drawable*>* pset = map_it->second;
			for (set_it = pset->rbegin(); set_it != pset->rend(); set_it++)
			{
				if(!(*set_it)->visible()) break;

				event.set_position(cursor_pos_x_, cursor_pos_y_);
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

	void Interface::dispatch_key_press_event (Drawable* obj, KeyEvent* event)
	{
		obj->press_key(event);
	}

	void Interface::dispatch_mouse_press_event (Drawable* obj, MouseEvent* event)
	{
		obj->press_mouse(event);
	}

	void Interface::dispatch_mouse_release_event (Drawable* obj, MouseEvent* event)
	{
		obj->release_mouse(event);
	}

	void Interface::dispatch_mouse_move_event (Drawable* obj, MouseEvent* event)
	{
		obj->move_mouse(event);
	}

}
