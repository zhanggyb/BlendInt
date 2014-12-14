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

#ifdef __UNIX__
#include <stddef.h>
#include <sys/time.h>
#endif	// __UNIX__

#include <BlendInt/Core/Time.hpp>

namespace BlendInt {

	uint64_t Time::kSavedTime = 0;

	uint64_t Time::GetIntervalOfSeconds()
	{
		uint64_t current = GetMicroSeconds();

		return (current - kSavedTime) / (1000000);
	}

	uint64_t Time::GetIntervalOfMilliseconds()
	{
		uint64_t current = GetMicroSeconds();

		return (current - kSavedTime) / (1000);
	}

	uint64_t Time::GetIntervalOfMicroseconds()
	{
		uint64_t current = GetMicroSeconds();

		return (current - kSavedTime);
	}

	uint64_t Time::GetMicroSeconds()
	{
		uint64_t retval = 0;
		struct timeval tv = {0, 0};

		gettimeofday(&tv, NULL);

		retval = tv.tv_sec * 1000 * 1000 + tv.tv_usec;

		return retval;
	}

	void Time::SaveCurrent()
	{
		kSavedTime = GetMicroSeconds();
	}

}
