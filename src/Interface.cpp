/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>
#include <iostream>

#include <BIL/Interface.hpp>

#include <BIL/FontConfig.hpp>
#include <BIL/FontCache.hpp>
#include <BIL/Theme.hpp>
#include <BIL/ShaderManager.hpp>
#include <BIL/Size.hpp>
#include <BIL/Traceable.hpp>
#include <BIL/Drawable.hpp>
#include <BIL/KeyEvent.hpp>
#include <BIL/MouseEvent.hpp>
#include <BIL/ContextMenuEvent.hpp>
#include <BIL/ContextManager.hpp>

namespace BIL {

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

		if (!Theme::initialize()) {
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
		Theme::release();
		FontCache::releaseAll();
		FontConfig::release();

		if (interface) {
			delete interface;
			interface = 0;
		}
	}

	Interface::Interface ()
			: cursor_pos_x_(0.0), cursor_pos_y_(0.0)
	{

	}

	Interface::~Interface ()
	{

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

		map<int, list<Drawable*>* >::iterator map_it;
		list<Drawable*>::iterator list_it;
		ContextManager* cm = ContextManager::instance();
		int layer = 0;
		for(map_it = cm->m_layers.begin(); map_it != cm->m_layers.end(); map_it++)
		{
			layer = map_it->first;
			list<Drawable*>* plist = map_it->second;
			for (list_it = plist->begin(); list_it != plist->end(); list_it++)
			{
				//(*list_it)->render();
				render_at_layer (*list_it, layer);
			}
		}

		glDisable(GL_BLEND);
	}

	void Interface::render_at_layer (Drawable* obj, int layer)
	{
		list<Traceable*>::const_iterator it;
		Drawable *item = NULL;
		for (it = obj->children().begin(); it != obj->children().end(); it++) {
			item = dynamic_cast<Drawable*>(*it);
			if (item) {

				// only drawable object at the layer will be called for render
				// object in differenct layer will be called in another loop
				if (item->z() == layer) {
					render_at_layer(item, layer);
				}

			}
		}

		obj->render();
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

			map<int, list<Drawable*>* >::reverse_iterator map_it;
			list<Drawable*>::reverse_iterator list_it;
			ContextManager* cm = ContextManager::instance();

			for(map_it = cm->m_layers.rbegin(); map_it != cm->m_layers.rend(); map_it++)
			{
				list<Drawable*>* plist = map_it->second;
				for (list_it = plist->rbegin(); list_it != plist->rend(); list_it++)
				{
					// TODO: only the focused widget can dispose key event
					switch (action) {
						case KeyPress:
							(*list_it)->contextMenuPressEvent(&event);
							break;
						case KeyRelease:
							(*list_it)->contextMenuReleaseEvent(&event);
							break;
						default:
							break;
					}
					if (event.accepted())
						break;
				}
				if(event.accepted()) {
					break;
				}
			}

		} else {

			KeyEvent event(key, scancode, action, mods);

			map<int, list<Drawable*>* >::reverse_iterator map_it;
			list<Drawable*>::reverse_iterator list_it;
			ContextManager* cm = ContextManager::instance();
			int layer = 0;

			for(map_it = cm->m_layers.rbegin(); map_it != cm->m_layers.rend(); map_it++)
			{
				layer = map_it->first;
				list<Drawable*>* plist = map_it->second;
				for (list_it = plist->rbegin(); list_it != plist->rend(); list_it++)
				{
					// TODO: only the focused widget can dispose key event
					switch (action) {
						case KeyPress:
							dispatch_key_press_event_at_layer((*list_it), &event, layer);
							break;
						case KeyRelease:
							// item->KeyReleaseEvent(dynamic_cast<BIL::KeyEvent*>(event));
							break;
						case KeyRepeat:
							// item->KeyRepeatEvent(&event);
							break;
						default:
							break;
					}
					if(event.accepted()) {
						break;
					}
				}
				if (event.accepted())
					break;
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
		event.set_window_pos(cursor_pos_x_, cursor_pos_y_);

		float local_x;
		float local_y;
		map<int, list<Drawable*>* >::reverse_iterator map_it;
		list<Drawable*>::reverse_iterator list_it;
		ContextManager* cm = ContextManager::instance();
		int layer = 0;

		for(map_it = cm->m_layers.rbegin(); map_it != cm->m_layers.rend(); map_it++)
		{
			layer = map_it->first;
			list<Drawable*>* plist = map_it->second;
			for (list_it = plist->rbegin(); list_it != plist->rend(); list_it++)
			{
				local_x = cursor_pos_x_ - ((*list_it)->pos().x());
				local_y = cursor_pos_y_ - ((*list_it)->pos().y());
				if ((local_x - 0.000001 > 0.0) && (local_y - 0.000001 > 0.0)
						&& (local_x - (*list_it)->size().width()) < 0.0
						&& (local_y - (*list_it)->size().height()) < 0.0) {
					event.set_pos(local_x, local_y);
					switch (action) {
						case GLFW_PRESS:
							dispatch_mouse_press_event_at_layer((*list_it), &event, layer);
							break;
						case GLFW_RELEASE:
							(*list_it)->mouseReleaseEvent(&event);
							break;
						default:
							break;
					}
				}
				if(event.accepted())
					break;
			}
			if (event.accepted())
				break;
		}
	}

	void Interface::cursorPosEvent (double xpos, double ypos)
	{
		cursor_pos_x_ = xpos;
		cursor_pos_y_ = size_.height() - ypos;

		MouseEvent event(MouseNone, MouseButtonNone);
		event.set_window_pos(cursor_pos_x_, cursor_pos_y_);

		float local_x;
		float local_y;

		map<int, list<Drawable*>* >::reverse_iterator map_it;
		list<Drawable*>::reverse_iterator list_it;
		ContextManager* cm = ContextManager::instance();
		int layer = 0;

		for(map_it = cm->m_layers.rbegin(); map_it != cm->m_layers.rend(); map_it++)
		{
			layer = map_it->first;
			list<Drawable*>* plist = map_it->second;
			for (list_it = plist->rbegin(); list_it != plist->rend(); list_it++)
			{
				local_x = cursor_pos_x_ - ((*list_it)->pos().x());
				local_y = cursor_pos_y_ - ((*list_it)->pos().y());
				event.set_pos(local_x, local_y);
				dispatch_mouse_move_event_at_layer((*list_it), &event, layer);
				if (event.accepted())
					break;
			}
			if(event.accepted()) break;
		}
	}

	void Interface::dispatch_key_press_event_at_layer (Drawable* obj, KeyEvent* event, int layer)
	{
		// only object int the same layer will do this function
		// objects in different layer will be called in other loop
		if (obj->z() == layer) {

			obj->keyPressEvent(event);

			if (event->accepted()) {
				return;
			} else {
				list<Traceable*>::const_reverse_iterator it;
				Drawable *item = NULL;
				for (it = obj->children().rbegin(); it != obj->children().rend();
			        it++)
				{
					item = dynamic_cast<Drawable*>(*it);
					if (item)
						dispatch_key_press_event_at_layer(item, event, layer);
					if (event->accepted())
						return;
				}
			}
		}
	}

	void Interface::dispatch_mouse_press_event_at_layer (Drawable* obj, MouseEvent* event, int layer)
	{
		if (obj->z() == layer) {

			obj->mousePressEvent(event);

			if (event->accepted()) {
				return;
			} else {
				list<Traceable*>::const_reverse_iterator it;
				Drawable *item = NULL;
				for (it = obj->children().rbegin(); it != obj->children().rend();
			        it++)
				{
					item = dynamic_cast<Drawable*>(*it);
					if (item) {
						event->set_pos(cursor_pos_x_ - (item->pos().x()),
					        cursor_pos_y_ - (item->pos().y()));
						dispatch_mouse_press_event_at_layer(item, event, layer);
					}
					if (event->accepted())
						return;
				}
			}
		}
	}

	void Interface::dispatch_mouse_release_event_at_layer (Drawable* obj, MouseEvent* event, int layer)
	{
		if (obj->z() == layer) {

			obj->mouseReleaseEvent(event);

			if (event->accepted()) {
				return;
			} else {
				list<Traceable*>::const_reverse_iterator it;
				Drawable *item = NULL;
				for (it = obj->children().rbegin(); it != obj->children().rend();
			        it++)
				{
					item = dynamic_cast<Drawable*>(*it);
					if (item) {
						event->set_pos(cursor_pos_x_ - (item->pos().x()),
					        cursor_pos_y_ - (item->pos().y()));
						dispatch_mouse_release_event_at_layer(item, event, layer);
					}
					if (event->accepted())
						return;
				}
			}
		}
	}

	void Interface::dispatch_mouse_move_event_at_layer (Drawable* obj, MouseEvent* event, int layer)
	{
		if(obj->z() == layer) {
			obj->mouseMoveEvent(event);

			if (event->accepted()) {
				return;
			} else {
				list<Traceable*>::const_reverse_iterator it;
				Drawable *item = NULL;
				for (it = obj->children().rbegin(); it != obj->children().rend();
						it++)
				{
					item = dynamic_cast<Drawable*>(*it);
					if (item) {
						event->set_pos(cursor_pos_x_ - (item->pos().x()),
								cursor_pos_y_ - (item->pos().y()));
						dispatch_mouse_move_event_at_layer(item, event, layer);
					}
					if (event->accepted())
						return;
				}
			}
		}
	}

}
