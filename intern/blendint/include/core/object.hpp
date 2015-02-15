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

#pragma once

#include <stdint.h>

#ifdef DEBUG
#include <map>
#include <string>
#endif	// DEBUG

#include <core/refptr.hpp>

namespace BlendInt {

	/**
	 * @brief The base class with reference count
	 */
	class Object
	{
	public:

		inline Object ()
		: reference_count_(0)
		{
		}

		virtual ~Object ()
		{
		}

		inline size_t reference_count ()
		{
			return reference_count_;
		}

	private:

		// disallow copy and assignment:
		Object (const Object& orig);
		Object& operator = (const Object& orig);

		template <typename T> friend class RefPtr;

		size_t reference_count_;

	};

}
