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

namespace BIL {

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
		std::map<Drawable*, ContextIndex>::iterator map_it;
		std::list<ContextLayer>::iterator list_it;
		std::list<ContextLayer>::iterator list_it_next;
		std::list<Drawable*>::iterator obj_list_it;

		int layer_removed_index = 0;
		int list_removed_index = 0;

		bool remove_action = false;

		map_it = m_index.find(obj);

		list_it = m_layers.begin();
		if (map_it != m_index.end()) {
			std::advance(list_it, map_it->second.layer_index);
			if (list_it->layer == obj->z()) {
				return false;
			} else {
				layer_removed_index = std::distance (m_layers.begin(), list_it);
				obj_list_it = list_it->list.begin();
				std::advance(obj_list_it, map_it->second.list_index);
				std::list<Drawable*>::iterator new_it = list_it->list.erase(obj_list_it);
				list_removed_index = std::distance (list_it->list.begin(), new_it);

				// update the map after
				//update_map (map_it->second.layer_index, std::distance(list_it->list.begin(), new_it));

				remove_action = true;
			}
		}

		bool insert_action = false;
		int layer_index_update = 0;
		int list_index_update = 0;
		ContextIndex context_index;

		// look for the place where we insert the new Drawable object into the m_layers
		for (list_it = m_layers.begin(); list_it != m_layers.end();
				list_it++)
		{
			list_it_next = list_it;
			std::advance(list_it_next, 1);

			// if we have already record the layer of the new object, add to the list there
			if (list_it->layer == obj->z())
			{
				list_it->list.push_back(obj);
				context_index.layer_index = std::distance(m_layers.begin(), list_it);
				context_index.list_index = list_it->list.size() - 1;
				m_index[obj] = context_index;
				break;
			}

			// if we need to insert a new layer
			if ((list_it_next != m_layers.end())
					&& (list_it_next->layer > obj->z()))
			{
				ContextLayer insert_item;
				insert_item.layer = obj->z();
				insert_item.list.push_back(obj);
				list_it = m_layers.insert(list_it_next, insert_item);
				layer_index_update = std::distance(m_layers.begin(), list_it);
				list_index_update = list_it->list.size () - 1;
				context_index.layer_index = layer_index_update;
				context_index.list_index = list_index_update;
				m_index[obj] = context_index;
				layer_index_update++;
				list_index_update = 0;
				insert_action = true;
			}
		}

		// if not found, push back a new layer
		if (list_it == m_layers.end())
		{
			ContextLayer append_item;
			append_item.layer = obj->z();
			append_item.list.push_back(obj);
			m_layers.push_back(append_item);
			context_index.layer_index = m_layers.size() - 1;
			context_index.list_index = append_item.list.size() - 1;
			m_index[obj] = context_index;
		}

		// TODO: update the maps after
		if(remove_action) {
			layer_index_update = std::min (layer_removed_index, layer_index_update);
			list_index_update = std::min (list_removed_index, list_index_update);
		}
		if(insert_action) {
			update_map(layer_index_update, m_layers.size() - 1, list_index_update);
		}

		return true;
	}

	bool ContextManager::remove_drawable (Drawable* obj)
	{
		std::map<Drawable*, ContextIndex>::iterator map_it;
		std::list<ContextLayer>::iterator list_it;
		std::list<Drawable*>::iterator obj_list_it;

		map_it = m_index.find(obj);

		list_it = m_layers.begin();

		if(map_it == m_index.end()) {
			return false;
		}

		int layer_index = map_it->second.layer_index;
		int list_index = map_it->second.list_index;
		std::advance (list_it, layer_index);
		obj_list_it = list_it->list.begin();
		std::advance (obj_list_it, list_index);
		list_it->list.erase(obj_list_it);

		m_index.erase(obj);

		// TODO: update map
		update_map (layer_index, layer_index, list_index);

		return true;
	}

	void ContextManager::update_map (int layer_index1, int layer_index2, int list_index)
	{
		std::map<Drawable*, ContextIndex>::iterator a_map_it;
		std::list<ContextLayer>::iterator a_list_it;
		std::list<ContextLayer>::iterator a_list_it_end;
		std::list<Drawable*>::iterator a_obj_list_it;
		bool first = true;
		int layer_index_start = std::min(layer_index1, layer_index2);
		int layer_index_stop = std::max(layer_index1, layer_index2);

		a_list_it = m_layers.begin();
		std::advance(a_list_it, layer_index_start);

		int i = layer_index_start;
		for (; i <= layer_index_stop; a_list_it++, i++)
		{
			if (first) {
				a_obj_list_it = a_list_it->list.begin();
				std::advance(a_obj_list_it, list_index);
				for(; a_obj_list_it != a_list_it->list.end(); a_obj_list_it++)
				{
					m_index[*a_obj_list_it].layer_index = std::distance(m_layers.begin(), a_list_it);
					m_index[*a_obj_list_it].list_index = std::distance(a_list_it->list.begin(), a_obj_list_it);
				}
				first = false;
			} else {
				for(a_obj_list_it = a_list_it->list.begin(); a_obj_list_it != a_list_it->list.end(); a_obj_list_it++)
				{
					m_index[*a_obj_list_it].layer_index = std::distance(m_layers.begin(), a_list_it);
					m_index[*a_obj_list_it].list_index = std::distance(a_list_it->list.begin(), a_obj_list_it);
				}
			}
		}
	}

}
