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
#include <string>
#endif	// DEBUG

#include <BlendInt/Core/RefPtr.hpp>

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

		Object ()
		: reference_count_(0)
		{
		}

		virtual ~Object ()
		{
		}

		Object (const Object& orig)
		: reference_count_(0)
		{
		}

		Object& operator = (const Object& orig);

		inline size_t reference_count ()
		{
			return reference_count_;
		}

	private:

		template <typename T> friend class RefPtr;

		size_t reference_count_;

	};

}

#endif /* _BLENDINT_OBJECT_HPP_ */
