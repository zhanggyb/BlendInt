/*
 * BasicObject.h
 *
 *  Created on: 2013年7月1日
 *      Author: zhanggyb
 */

#ifndef _BIL_BASICOBJECT_H_
#define _BIL_BASICOBJECT_H_

#include <stdint.h>
#include <map>

namespace BIL {

class BasicObject {

public:

	BasicObject();

	virtual ~BasicObject();

	uint64_t getId() const {
		return _id;
	}

	static BasicObject* find (uint64_t id);

private:

	inline bool registerObj (void);

	inline bool unregisterObj (void);

	BasicObject (const BasicObject& orig);

	BasicObject& operator = (const BasicObject& orig);

	uint64_t _id;	/** A unique ID for object */

	static uint64_t id_last;
	static std::map<uint64_t, BasicObject*> map;

};

} /* namespace BIL */
#endif /* _BIL_BASICOBJECT_H_ */
