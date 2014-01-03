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

namespace BlendInt {

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

		Object (Object* superior);

		virtual ~Object ();

		bool Bind (Object* sub);

		bool Unbind (Object* sub);

		void UnbindAll ();

		bool UnboundFrom (Object* super);

		void UnboundFromAll ();

		bool BoundTo (Object* super);

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

		inline const std::set<Object*>* superiors() const {return m_superiors.get();}

		inline const std::set<Object*>* subordinates() const {return m_subordinates.get();}

	private:

		boost::scoped_ptr<std::set<Object*> > m_superiors;

		boost::scoped_ptr<std::set<Object*> > m_subordinates;

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

#endif
	};

}

#endif /* _BLENDINT_OBJECT_HPP_ */
