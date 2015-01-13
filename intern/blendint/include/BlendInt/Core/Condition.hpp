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

#ifndef _BLENDINT_CORE_CONDITION_HPP_
#define _BLENDINT_CORE_CONDITION_HPP_

#ifdef __UNIX__
#include <pthread.h>
#endif

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Core/Mutex.hpp>

namespace BlendInt {

	class Condition
	{
	public:

		inline Condition ()
		{}

		inline ~Condition ()
		{}

		inline bool initialize ()
		{return true;}

		inline void destroy ()
		{

		}

		inline bool signal ()
		{
			return true;
		}

		inline bool broadcast ()
		{
			return true;
		}

		inline bool wait (Mutex* mutex)
		{
			return true;
		}

	};

}

#endif /* _BLENDINT_CORE_CONDITION_HPP_ */
