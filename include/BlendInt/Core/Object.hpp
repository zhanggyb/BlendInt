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

#ifdef DEBUG
#include <map>
#endif	// DEBUG

#include <string>
#include <stdexcept>

namespace BlendInt {

	class ObjectPtr;

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

		static ObjectPtr Create ();

		inline void set_name (const char* name)
		{
			m_name = name;
		}

		inline void set_name (const std::string& name)
		{
			m_name = name;
		}

		const std::string& name () const {return m_name;}

		inline size_t ref_count ()
		{
			return m_count;
		}

#ifdef DEBUG
		static void Retain (Object* obj);
#endif

		static void Destroy (Object* obj);

	protected:

		Object ();

		virtual ~Object ();

#ifndef DEBUG
		static void Retain (Object* obj);
#endif

	private:

		size_t m_count;

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

		static const std::map<uint64_t, Object*>& GetMap ()
		{
			return obj_map;
		}

		static void ResetID ()
		{
			id_last = 1;
			obj_map.clear();
		}

	private:

		friend class ObjectPtr;

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

	class ObjectPtr
	{
	public:

		ObjectPtr ()
		: m_p(0)
		{
		}

		ObjectPtr (Object* p)
				: m_p(p)
		{
			++m_p->m_count;
		}

		ObjectPtr (const ObjectPtr& orig)
				: m_p(orig.m_p)
		{
			++m_p->m_count;
		}

		~ObjectPtr ()
		{
			if (--m_p->m_count == 0)
				delete m_p;
		}

		Object* operator-> ()
		{
			return m_p;
		}

		Object& operator* ()
		{
			if(!m_p) {
				throw std::logic_error("No object stored!");
			}
			return *m_p;
		}

		ObjectPtr& operator = (const ObjectPtr& orig)
		{
			if(m_p) {

				Object* const old = m_p;
				m_p = orig.m_p;
				++m_p->m_count;

				if (--old->m_count == 0)
					delete old;

			} else {
				m_p = orig.m_p;
				++m_p->m_count;
			}

			return *this;
		}

	private:
		Object* m_p;
	};

}

#endif /* _BLENDINT_OBJECT_HPP_ */
