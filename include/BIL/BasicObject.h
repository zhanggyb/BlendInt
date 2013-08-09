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
	 * @brief The basic class which has a unique ID, one parent and some children
	 *
	 * BasicObject is the base class of most objects in this library, it provides:
	 *	- unique id: each instance has its unique id
	 *	- parent pointer: each instance has its one parent (or NULL)
	 *	- children: each instance has its children, when it's deleted, all children
	 *	will be deleted too. It's a common behavior in GUI
	 */
	class BasicObject
	{

	public:

		/**
		 * @brief default constructor
		 * @param parent the parent pointer
		 */
		BasicObject (BasicObject* parent = NULL);

		virtual ~BasicObject ();

		/**
		 * @brief get the object id
		 * @return
		 */
		uint64_t getId () const
		{
			return _id;
		}

		static BasicObject* find (uint64_t id);

		/**
		 * @brief get the parent pointer
		 * @return
		 */
		const BasicObject* getParent (void) const
		{
			return _parent;
		}

		/**
		 * @brief set parent object
		 * @param parent parent pointer
		 * @return true or false
		 *
		 * Set the parent object and add self into the children list of parent
		 */
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

		void deleteChildren (void);

	private:

		inline bool registerObj (void);

		inline bool unregisterObj (void);

		/**
		 * @brief Declare copy constructor in private to disable it
		 */
		BasicObject (const BasicObject& orig);

		/**
		 * @brief Declare assignment constructor in private to disable it
		 */
		BasicObject& operator = (const BasicObject& orig);

		uint64_t _id; /** A unique ID for object */

		BasicObject* _parent; /** parent object */

		static uint64_t id_last;
		static std::map<uint64_t, BasicObject*> map;

	};

} /* namespace BIL */
#endif /* _BIL_BASICOBJECT_H_ */
