/*
 * This file is part of BILO (Blender Interface Library).
 *
 * BILO (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <iostream>
#include <stdlib.h>

#include <BILO/ContextManager.hpp>

#include <BILO/Drawable.hpp>

using std::cout;
using std::cerr;
using std::endl;

namespace BILO {

	ContextManager* ContextManager::context_manager = 0;

	ContextManager* ContextManager::instance ()
	{
		if (!context_manager) {
			cerr << "The Context Manager is not initialized successfully! Exit"
			        << endl;
			exit(EXIT_FAILURE);
		}

		return context_manager;
	}

	bool ContextManager::initialize ()
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

	void ContextManager::release ()
	{
		// TODO: remove all drawable objects
		if(context_manager) delete context_manager;

		context_manager = 0;
	}

	ContextManager::ContextManager ()
	{

	}

	ContextManager::~ContextManager ()
	{
		map<int, set<Drawable*>* >::iterator map_it;
		set<Drawable*>::iterator set_it;
		for(map_it = m_layers.begin(); map_it != m_layers.end(); map_it++)
		{
			set<Drawable*>* pset = map_it->second;
			for(set_it = pset->begin(); set_it != pset->end(); set_it++)
			{
				delete *set_it;
			}
		}

		m_layers.clear();
		m_index.clear();
	}

	bool ContextManager::add_drawable (Drawable* obj)
	{
		if (!obj) return false;

		map<Drawable*, int>::iterator map_it;
		
		map_it = m_index.find(obj);

		if(map_it != m_index.end()) {
			if (map_it->second == obj->z()) {
				return false;
			}

			set<Drawable*>* p = m_layers[map_it->second];
			set<Drawable*>::iterator it = p->find(obj);
			if (it != p->end()) {
				p->erase (it);
			} else {
#ifdef DEBUG
				std::cerr << "Note: object is not recorded" << std::endl;
#endif
			}

			if (p->empty()) {
				m_layers.erase(map_it->second);
				delete p;
			}

			map<int, set<Drawable*>* >::iterator layer_it;
			layer_it = m_layers.find(obj->z());
			if(layer_it != m_layers.end()) {
				layer_it->second->insert(obj);
			} else {
				set<Drawable*>* new_set = new set<Drawable*>;
				new_set->insert(obj);
				m_layers[obj->z()] = new_set;
			}
			
		} else {

			map<int, set<Drawable*>* >::iterator layer_it;
			layer_it = m_layers.find(obj->z());
			if(layer_it != m_layers.end()) {
				layer_it->second->insert(obj);
			} else {
				set<Drawable*>* new_set = new set<Drawable*>;
				new_set->insert(obj);
				m_layers[obj->z()] = new_set;
			}
			
		}

		m_index[obj] = obj->z();
		return true;
	}

	void ContextManager::bind (Drawable* obj)
	{
		if (!obj) return;

		if (obj->m_parent.object.nameless) {
			if (obj->m_parent.type == ParentDrawable) {
				obj->m_parent.object.drawable->m_children.erase(obj);
			}
		}

		if(add_drawable(obj)) {
			std::cerr << "add object" << std::endl;
		} else {
			std::cerr << "obj already in contextmanager with the same layer" << std::endl;
		}

		obj->m_parent.type = ParentContextManager;
		obj->m_parent.object.context = this;

		return;
	}

	void ContextManager::unbind (Drawable* obj)
	{
		if (!obj) return;

		if (!obj->m_parent.object.nameless) return;

		if (obj->m_parent.type == ParentDrawable) {
			std::cerr << "obj not in context manager, won't unbind it" << std::endl;
			return;
		}

		if(remove_drawable(obj)) {
			std::cerr << "remove object" << std::endl;
		} else {
			std::cerr << "obj not in in contextmanager with the same layer" << std::endl;
		}

		obj->m_parent.type = ParentUnknown;
		obj->m_parent.object.nameless = 0;

		return;
	}

	bool ContextManager::remove_drawable (Drawable* obj)
	{
		if (!obj) return false;

		map<Drawable*, int>::iterator map_it;
		
		map_it = m_index.find(obj);

		if(map_it != m_index.end()) {

			set<Drawable*>* p = m_layers[map_it->second];
			set<Drawable*>::iterator it = p->find(obj);
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
