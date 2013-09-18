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

#include <iostream>

#include <BIL/ContextManager.hpp>

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
		
		map_it = m_map.find(obj);

		if(map_it != m_map.end()) {
			if (map_it->second == obj->z()) {
				return false;
			}

			list<Drawable*>* p = m_layers[map_it->second];
			list<Drawable*>::iterator it = find(p->begin(), p->end(), obj);
			if (it != p->end()) {
				p->erase (it);
			} else {
#ifdef DEBUG
				std::cerr << "Error: object is not recorded" << std::endl;
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

		m_map[obj] = obj->z();
		return true;
	}

	bool ContextManager::remove_drawable (Drawable* obj)
	{
		if (!obj) return false;

		map<Drawable*, int>::iterator map_it;
		
		map_it = m_map.find(obj);

		if(map_it != m_map.end()) {

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

			m_map.erase(obj);

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

		std::cout << "size of index map:" << m_map.size() << std::endl;

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
