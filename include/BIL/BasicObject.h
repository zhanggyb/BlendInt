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

#include <BIL/ChildrenList.h>

namespace BIL {

	/**
	 * The basic class which has a unique ID, one parent and some children
	 */
	class BasicObject
	{

	public:

		BasicObject (BasicObject* parent = NULL);

		virtual ~BasicObject ();

		uint64_t getId () const
		{
			return _id;
		}

		static BasicObject* find (uint64_t id);

		const BasicObject* getParent (void) const
		{
			return _parent;
		}

		bool setParent (BasicObject* parent);

		bool addChild (BasicObject *child);

		static unsigned int mapSize (void)
		{
			return map.size();
		}

#ifdef DEBUG
		static void reset (void)
		{
			id_last = 1;
			map.clear();
		}
#endif

	protected:

		bool removeChild (BasicObject* child);

		ChildrenList<BasicObject*> _children;

	private:

		inline bool registerObj (void);

		inline bool unregisterObj (void);

		BasicObject (const BasicObject& orig);

		BasicObject& operator = (const BasicObject& orig);

		uint64_t _id; /** A unique ID for object */

		BasicObject* _parent;

		static uint64_t id_last;
		static std::map<uint64_t, BasicObject*> map;

	};

} /* namespace BIL */
#endif /* _BIL_BASICOBJECT_H_ */
