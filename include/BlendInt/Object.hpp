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

#include <set>
#ifdef DEBUG
#include <map>
#endif	// DEBUG
#include <boost/smart_ptr.hpp>

namespace BlendInt {

	class Object
	{
	public:

		//void* operator new(size_t);

		//void operator delete (void*);

		Object ();

		Object (Object* parent);

		~Object ();

		bool Bind (Object* sub);

		bool Unbind (Object* sub);

		bool UnbindFrom (Object* super);

		void UnbindFromAll ();

		bool BindTo (Object* super);

		inline void set_name (const char* name)
		{
			m_name = name;
		}

		inline void set_name (const std::string& name)
		{
			m_name = name;
		}

		unsigned int ref_count () const {return m_ref_count;}

	private:

		unsigned int m_ref_count;

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
