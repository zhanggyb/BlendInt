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
#endif

namespace BlendInt {

	Object::Object()
	: m_ref_count(0)
#ifdef DEBUG
	, m_id(0)
#endif
	{
		m_superiors.reset(new std::set<Object*>);
		m_subordinates.reset(new std::set<Object*>);

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

	Object::Object(Object* parent)
	: m_ref_count(0)
#ifdef DEBUG
	, m_id(0)
#endif
	{
		m_superiors.reset(new std::set<Object*>);
		m_subordinates.reset(new std::set<Object*>);

		BindTo (parent);

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
#ifdef DEBUG
		std::cout << "Destructor in object: " << m_name << "(" << m_ref_count << ")" << std::endl;
#endif

		std::set<Object*>::iterator it;
		while (m_superiors->size()) {
			it = m_superiors->begin();
			(*it)->m_subordinates->erase(this);
			m_ref_count--;
			m_superiors->erase(it);
		}

		Object* sub;
		while(m_subordinates->size()) {
			it = m_subordinates->begin();
			sub = *it;
			(*it)->m_superiors->erase(this);
			(*it)->m_ref_count--;
			m_subordinates->erase(it);
			if(sub->m_ref_count == 0) {
				delete sub;
				sub = 0;
			}
		}

#ifdef DEBUG
		unregister_from_map();
#endif
	}

	bool Object::Bind (Object* sub)
	{
		if (!sub) return false;
		if (sub == this) return false;	// cannot bind self

		if(m_subordinates->count(sub)) return true;

		sub->m_superiors->insert(this);
		sub->m_ref_count++;

		m_subordinates->insert(sub);

		return true;
	}

	bool Object::Unbind (Object* sub)
	{
		if(!sub) return false;
		if(sub == this) return false;

		if(!m_subordinates->count(sub))
			return false;

		sub->m_superiors->erase(this);
		sub->m_ref_count--;
		m_subordinates->erase(sub);
		return true;
	}

	bool Object::UnbindFrom (Object* super)
	{
		if(!super) return false;
		if(super == this) return false;

		if(!m_superiors->count(super))
			return false;

		super->m_subordinates->erase(this);
		m_ref_count--;
		m_superiors->erase(super);
		return true;
	}

	void Object::UnbindFromAll ()
	{
		std::set<Object*>::iterator it;

		while (m_superiors->size()) {
			it = m_superiors->begin();
			(*it)->m_subordinates->erase(this);
			m_ref_count--;
			m_superiors->erase(it);
		}
	}

	bool Object::BindTo (Object* super)
	{
		if (!super) return false;
		if (super == this) return false;	// cannot bind self

		if (m_superiors->count(super)) return true;

		super->m_subordinates->insert(this);
		m_ref_count++;
		m_superiors->insert(super);
		return true;
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
		for(it = m_superiors->begin(); it!= m_superiors->end(); it++)
		{
			std::cout << " " << (*it)->m_name << "(" << (*it)->ref_count() << ")";
		}
		std::cout << std::endl;
	}


	void Object::PrintSubs ()
	{
		std::set<Object*>::iterator it;
		std::cout << "Subordinate(s) of " << m_name << ":";
		for(it = m_subordinates->begin(); it!= m_subordinates->end(); it++)
		{
			std::cout << " " <<  (*it)->m_name << "(" << (*it)->ref_count() << ")";
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

#endif

}
