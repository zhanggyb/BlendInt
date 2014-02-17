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

#include <BlendInt/Core/Object.hpp>

#ifdef DEBUG
#include <iostream>
#include <stdexcept>
#endif

namespace BlendInt {

	Object::Object()
	: m_ref_count(0)
#ifdef DEBUG
	, m_id(0)
#endif
	{

#ifdef DEBUG
		// generate a unique id
		uint64_t temp = id_last;

		while (obj_map.count(id_last)) {
			id_last++;
			if (temp == id_last)
				throw std::out_of_range("Cannot assign unique id for object");
		}

		m_id = id_last;

		register_in_map();
		id_last++;
#endif
	}

	Object::~Object()
	{
#ifdef DEBUG
		unregister_from_map();
#endif
	}

	void Object::Destroy(Object *obj)
	{
		if(obj == 0) return;

		if(obj->m_ref_count == 0) {
#ifdef DEBUG
			std::cerr << "Warning: object " << obj->name() << " deleted with no reference" << std::endl;
#endif
			delete obj;
		} else {
			obj->m_ref_count--;
			if(obj->m_ref_count == 0) {
				delete obj;
			}
		}
	}

	void Object::Retain(Object* obj)
	{
		obj->m_ref_count++;
	}

#ifdef DEBUG

	uint64_t Object::id_last = 1;

	std::map<uint64_t, Object*> Object::obj_map;

	Object* Object::Find (uint64_t id)
	{
		Object *ret = 0;
		if (obj_map.count(id))
			ret = obj_map[id];

		return ret;
	}

	inline bool Object::register_in_map ()
	{
		obj_map[m_id] = this;
		return true;
	}

	inline bool Object::unregister_from_map ()
	{
		obj_map.erase(m_id);
		return true;
	}

	void Object::Print ()
	{
		std::map<uint64_t, Object*>::iterator it;
		std::cerr << "Print objects: "<< std::endl;
		for(it = obj_map.begin(); it != obj_map.end(); it++)
		{
			std::cerr << it->second->m_name << " ";
		}
		std::cerr << std::endl;
	}

	bool Object::CheckAllocatedObjects ()
	{
		unsigned int mapsize = Object::GetMapSize();
		std::cout << "map size: " << mapsize << std::endl;

		if(mapsize > 0) {
			std::map<uint64_t, Object*>::const_iterator it;
			for (it = Object::GetMap().begin(); it != Object::GetMap().end(); it++)
			{
				std::cout << "id: " << it->first << " name: " << it->second->name() << " was not deleted!" << std::endl;
			}
		}

		return (mapsize == 0);
	}

#endif

}

