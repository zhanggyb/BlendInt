/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_OBJECT_HPP_
#define _BLENDINT_OBJECT_HPP_

#include <stdint.h>

#include <set>
#ifdef DEBUG
#include <map>
#endif	// DEBUG
#include <boost/smart_ptr.hpp>

#include <Cpp/Events.hpp>

namespace BlendInt {

	class Interface;

	class ObjectEvents
	{
	public:

		friend class Interface;

		ObjectEvents ()
		: m_connections(0)
		{
			m_connections = new Cpp::ConnectionScope;
		}

		Cpp::ConnectionScope* operator -> () const {return m_connections;}

	private:

		~ObjectEvents ()
		{
			delete m_connections;
		}

		Cpp::ConnectionScope* m_connections;
	};

	/**
	 * @brief The base class of most BlendInt objects
	 *
	 * Objects organize themselves in object trees. When you create a Object with another
	 * object as superior, the object will automatically add itself to the superior's sub set.
	 * The superior object takes ownership of the object; i.e., it will automatically delete
	 * its children in its sub objects. You can look for an object by name and optionally type
	 * using findChild() or findChildren().
	 */
	class Object
	{
	public:

		Object ();

		explicit Object (Object* super);

		virtual ~Object ();

		bool Attach (Object* sub);

		bool Detach (Object* sub);

		void DetachAllSubs ();

		bool DetachFrom (Object* super);

		void DetachFromAllSupers ();

		bool AttachTo (Object* super);

		/**
		 * @brief Detach and try to delete sub object
		 * @param[in] sub The sub object
		 * @return
		 * 	- true if success
		 * 	- false if fail
		 *
		 * This function detach and try to delete the sub object if it has no more super
		 * object.
		 *
		 * Return false if the sub is not in sub objects, or it's not deleted if it has
		 * more super object attaching.
		 */
		bool Destroy (Object* sub);

		inline void set_name (const char* name)
		{
			m_name = name;
		}

		inline void set_name (const std::string& name)
		{
			m_name = name;
		}

		const std::string& name () const {return m_name;}

		size_t GetReferenceCount ();

		inline const std::set<Object*>* superiors() const {return m_supers.get();}

		inline const std::set<Object*>* subordinates() const {return m_subs.get();}

	private:

		static ObjectEvents* events;

		boost::scoped_ptr<std::set<Object*> > m_supers;

		boost::scoped_ptr<std::set<Object*> > m_subs;

		std::string m_name;

#ifdef DEBUG
	public:

		static Object* Find (uint64_t id);

#ifdef __APPLE__
		static unsigned long GetMapSize ()
#else
        static unsigned int GetMapSize ()
#endif
		{
			return obj_map.size();
		}

		void PrintSupers ();

		void PrintSubs ();

		static const std::map<uint64_t, Object*>& GetMap ()
		{
			return obj_map;
		}

		static void Reset ()
		{
			id_last = 1;
			obj_map.clear();
		}

	private:

		inline bool register_in_map ();

		inline bool unregister_from_map ();

		uint64_t m_id; /** A unique ID for object */

		static uint64_t id_last;

		static std::map<uint64_t, Object*> obj_map;

	public:

		static void Print ();

		static bool CheckAllocatedObjects ();

#endif
	};

}

#endif /* _BLENDINT_OBJECT_HPP_ */
