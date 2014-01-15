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

#include <BlendInt/Object.hpp>

#ifdef DEBUG
#include <iostream>
#include <stdexcept>
#endif

namespace BlendInt {

	ObjectEvents* Object::events = 0;

	Object::Object()
#ifdef DEBUG
	: m_id(0)
#endif
	{
		m_supers.reset(new std::set<Object*>);
		m_subs.reset(new std::set<Object*>);

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

	Object::Object(Object* super)
#ifdef DEBUG
	: m_id(0)
#endif
	{
		m_supers.reset(new std::set<Object*>);
		m_subs.reset(new std::set<Object*>);

		AttachTo (super);

#ifdef DEBUG
		// generate a unique id
		uint64_t temp = id_last;

		while (obj_map.count(id_last) == 1) {
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
		std::set<Object*>::iterator it;
		while (m_supers->size()) {
			it = m_supers->begin();
			(*it)->m_subs->erase(this);
			m_supers->erase(it);
		}

		Object* sub;
		while(m_subs->size()) {
			it = m_subs->begin();
			sub = *it;
			(*it)->m_supers->erase(this);
			m_subs->erase(it);
			if(sub->m_supers->size() == 0) {
				delete sub;
				sub = 0;
			}
		}

#ifdef DEBUG
		unregister_from_map();
#endif
	}

	bool Object::Attach (Object* sub)
	{
		if (!sub) return false;
		if (sub == this) return false;	// cannot bind self

		if(m_subs->count(sub)) return true;

		sub->m_supers->insert(this);

		m_subs->insert(sub);

		return true;
	}

	bool Object::Detach (Object* sub)
	{
		if(!sub) return false;
		if(sub == this) return false;

		if(!m_subs->count(sub))
			return false;

		sub->m_supers->erase(this);
		m_subs->erase(sub);
		return true;
	}

	void Object::DetachAllSubs ()
	{
		std::set<Object*>::iterator it;

		while (m_subs->size()) {
			it = m_subs->begin();
			(*it)->m_supers->erase(this);
			m_subs->erase(it);
		}
	}

	bool Object::DetachFrom (Object* super)
	{
		if(!super) return false;
		if(super == this) return false;

		if(!m_supers->count(super))
			return false;

		super->m_subs->erase(this);
		m_supers->erase(super);
		return true;
	}

	void Object::DetachFromAllSupers ()
	{
		std::set<Object*>::iterator it;

		while (m_supers->size()) {
			it = m_supers->begin();
			(*it)->m_subs->erase(this);
			m_supers->erase(it);
		}
	}

	bool Object::AttachTo (Object* super)
	{
		if (!super) return false;
		if (super == this) return false;	// cannot bind self

		if (m_supers->count(super)) return true;

		super->m_subs->insert(this);
		m_supers->insert(super);
		return true;
	}

	bool Object::Destroy (Object* sub)
	{
		if(Detach(sub)) {
			if(!sub->m_supers->size()) {
				delete sub;
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	}

	size_t Object::GetReferenceCount()
	{
		return m_supers->size();
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

	void Object::PrintSupers ()
	{
		std::set<Object*>::iterator it;

		std::cout << "Superior(s) of " << m_name << ":";
		for(it = m_supers->begin(); it!= m_supers->end(); it++)
		{
			std::cout << " " << (*it)->m_name << "(" << (*it)->m_supers->size() << ")";
		}
		std::cout << std::endl;
	}


	void Object::PrintSubs ()
	{
		std::set<Object*>::iterator it;
		std::cout << "Subordinate(s) of " << m_name << ":";
		for(it = m_subs->begin(); it!= m_subs->end(); it++)
		{
			std::cout << " " <<  (*it)->m_name << "(" << (*it)->m_supers->size() << ")";
		}
		std::cout << std::endl;
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

