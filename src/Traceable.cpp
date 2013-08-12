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

#include <BIL/Traceable.h>

using namespace std;

namespace BIL {

	uint64_t Traceable::id_last = 1;

	map<uint64_t, Traceable*> Traceable::objMap;

	list<Traceable*> Traceable::solo;

	Traceable::Traceable (Traceable* parent)
			: _parent(parent)
	{
		// generate a unique id
		uint64_t temp = id_last;

		while (Traceable::objMap.count(id_last) == 1) {
			id_last++;
			if (temp == id_last)
				throw;	// TODO: define exception
		}

		_id = id_last;

		registerObj();
		id_last++;

		if (_parent != NULL) {
			(_parent->_children).push_back(this);
		} else {
			solo.push_back(this);
		}
	}

	Traceable::~Traceable ()
	{
		Traceable* item = NULL;
		while (_children.size() > 0) {
			item = _children.back();
			_children.pop_back();
			if (item != NULL) delete item;
		}

		_children.clear();

		if (_parent != NULL) {
			_parent->removeChild(this);	// Be careful of this line
		} else {
			list<Traceable*>::iterator it;
			it = std::find(solo.begin(), solo.end(), this);
			if(it != solo.end()) {
				solo.remove(this);
			}
		}

		unregisterObj();
	}

	inline bool Traceable::registerObj (void)
	{
		Traceable::objMap[_id] = this;
		return true;
	}

	Traceable* Traceable::find (uint64_t id)
	{
		Traceable *ret = NULL;
		if (Traceable::objMap.count(id) == 1)
			ret = Traceable::objMap[id];

		return ret;
	}

	inline bool Traceable::unregisterObj (void)
	{
		Traceable::objMap.erase(_id);
		return true;
	}

	bool Traceable::setParent (Traceable* parent)
	{
		if (_parent == parent) return true;

		if (_parent == NULL) {
			if (parent == NULL) {
				return true;
			}

			list<Traceable*>::iterator it;
			it = std::find(solo.begin(), solo.end(), this);
			if(it != solo.end()) {
				solo.remove(this);
			}
		} else {

			_parent->removeChild(this, false);

			if (parent != NULL) {
				_parent = parent;
				return (_parent->addChild(this));
			} else {
				_parent = NULL;
				solo.push_back(this);
			}

		}
		return true;
	}

	bool Traceable::addChild (Traceable* child)
	{
		if (child == NULL)
			return false;

		if (child->_parent != NULL) {
			(child->_parent)->removeChild(child, false);
		}

		_children.push_back(child);
		child->_parent = this;

		return true;
	}

	bool Traceable::removeChild (Traceable* child, bool registersolo)
	{
		if(child->_parent != this) return false;

		if (child == NULL)
			return false;

		_children.erase(child);
		//child->setParent(NULL);
		child->_parent = NULL;
		if(registersolo) {
			solo.push_back(child);
		}

		return true;
	}


	void Traceable::deleteChildren (void)
	{
		Traceable* item = NULL;

		while (_children.size() > 0) {
			item = _children.back();
			_children.pop_back();
			if (item != NULL) delete item;
		}

		_children.clear();
	}

} /* namespace BIL */
