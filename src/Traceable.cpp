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
#include <algorithm>

#include <BILO/Traceable.hpp>
#include <stdexcept>

using namespace std;

namespace BILO {

	uint64_t Traceable::id_last = 1;

	map<uint64_t, Traceable*> Traceable::obj_map;

	Traceable::Traceable (Traceable* parent)
			: m_parent(parent)
	{
		// generate a unique id
		uint64_t temp = id_last;

		while (Traceable::obj_map.count(id_last) == 1) {
			id_last++;
			if (temp == id_last)
				throw std::out_of_range("Cannot assign unique id for object");
		}

		m_id = id_last;

		register_in_map();
		id_last++;

		if (m_parent) {
			(m_parent->m_children).push_back(this);
		}
	}

	Traceable::~Traceable ()
	{
		Traceable* item = 0;
		while (m_children.size() > 0) {
			item = m_children.back();
			m_children.pop_back();
			if (item) {
				remove_child(item);
				delete item;
			}
		}

		m_children.clear();

		if (m_parent) {
			// _parent->removeChild(this);	// Be careful of this line
			// parent_->children_.erase(this);
			m_parent->m_children.remove(this);
		}

		unregister_from_map();
	}

	inline bool Traceable::register_in_map ()
	{
		Traceable::obj_map[m_id] = this;
		return true;
	}

	Traceable* Traceable::find (uint64_t id)
	{
		Traceable *ret = NULL;
		if (Traceable::obj_map.count(id) == 1)
			ret = Traceable::obj_map[id];

		return ret;
	}

	inline bool Traceable::unregister_from_map ()
	{
		Traceable::obj_map.erase(m_id);
		return true;
	}

	void Traceable::set_parent (Traceable* parent)
	{
		if (!m_parent) {
			if (!parent) {
				return;
			}
		} else {
			m_parent->remove_child(this);
		}

		if (parent) {
			m_parent = parent;
			// return (_parent->addChild(this));
			m_parent->m_children.push_back(this);
		} else {
			m_parent = 0;
		}
	}

	bool Traceable::add_child (Traceable* child)
	{
		if (!child)
			return false;

//		if (child->parent_ == this)
//			return false;

		if (child->m_parent) {
			(child->m_parent)->remove_child(child);
		}

		m_children.push_back(child);
		child->m_parent = this;

		return true;
	}

	bool Traceable::remove_child (Traceable* child)
	{
		if (child->m_parent != this)
			return false;

		if (!child)
			return false;

		//children_.erase(child);
		//child->setParent(NULL);
		m_children.remove(child);
		child->m_parent = 0;

		return true;
	}

	void Traceable::delete_children ()
	{
		Traceable* item = 0;

		while (m_children.size() > 0) {
			item = m_children.back();
			m_children.pop_back();
			if (item)
				delete item;
		}

		m_children.clear();
	}

} /* namespace BILO */
