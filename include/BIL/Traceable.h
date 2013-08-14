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

#ifndef _BIL_TRACEABLE_H_
#define _BIL_TRACEABLE_H_

#include <stdint.h>
#include <list>
#include <map>

#include <BIL/ChildrenList.h>

using namespace std;

namespace BIL {

	/**
	 * @brief The basic class which has a unique ID, one parent and some
	 * children
	 *
	 * BasicObject is the base class of most objects in this library, it
	 * provides:
	 *	- unique id: each instance has its unique id
	 *	- parent pointer: each instance has its one parent (or NULL)
	 *	- children: each instance has its children, when it's deleted, all
	 * children	will be deleted too. It's a common behavior in GUI
	 */
	class Traceable
	{

	public:	// static

		static Traceable* find (uint64_t id);

		static unsigned int mapSize (void)
		{
			return objMap.size();
		}

		static map<uint64_t, Traceable*>* getMap (void)
		{
			return &objMap;
		}

		static list<Traceable*>* getList (void)
		{
			return &solo;
		}

#ifdef DEBUG
		static void reset (void)
		{
			id_last = 1;
			objMap.clear();
		}
#endif

	public:

		/**
		 * @brief default constructor
		 * @param parent the parent pointer
		 */
		Traceable (Traceable* parent = NULL);

		virtual ~Traceable ();

		/**
		 * @brief get the object id
		 * @return
		 */
		uint64_t getId () const
		{
			return id_;
		}
		/**
		 * @brief get the parent pointer
		 * @return
		 */
		const Traceable* getParent (void) const
		{
			return parent_;
		}

		/**
		 * @brief set parent object
		 * @param parent parent pointer
		 * @return true or false
		 *
		 * Set the parent object and add self into the children list of parent
		 */
		bool setParent (Traceable* parent);

		bool addChild (Traceable *child);

	protected:

		bool removeChild (Traceable* child, bool registersolo = true);

		ChildrenList<Traceable*> _children;

		void deleteChildren (void);

	private:

		inline bool registerObj (void);

		inline bool unregisterObj (void);

	private:	// member functions disabled

		/**
		 * @brief Declare copy constructor in private to disable it
		 */
		Traceable (const Traceable& orig);

		/**
		 * @brief Declare assignment constructor in private to disable it
		 */
		Traceable& operator = (const Traceable& orig);

	private:	// member variables

		uint64_t id_; /** A unique ID for object */

		Traceable* parent_; /** parent object */

	private:	// static member variables

		static uint64_t id_last;

		static map<uint64_t, Traceable*> objMap;

		static list<Traceable*> solo;

	};

} /* namespace BIL */
#endif /* _BIL_TRACEABLE_H_ */
