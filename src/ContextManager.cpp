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

#include <BlendInt/ContextManager.hpp>
#include <BlendInt/AbstractWidget.hpp>

#include <BlendInt/Types.hpp>

using std::cout;
using std::cerr;
using std::endl;

namespace BlendInt {

	ContextManager* ContextManager::context_manager = 0;

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
	: m_focus(0)
	{
		m_events.reset(new Cpp::ConnectionScope);
	}

	ContextManager::~ContextManager ()
	{
		m_focus = 0;

		map<int, set<AbstractWidget*>* >::iterator map_it;
		set<AbstractWidget*>::iterator set_it;
		set<AbstractWidget*>* pset = 0;

		for(map_it = m_layers.begin(); map_it != m_layers.end(); map_it++)
		{
			pset = map_it->second;
			for(set_it = pset->begin(); set_it != pset->end(); set_it++)
			{
				(*set_it)->destroyed().disconnectOne(this, &ContextManager::OnDestroyObject);

				if((*set_it)->ref_count() == 0)
					delete *set_it;
			}

			pset->clear();
		}

		m_layers.clear();
		m_index.clear();
	}


	bool ContextManager::Register (AbstractWidget* obj)
	{
		if (!obj) return false;

		if(obj->m_flag[2]) return true;

		AddWidget(obj);

		obj->m_flag.set(2);

		m_events->connect(obj->destroyed(), this, &ContextManager::OnDestroyObject);

		return true;
	}

	bool ContextManager::Unregister (AbstractWidget* obj)
	{
		if (!obj) return false;

		if(!obj->m_flag[2]) return true;

		if(!RemoveWidget(obj)) {
			obj->m_flag.reset(2);
			std::cerr << "obj not in in contextmanager with the same layer" << std::endl;
			return false;
		}

		obj->m_flag.reset(2);

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

			map<int, set<AbstractWidget*>* >::iterator layer_it;
			layer_it = m_layers.find(obj->z());
			if(layer_it != m_layers.end()) {
				layer_it->second->insert(obj);
			} else {
				set<AbstractWidget*>* new_set = new set<AbstractWidget*>;
				new_set->insert(obj);
				m_layers[obj->z()] = new_set;
			}
			
//		}

		m_index[obj] = obj->z();
		//return true;
	}

	void ContextManager::OnDestroyObject(AbstractWidget* obj)
	{
		std::cout << "Get event" << std::endl;
		if(!obj) return;

		RemoveWidget(obj);
		obj->m_flag.reset(2);
		obj->destroyed().disconnectOne(this, &ContextManager::OnDestroyObject);
	}

	bool ContextManager::RemoveWidget (AbstractWidget* obj)
	{
		if (!obj) return false;

		if(m_focus == obj) {
			m_focus = 0;
		}

		map<AbstractWidget*, int>::iterator map_it;
		
		map_it = m_index.find(obj);

		if(map_it != m_index.end()) {

			set<AbstractWidget*>* p = m_layers[map_it->second];
			set<AbstractWidget*>::iterator it = p->find(obj);
			if (it != p->end()) {
				p->erase (it);
			} else {
#ifdef DEBUG
				std::cerr << "Error: object " << obj->name() << " is not recorded" << std::endl;
#endif
			}

			if (p->empty()) {
				m_layers.erase(map_it->second);
				delete p;
			}

			m_index.erase(obj);

		} else {
#ifdef DEBUG
			std::cerr << "Error: object " << obj->name() << " is not recorded" << std::endl;
#endif
			return false;
		}

		return true;
	}

#ifdef DEBUG

	void ContextManager::print ()
	{
		LayerType::iterator map_it;
		SetType::iterator set_it;

		SetType* pset;
		std::cout << std::endl;

		std::cerr << "size of index map:" << m_index.size() << std::endl;
		std::cerr << "size of layer map:" << m_layers.size() << std::endl;

		for(map_it = m_layers.begin(); map_it != m_layers.end(); map_it++)
		{
			std::cerr << "Layer: " << map_it->first << std::endl;
			pset = map_it->second;
			for(set_it = pset->begin(); set_it != pset->end(); set_it++)
			{
				std::cerr << (*set_it)->name() << " ";
			}
			std::cerr << std::endl;
		}
	}

#endif

}
