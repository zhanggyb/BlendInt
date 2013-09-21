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
	}

	ContextManager::ContextManager ()
	{

	}

	ContextManager::~ContextManager ()
	{

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

			list<Drawable*>* p = m_layers[map_it->second];
			list<Drawable*>::iterator it = find(p->begin(), p->end(), obj);
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

			map<int, list<Drawable*>* >::iterator layer_it;
			layer_it = m_layers.find(obj->z());
			if(layer_it != m_layers.end()) {
				layer_it->second->push_back(obj);
			} else {
				list<Drawable*>* new_list = new list<Drawable*>;
				new_list->push_back(obj);
				m_layers[obj->z()] = new_list;
			}
			
		} else {

			map<int, list<Drawable*>* >::iterator layer_it;
			layer_it = m_layers.find(obj->z());
			if(layer_it != m_layers.end()) {
				layer_it->second->push_back(obj);
			} else {
				list<Drawable*>* new_list = new list<Drawable*>;
				new_list->push_back(obj);
				m_layers[obj->z()] = new_list;
			}
			
		}

		m_index[obj] = obj->z();
		return true;
	}

	void ContextManager::bind (Drawable* obj)
	{
		if (!obj) return;

		if (obj->m_parent_new.object.nameless) {
			if (obj->m_parent_new.type == ParentContextManager) {
				// TODO: change to use a new function
//				ContextManager::instance()->remove_drawable(obj);
			}
			if (obj->m_parent_new.type == ParentDrawable) {
				obj->m_parent_new.object.drawable->m_children_new.erase(obj);
			}
		}

		if(add_drawable(obj)) {
			std::cerr << "add object" << std::endl;
		} else {
			std::cerr << "obj already in contextmanager with the same layer" << std::endl;
		}

		obj->m_parent_new.type = ParentContextManager;
		obj->m_parent_new.object.context = this;

		return;
	}

	void ContextManager::unbind (Drawable* obj)
	{
		if (!obj) return;

		if (!obj->m_parent_new.object.nameless) return;

		if (obj->m_parent_new.type == ParentDrawable) {
			std::cerr << "obj not in context manager, won't unbind it" << std::endl;
			return;
		}

		if(remove_drawable(obj)) {
			std::cerr << "remove object" << std::endl;
		} else {
			std::cerr << "obj not in in contextmanager with the same layer" << std::endl;
		}

		obj->m_parent_new.type = ParentUnknown;
		obj->m_parent_new.object.nameless = 0;

		return;
	}

	bool ContextManager::remove_drawable (Drawable* obj)
	{
		if (!obj) return false;

		map<Drawable*, int>::iterator map_it;
		
		map_it = m_index.find(obj);

		if(map_it != m_index.end()) {

			list<Drawable*>* p = m_layers[map_it->second];
			list<Drawable*>::iterator it = find(p->begin(), p->end(), obj);
			if (it != p->end()) {
				p->erase (it);
			} else {
#ifdef DEBUG
				std::wcerr << "Error: object " << obj->name() << " is not recorded" << std::endl;
#endif
			}

			if (p->empty()) {
				m_layers.erase(map_it->second);
				delete p;
			}

			m_index.erase(obj);

		} else {
#ifdef DEBUG
			std::wcerr << "Error: object " << obj->name() << " is not recorded" << std::endl;
#endif
			return false;
		}

		return true;
	}

#ifdef DEBUG

	void ContextManager::print ()
	{
		LayerType::iterator map_it;
		ListType::iterator list_it;

		ListType* plist;
		std::cout << std::endl;

		std::cout << "size of index map:" << m_index.size() << std::endl;

		std::cout << "size of layer map:" << m_layers.size() << std::endl;

		for(map_it = m_layers.begin(); map_it != m_layers.end(); map_it++)
		{
			std::cout << "Layer: " << map_it->first << std::endl;
			plist = map_it->second;
			for(list_it = plist->begin(); list_it != plist->end(); list_it++)
			{
				std::wcout << (*list_it)->name() << " ";
			}
			std::cout << std::endl;
		}
	}

#endif

}
