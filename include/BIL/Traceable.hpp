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

#ifndef _BIL_TRACEABLE_H_
#define _BIL_TRACEABLE_H_

#include <stdint.h>
#include <cstdlib>
#include <list>
#include <map>

/**
 * A macro to disallow the copy constructor and operator= functions
 * This should be used in the private: declarations for a class
 */
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

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
		uint64_t id (void) const
		{
			return id_;
		}
		/**
		 * @brief get the parent pointer
		 * @return
		 */
		const Traceable* parent (void) const
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

	protected:	// member functions

		bool RemoveChild (Traceable* child, bool registersolo = true);

		void DeleteChildren (void);

	protected:

		//ChildrenList<Traceable*> _children;
		std::list<Traceable*> children_;

	private:

		inline bool RegisterObj (void);

		inline bool UnregisterObj (void);

	private:	// member variables

		uint64_t id_; /** A unique ID for object */

		Traceable* parent_; /** parent object */

	private:	// static member variables

		static uint64_t id_last;

		static map<uint64_t, Traceable*> objMap;

		static list<Traceable*> solo;

	private:	// member functions disabled

		DISALLOW_COPY_AND_ASSIGN(Traceable);
	};

} /* namespace BIL */
#endif /* _BIL_TRACEABLE_H_ */
