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

#ifndef _INCLUDE_BLENDINT_CORE_THREAD_HPP_
#define _INCLUDE_BLENDINT_CORE_THREAD_HPP_

#ifdef __UNIX__
#include <pthread.h>
#endif

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Core/Object.hpp>

namespace BlendInt {

	class AbstractRunnable: public Object
	{
	public:

		inline AbstractRunnable() {}

		virtual ~AbstractRunnable ()
		{}

		virtual void* Run () = 0;
	};

	class ThreadMutex;

	class ThreadMutexAttrib
	{
	public:
		
		enum Type {
			NORMAL = PTHREAD_MUTEX_NORMAL,
			ERRORCHECK = PTHREAD_MUTEX_ERRORCHECK,
			RECURSIVE = PTHREAD_MUTEX_RECURSIVE,
			DEFAULT = PTHREAD_MUTEX_DEFAULT	
        };
		
		inline ThreadMutexAttrib () {}
		
		inline ~ThreadMutexAttrib () {}
		
		inline bool initialize ()
		{
			return pthread_mutexattr_init(&attribute_) == 0 ? true : false;
		}
		
		inline bool destroy ()
		{
			return pthread_mutexattr_destroy(&attribute_) == 0 ? true : false;
		}
		
        inline bool set_protocol (int protocol)
        {
            return pthread_mutexattr_setprotocol(&attribute_, protocol) == 0 ? true : false;
        }
        
	private:
		
		friend class ThreadMutex;
		
		pthread_mutexattr_t attribute_;
		
	};

	/**
	 * @brief Thread Mutex
	 *
	 * Create static mutex by default, if dynamically, use initialize()
	 * and destroy() when no longer used.
	 */
	class ThreadMutex
	{
	public:
		
		inline ThreadMutex ()
		: mutex_(PTHREAD_MUTEX_INITIALIZER)
		{
		}
		
		~ThreadMutex ()
		{
		}

		inline bool initialize (const ThreadMutexAttrib& attr)
		{
			return pthread_mutex_init(&mutex_, &attr.attribute_);
		}

		inline bool lock ()
		{
			return pthread_mutex_lock(&mutex_) == 0 ? true : false;
		}
		
		inline bool trylock ()
		{
			return pthread_mutex_trylock(&mutex_) == 0 ? true : false;
		}
		
		inline bool unlock ()
		{
			return pthread_mutex_unlock(&mutex_) == 0 ? true : false;
		}
		
		inline bool destroy ()
		{
			return pthread_mutex_destroy(&mutex_) == 0 ? true : false;
		}
		
	private:
		
		pthread_mutex_t mutex_;
		
	};

	class Thread
	{
	public:

		Thread (const RefPtr<AbstractRunnable>& runnable, bool is_detached = false);

		explicit Thread (bool is_detached = false);

		virtual ~Thread();

		void Start ();

		void* Join ();

	protected:

		virtual void* Run () {return 0;}

	private:

		void SetCompleted ();

		void PrintError (const char* msg, int status, const char* filename, int line_no);

		static void* StartThreadRunnable (void* p);

		static void* StartThread (void* p);

		RefPtr<AbstractRunnable> runnable_;

		pthread_t thread_id_;	// thread ID

		bool detached_;		// true if thread created in detached state; false otherwise

		pthread_attr_t attribute_;

		void* result_;	// stores return value of run()

		DISALLOW_COPY_AND_ASSIGN(Thread);
	};

}

#endif /* _INCLUDE_BLENDINT_CORE_THREAD_HPP_ */
