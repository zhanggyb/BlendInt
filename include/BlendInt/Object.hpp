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

namespace BlendInt {

	class Interface;

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
			return m_ref_count;
		}

#ifdef DEBUG
		static void CountOnce (Object* obj);
#endif

		static void Destroy (Object* obj);

	protected:

		virtual ~Object ();

#ifndef DEBUG
		static void CountOnce (Object* obj);
#endif

	private:

		size_t m_ref_count;

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
