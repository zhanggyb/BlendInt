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

#include <BIL/BasicObject.h>

using namespace std;

namespace BIL {

	uint64_t BasicObject::id_last = 1;

	map<uint64_t, BasicObject*> BasicObject::map;

	BasicObject::BasicObject (BasicObject* parent)
			: _parent(parent)
	{
		// generate a unique id
		uint64_t temp = id_last;

		while (BasicObject::map.count(id_last) == 1) {
			id_last++;
			if (temp == id_last)
				throw;	// TODO: define exception
		}

		_id = id_last;

		registerObj();
		id_last++;

		if (_parent != NULL) {
			(_parent->_children).push_back(this);
		}
	}

	BasicObject::~BasicObject ()
	{
		BasicObject* item = NULL;
		while (_children.size() > 0) {
			item = _children.back();
			_children.pop_back();
			if (item != NULL) delete item;
		}

		_children.clear();

		if (_parent != NULL) {
			_parent->removeChild(this);	// Be careful of this line
		}

		unregisterObj();
	}

	inline bool BasicObject::registerObj (void)
	{
		BasicObject::map[_id] = this;
		return true;
	}

	BasicObject* BasicObject::find (uint64_t id)
	{
		BasicObject *ret = NULL;
		if (BasicObject::map.count(id) == 1)
			ret = BasicObject::map[id];

		return ret;
	}

	inline bool BasicObject::unregisterObj (void)
	{
		BasicObject::map.erase(_id);
		return true;
	}

	bool BasicObject::setParent (BasicObject* parent)
	{
		_parent = parent;

		if (_parent != NULL) {
			return (_parent->addChild(this));
		}

		return true;
	}

	bool BasicObject::addChild (BasicObject* child)
	{
		if (child == NULL)
			return false;

		if (child->_parent != NULL) {
			(child->_parent)->removeChild(child);
		}

		_children.push_back(child);
		child->_parent = this;

		return true;
	}

	bool BasicObject::removeChild (BasicObject* child)
	{
		if (child == NULL)
			return false;

		_children.erase(child);
		child->setParent(NULL);

		return true;
	}


	void BasicObject::deleteChildren (void)
	{
		BasicObject* item = NULL;

		while (_children.size() > 0) {
			item = _children.back();
			_children.pop_back();
			if (item != NULL) delete item;
		}

		_children.clear();
	}

} /* namespace BIL */
