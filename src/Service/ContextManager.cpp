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

#include <iostream>
#include <stdlib.h>

#include <BlendInt/Types.hpp>

#include <BlendInt/OpenGL/GLTexture2D.hpp>

#include <BlendInt/Gui/AbstractWidget.hpp>

#include <BlendInt/Service/ContextManager.hpp>

using std::cout;
using std::cerr;
using std::endl;

namespace BlendInt {

	ContextLayer::ContextLayer()
	: refresh(true), widgets(0), buffer(0)
	{

	}

	ContextLayer::~ContextLayer()
	{
		if(buffer) {
			std::cout << "Delete texture buffer in context layer" << std::endl;
			delete buffer;
		}

		if(widgets) {
			std::cout << "Delete widget set in context layer" << std::endl;

			/*
			set<AbstractWidget*>::iterator it;

			for(it = widgets->begin(); it != widgets->end(); it++)
			{
				if((*it)->count() == 0) delete *it;
			}
			*/

			widgets->clear();
		}
	}

	ContextManager* ContextManager::context_manager = 0;

	bool ContextManager::refresh_once = false;

	bool ContextManager::force_refresh_all = true;

	ContextManager* ContextManager::Instance ()
	{
		if (!context_manager) {
			cerr << "The Context Manager is not initialized successfully! Exit"
			        << endl;
			exit(EXIT_FAILURE);
		}

		return context_manager;
	}

	bool ContextManager::Initialize ()
	{
		bool result = true;

		if (!context_manager) {
			context_manager = new ContextManager;
		}

		if (!context_manager) {
			result = false;
		}

		return result;
	}

	void ContextManager::Release ()
	{
		if(context_manager) {
			delete context_manager;
			context_manager = 0;
		}
	}

	ContextManager::ContextManager ()
	: m_main_buffer(0)
	{
		m_events.reset(new Cpp::ConnectionScope);
		m_hover_deque.reset(new std::deque<AbstractWidget*>);

		m_main_buffer = new GLTexture2D;
	}

	ContextManager::~ContextManager ()
	{
		// set focus widget to 0

		if(AbstractWidget::focused_widget) {
			AbstractWidget::focused_widget->m_flag.reset(AbstractWidget::WidgetFlagFocus);
			AbstractWidget::focused_widget = 0;
		}

		map<int, ContextLayer>::iterator layer_iter;
		set<AbstractWidget*>::iterator widget_iter;
		set<AbstractWidget*>* widget_set_p = 0;

		for(layer_iter = m_layers.begin(); layer_iter != m_layers.end(); layer_iter++)
		{
			widget_set_p = layer_iter->second.widgets;
			for(widget_iter = widget_set_p->begin(); widget_iter != widget_set_p->end(); widget_iter++)
			{
				(*widget_iter)->destroyed().disconnectOne(this, &ContextManager::OnDestroyObject);

				//if((*widget_iter)->count() == 0) delete *widget_iter;
			}

			widget_set_p->clear();
		}

		m_layers.clear();
		m_index.clear();

		if(m_main_buffer) {
			m_main_buffer->Clear();
			delete m_main_buffer;
		}
	}


	bool ContextManager::Register (AbstractWidget* obj)
	{
		if (!obj) return false;

		if(obj->m_flag[AbstractWidget::WidgetFlagRegistered]) return true;

		AddWidget(obj);

		obj->m_flag.set(AbstractWidget::WidgetFlagRegistered);

		m_events->connect(obj->destroyed(), this, &ContextManager::OnDestroyObject);

		return true;
	}

	bool ContextManager::Unregister (AbstractWidget* obj)
	{
		if (!obj) return false;

		if(!obj->m_flag[AbstractWidget::WidgetFlagRegistered]) return true;

		if(!RemoveWidget(obj)) {
			obj->m_flag.reset(AbstractWidget::WidgetFlagRegistered);
			DBG_PRINT_MSG("object: %s is not in stored in layer %d in context manager", obj->name().c_str(), obj->z());
			return false;
		}

		obj->m_flag.reset(AbstractWidget::WidgetFlagRegistered);

		obj->destroyed().disconnectOne(this, &ContextManager::OnDestroyObject);

		return true;
	}

	void ContextManager::AddWidget (AbstractWidget* obj)
	{
//		map<AbstractWidget*, int>::iterator map_it;
//
//		map_it = m_index.find(obj);
//
//		if(map_it != m_index.end()) {
//			if (map_it->second == obj->z()) {
//				return false;
//			}
//
//			set<AbstractWidget*>* p = m_layers[map_it->second];
//			set<AbstractWidget*>::iterator it = p->find(obj);
//			if (it != p->end()) {
//				p->erase (it);
//			} else {
//#ifdef DEBUG
//				std::cerr << "Note: object is not recorded" << std::endl;
//#endif
//			}
//
//			if (p->empty()) {
//				m_layers.erase(map_it->second);
//				delete p;
//			}
//
//			map<int, set<AbstractWidget*>* >::iterator layer_it;
//			layer_it = m_layers.find(obj->z());
//			if(layer_it != m_layers.end()) {
//				layer_it->second->insert(obj);
//			} else {
//				set<AbstractWidget*>* new_set = new set<AbstractWidget*>;
//				new_set->insert(obj);
//				m_layers[obj->z()] = new_set;
//			}
//
//		} else {

			map<int, ContextLayer>::iterator layer_iter;
			layer_iter = m_layers.find(obj->z());
			if(layer_iter != m_layers.end()) {
				layer_iter->second.widgets->insert(obj);
			} else {
				set<AbstractWidget*>* new_widget_set_p = new set<AbstractWidget*>;
				new_widget_set_p->insert(obj);
				m_layers[obj->z()].widgets = new_widget_set_p;

				// Refresh this layer in the render loop
				m_layers[obj->z()].refresh = true;
				refresh_once = true;

			}
			
//		}

		m_index[obj] = obj->z();
		//return true;
	}

	bool ContextManager::RemoveWidget (AbstractWidget* obj)
	{
		if (!obj) return false;

		if(AbstractWidget::focused_widget == obj) {
			obj->m_flag.reset(AbstractWidget::WidgetFlagFocus);
			AbstractWidget::focused_widget = 0;
		}

		map<AbstractWidget*, int>::iterator index_iter;
		
		index_iter = m_index.find(obj);

		if(index_iter != m_index.end()) {

			int z = index_iter->second;

			set<AbstractWidget*>* widget_set_p = m_layers[z].widgets;
			set<AbstractWidget*>::iterator widget_iter = widget_set_p->find(obj);
			if (widget_iter != widget_set_p->end()) {
				widget_set_p->erase (widget_iter);
			} else {
				DBG_PRINT_MSG("Error: object %s is not recorded in set", obj->name().c_str());
			}

			if (widget_set_p->empty()) {
				//delete widget_set_p; widget_set_p = 0;
				delete m_layers[z].widgets; m_layers[z].widgets = 0;

				m_layers[z].buffer->Clear();
				delete m_layers[z].buffer; m_layers[z].buffer = 0;

				m_layers.erase(z);
			}

			m_index.erase(obj);

		} else {
			DBG_PRINT_MSG("Error: object %s is not recorded in map", obj->name().c_str());
			return false;
		}

		return true;
	}

	void ContextManager::OnDestroyObject(AbstractWidget* obj)
	{
		std::cout << "Get event" << std::endl;
		if(!obj) return;

		RemoveWidget(obj);
		obj->m_flag.reset(AbstractWidget::WidgetFlagRegistered);
		obj->destroyed().disconnectOne(this, &ContextManager::OnDestroyObject);

		// TODO: remove this widget and its children if it's in m_cursor_widget_stack
	}

#ifdef DEBUG

	void ContextManager::BuildWidgetListAtCursorPoint (const Point& cursor_point,
	        const AbstractWidget* parent)
	{
		if (parent) {
			for (std::set<AbstractWidget*>::iterator it =
			        parent->m_branches.begin(); it != parent->m_branches.end();
			        it++) {
				if ((*it)->contain(cursor_point)) {
					m_hover_deque->push_back(*it);
					BuildWidgetListAtCursorPoint(cursor_point, *it);
					break;	// if break or continue the loop?
				}
			}
		} else {
			m_hover_deque->clear();

			map<int, ContextLayer>::reverse_iterator layer_riter;
			set<AbstractWidget*>::iterator widget_iter;
			set<AbstractWidget*>* widget_set_p = 0;

			bool stop = false;

			for (layer_riter = m_layers.rbegin(); layer_riter != m_layers.rend();
			        layer_riter++) {
				widget_set_p = layer_riter->second.widgets;
				for (widget_iter = widget_set_p->begin(); widget_iter != widget_set_p->end();
				        widget_iter++) {
					if ((*widget_iter)->contain(cursor_point)) {
						m_hover_deque->push_back(*widget_iter);
						BuildWidgetListAtCursorPoint(cursor_point, *widget_iter);
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

	void ContextManager::RemoveWidgetFromHoverDeque(AbstractWidget* widget)
	{
		while(m_hover_deque->size()) {
			m_hover_deque->back()->m_flag.reset(AbstractWidget::WidgetFlagContextHoverList);

			if(m_hover_deque->back() == widget) {
				m_hover_deque->pop_back();
				break;
			}

			m_hover_deque->pop_back();
		}
	}

	void ContextManager::SetFocusedWidget (AbstractWidget* widget)
	{
		if(AbstractWidget::focused_widget) {
			AbstractWidget::focused_widget->m_flag.reset(AbstractWidget::WidgetFlagFocus);
		}

		AbstractWidget::focused_widget = widget;
		if(AbstractWidget::focused_widget) {
			AbstractWidget::focused_widget->m_flag.set(AbstractWidget::WidgetFlagFocus);
		}
	}

	void ContextManager::RefreshLayer (int layer)
	{
		map<int, ContextLayer>::iterator layer_iter;

		layer_iter = m_layers.find(layer);

		if(layer_iter != m_layers.end()) {
			m_layers[layer].refresh = true;
			refresh_once = true;
		}
	}

	void ContextManager::print ()
	{
		map<int, ContextLayer>::iterator layer_iter;
		set<AbstractWidget*>::iterator widget_iter;

		set<AbstractWidget*>* widget_set_p;
		std::cout << std::endl;

		std::cerr << "size of index map:" << m_index.size() << std::endl;
		std::cerr << "size of layer map:" << m_layers.size() << std::endl;

		for(layer_iter = m_layers.begin(); layer_iter != m_layers.end(); layer_iter++)
		{
			std::cerr << "Layer: " << layer_iter->first << std::endl;
			widget_set_p = layer_iter->second.widgets;
			for(widget_iter = widget_set_p->begin(); widget_iter != widget_set_p->end(); widget_iter++)
			{
				std::cerr << (*widget_iter)->name() << " ";
			}
			std::cerr << std::endl;
		}
	}

#endif

}

