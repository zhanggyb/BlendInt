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
#include <algorithm>

#include <BIL/Traceable.hpp>

using namespace std;

namespace BIL {

	uint64_t Traceable::id_last = 1;

	map<uint64_t, Traceable*> Traceable::obj_map;

	list<Traceable*> Traceable::solos;

	Traceable::Traceable (Traceable* parent)
			: parent_(parent)
	{
		// generate a unique id
		uint64_t temp = id_last;

		while (Traceable::obj_map.count(id_last) == 1) {
			id_last++;
			if (temp == id_last)
				throw;	// TODO: define exception
		}

		id_ = id_last;

		registerObject();
		id_last++;

		if (parent_ != NULL) {
			(parent_->children_).push_back(this);
		} else {
			solos.push_back(this);
		}
	}

	Traceable::~Traceable ()
	{
		Traceable* item = NULL;
		while (children_.size() > 0) {
			item = children_.back();
			children_.pop_back();
			if (item) {
				removeChild(item);
				delete item;
			}
		}

		children_.clear();

		if (parent_) {
			// _parent->removeChild(this);	// Be careful of this line
			// parent_->children_.erase(this);
			parent_->children_.remove (this);
		} else {
			list<Traceable*>::iterator it;
			it = std::find(solos.begin(), solos.end(), this);
			if(it != solos.end()) {
				solos.remove(this);
			}
		}

		unregisterObject();
	}

	inline bool Traceable::registerObject ()
	{
		Traceable::obj_map[id_] = this;
		return true;
	}

	Traceable* Traceable::find (uint64_t id)
	{
		Traceable *ret = NULL;
		if (Traceable::obj_map.count(id) == 1)
			ret = Traceable::obj_map[id];

		return ret;
	}

	inline bool Traceable::unregisterObject ()
	{
		Traceable::obj_map.erase(id_);
		return true;
	}

	bool Traceable::setParent (Traceable* parent)
	{
		if (parent_ == parent) return true;

		if (parent_ == NULL) {
			if (parent == NULL) {
				return true;
			}

			list<Traceable*>::iterator it;
			it = std::find(solos.begin(), solos.end(), this);
			if(it != solos.end()) {
				solos.remove(this);
			}
		} else {
			parent_->removeChild(this, false);
		}

		if (parent != NULL) {
			parent_ = parent;
			// return (_parent->addChild(this));
			parent_->children_.push_back(this);
		} else {
			parent_ = NULL;
			solos.push_back(this);
		}

		return true;
	}

	bool Traceable::addChild (Traceable* child)
	{
		if (child == NULL)
			return false;

		if (child->parent_ == this)
			return false;

		if (child->parent_) {
			(child->parent_)->removeChild(child, false);
		} else {
			list<Traceable*>::iterator it;
			it = std::find(solos.begin(), solos.end(), child);
			if(it != solos.end()) {
				solos.remove(child);
			}
		}

		children_.push_back(child);
		child->parent_ = this;

		return true;
	}

	bool Traceable::removeChild (Traceable* child, bool registersolo)
	{
		if(child->parent_ != this) return false;

		if (child == NULL)
			return false;

		//children_.erase(child);
		//child->setParent(NULL);
		children_.remove(child);
		child->parent_ = NULL;
		if(registersolo) {
			solos.push_back(child);
		}

		return true;
	}


	void Traceable::deleteChildren ()
	{
		Traceable* item = NULL;

		while (children_.size() > 0) {
			item = children_.back();
			children_.pop_back();
			if (item != NULL) delete item;
		}

		children_.clear();
	}

	void Traceable::clearSoloList ()
	{
		list<Traceable*>::reverse_iterator it;
		for (it = solos.rbegin(); it != solos.rend(); it++)
		{
			delete *it;
		}
		solos.clear();
	}

} /* namespace BIL */
