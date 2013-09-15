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

#ifndef _BIL_TRACEABLE_HPP_
#define _BIL_TRACEABLE_HPP_

#include <stdint.h>
#include <cstdlib>
#include <list>
#include <map>

/**
 * A macro to disallow the copy constructor and operator= functions
 * This should be used in the private: declarations for a class
 */
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	private: \
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
	DISALLOW_COPY_AND_ASSIGN(Traceable);

	public:
		// static

		static Traceable* find (uint64_t id);

		static unsigned int mapSize ()
		{
			return obj_map.size();
		}

		static const map<uint64_t, Traceable*>& getMap ()
		{
			return obj_map;
		}

#ifdef DEBUG
		static void reset ()
		{
			id_last = 1;
			obj_map.clear();
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
		uint64_t id () const
		{
			return m_id;
		}
		/**
		 * @brief get the parent pointer
		 * @return
		 */
		const Traceable* parent () const
		{
			return m_parent;
		}
		const std::list<Traceable*>& children () const
		{
			return m_children;
		}

#ifdef DEBUG
	public:
#else
	protected:
#endif
		// member functions

		/**
		 * @brief set parent object
		 * @param parent parent pointer
		 * @return true or false
		 *
		 * Set the parent object and add self into the children list of parent
		 */
		void set_parent (Traceable* parent);

		bool add_child (Traceable *child);

		bool remove_child (Traceable* child);

		void delete_children ();

	protected:

		//ChildrenList<Traceable*> _children;
		std::list<Traceable*> m_children;

		Traceable* m_parent; /** parent object */

	private:

		inline bool register_in_map ();

		inline bool unregister_from_map ();

		// member variables

		uint64_t m_id; /** A unique ID for object */

		// static member variables
		static uint64_t id_last;

		static map<uint64_t, Traceable*> obj_map;
	};

} /* namespace BIL */
#endif /* _BIL_TRACEABLE_HPP_ */
