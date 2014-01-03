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

#ifndef _BLENDINT_TIMER_HPP_
#define _BLENDINT_TIMER_HPP_

#include <signal.h>
#include <time.h>

#ifdef __LINUX__
#define BLENDINT_USE_POSIX_TIMER 1
#else
#define BLENDINT_USE_POSIX_TIMER 0
#include <pthread.h>
#endif

#include <Cpp/Events.hpp>

#include <BlendInt/Object.hpp>

namespace BlendInt {

	/**
	 * @brief The timer class
	 *
	 * The Timer class provides timers which will fire event when time out.
	 *
	 * To use it, create a timer and connect the timeout() event to the appropriate event callee,
	 * and call Start() to enable the timer.
	 *
	 * For example:
	 * @code
	 	 Timer* timer = new Timer;
	 	 events()->connect(timer->timeout(), this, &Foo::do_sth);
	 	 timer->SetInterval(50);	// 50 ms
	  @endcode
	 *
	 * @ingroup animation
	 */
	class Timer: public Object
	{
	public:

		/**
		 * @brief Default constructor
		 */
		Timer ();

		Timer (Object *super);

		/**
		 * @brief Destructor
		 */
		~Timer ();

		/**
		 * @brief Start the timer
		 */
		void Start ();

		/**
		 * @brief Stop the timer
		 */
		void Stop ();

		/**
		 * @brief Reset the Interval
		 * @param interval the interval in millisecond
		 *
		 * Reset the interval time of the timer, if it's already started, the timer will
		 * continue to running with new interval
		 */
		void SetInterval (unsigned int interval);

		/**
		 * @brief Get the interval time
		 * @return
		 */
		unsigned int interval () const {return m_interval;}

		/**
		 * @brief Check if the timer is active (enabled)
		 * @return true: enabled
		 */
		bool enabled () const {return m_enabled;}

		/**
		 * @brief The timeout event
		 * @return Reference to a Cpp::Event
		 */
		Cpp::EventRef<> timeout() {return m_timeout;}

	protected:

#if BLENDINT_USE_POSIX_TIMER
		static void ThreadCallback (union sigval sigev_value);
#else
		static void *ThreadCallback (void* data);
#endif

		void set_interval (unsigned int interval)
		{
			m_interval = interval;
		}

	private:

		void Create ();

#if BLENDINT_USE_POSIX_TIMER
		timer_t m_id;
#else
		pthread_t m_id;	// thread id
#endif	// __APPLE__

		/**
		 * @brief the interval time in millisecond
		 *
		 * The default is 40ms: 25fps
		 */
		unsigned int m_interval;

		bool m_enabled;

		/**
		 * @brief the time out event
		 */
		Cpp::Event<> m_timeout;

	};

}

#endif /* _BLENDINT_TIMER_HPP_ */
