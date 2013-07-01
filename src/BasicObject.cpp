/*
 * BasicObject.cpp
 *
 *  Created on: 2013年7月1日
 *      Author: zhanggyb
 */

#include <iostream>

#include "BasicObject.h"

namespace BIL {

uint64_t BasicObject::id_last = 1;

std::map<uint64_t, BasicObject*> BasicObject::map;

BasicObject::BasicObject ()
{
	// TODO Auto-generated constructor stub
	uint64_t temp = id_last;

	while (BasicObject::map.count(id_last) == 1) {
		id_last++;
		if (temp == id_last)
			throw;	// TODO: define exception
	}

	_id = id_last;

	registerObj();
	id_last++;
}

BasicObject::~BasicObject ()
{
	// TODO Auto-generated destructor stub
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

} /* namespace BIL */
