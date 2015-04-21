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

#ifndef _BLENDINT_CORE_MUTEX_HPP_
#define _BLENDINT_CORE_MUTEX_HPP_

#ifdef __UNIX__
#include <pthread.h>
#endif

#include <blendint/core/types.hpp>

namespace BlendInt {

	class Mutex;

	class MutexAttrib
	{
	public:

		enum Type {
			NORMAL = PTHREAD_MUTEX_NORMAL,
			ERRORCHECK = PTHREAD_MUTEX_ERRORCHECK,
			RECURSIVE = PTHREAD_MUTEX_RECURSIVE,
			DEFAULT = PTHREAD_MUTEX_DEFAULT
        };

		inline MutexAttrib () {}

		inline ~MutexAttrib () {}

		inline bool initialize ()
		{
			return pthread_mutexattr_init(&attribute_) == 0 ? true : false;
		}

		inline bool destroy ()
		{
			return pthread_mutexattr_destroy(&attribute_) == 0 ? true : false;
		}

		inline bool set_type (int type)
		{
			return pthread_mutexattr_settype(&attribute_, type) == 0 ? true : false;
		}

        inline bool set_protocol (int protocol)
        {
            return pthread_mutexattr_setprotocol(&attribute_, protocol) == 0 ? true : false;
        }

	private:

		friend class Mutex;

		pthread_mutexattr_t attribute_;

	};

	/**
	 * @brief Thread Mutex
	 *
	 * Create static mutex by default, if dynamically, use initialize()
	 * and destroy() when no longer used.
	 */
	class Mutex
	{
	public:

		inline Mutex ()
		: mutex_(PTHREAD_MUTEX_INITIALIZER)
		{
		}

		inline ~Mutex ()
		{
		}

		inline bool initialize (const MutexAttrib& attr)
		{
#ifdef DEBUG
			if(pthread_mutex_init(&mutex_, &attr.attribute_) != 0) {
				DBG_PRINT_MSG("%s", "Fail to initialize mutex");
				return false;
			}

			return true;
#else
			return pthread_mutex_init(&mutex_, &attr.attribute_);
#endif	// DEBUG
		}

		inline bool lock ()
		{
#ifdef DEBUG
			if(pthread_mutex_lock(&mutex_) != 0) {
				DBG_PRINT_MSG("%s", "Fail to lock mutex");
				return false;
			}

			return true;
#else
			return pthread_mutex_lock(&mutex_) == 0 ? true : false;
#endif	// DEBUG
		}

		inline bool trylock ()
		{
#ifdef DEBUG
			if(pthread_mutex_trylock(&mutex_) != 0) {
				DBG_PRINT_MSG("%s", "Fail to trylock mutex");
				return false;
			}

			return true;
#else
			return pthread_mutex_trylock(&mutex_) == 0 ? true : false;
#endif	// DEBUG
		}

		inline bool unlock ()
		{
#ifdef DEBUG
			if(pthread_mutex_unlock(&mutex_) != 0) {
				DBG_PRINT_MSG("%s", "Fail to unlock mutex");
				return false;
			}

			return true;
#else
			return pthread_mutex_unlock(&mutex_) == 0 ? true : false;
#endif	// DEBUG
		}

		inline bool destroy ()
		{
#ifdef DEBUG
			if(pthread_mutex_destroy(&mutex_) != 0) {
				DBG_PRINT_MSG("%s", "Fail to destroy mutex");
				return false;
			}
			return true;
#else
			return pthread_mutex_destroy(&mutex_) == 0 ? true : false;
#endif	// DEBUG
		}

	private:

		pthread_mutex_t mutex_;

	};

}

#endif /* _BLENDINT_CORE_MUTEX_HPP_ */
