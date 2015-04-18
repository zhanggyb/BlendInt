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

#include <signal.h>
#include <time.h>

#ifdef __LINUX__
#define BLENDINT_USE_POSIX_TIMER 1
#else
#define BLENDINT_USE_POSIX_TIMER 0
#include <pthread.h>
#endif

#include <cppevent/event.hpp>

#include <core/object.hpp>

namespace BlendInt {

  /**
   * @brief The timer class
   *
   * The Timer class provides timers which will fire event when time out.
   *
   * To use it, create a timer and connect the timeout() event to the appropriate event callee,
   * and call Start() to enable the timer.
   *
   * Example code for usage:
   * @code
   Timer* timer = new Timer;
   events()->connect(timer->timeout(), this, &Foo::do_sth);
   timer->SetInterval(50);	// 50 ms
   @endcode
   *
   * @ingroup blendint_core
   */
  class Timer: public Object
  {
  public:

    /**
     * @brief Default constructor
     */
    Timer ();

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
    unsigned int interval () const
    {
      return interval_;
    }

    /**
     * @brief Check if the timer is active (enabled)
     * @return true: enabled
     */
    bool enabled () const
    {
      return enabled_;
    }

    /**
     * @brief The timeout event
     * @return Reference to a CppEvent::Event
     */
    CppEvent::EventRef<Timer*> timeout ()
    {
      return timeout_;
    }

    static double GetIntervalOfSeconds ();

    static double GetIntervalOfMilliseconds ();

    static double GetIntervalOfMicroseconds ();

    static double GetProgramSeconds ();

    static double GetProgramMilliseconds ();

    static double GetProgramMicroseconds ();

    static uint64_t GetMicroSeconds ();

    static void SaveCurrentTime ();

    static void SaveProgramTime ();

    static inline uint64_t saved_time ()
    {
      return kSavedTime;
    }

    static inline uint64_t program_time ()
    {
      return kProgramTime;
    }

  protected:

#if BLENDINT_USE_POSIX_TIMER
    static void ThreadCallback (union sigval sigev_value);
#else
    static void *ThreadCallback (void* data);
#endif

    void set_interval (unsigned int interval)
    {
      interval_ = interval;
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
    unsigned int interval_;

    bool enabled_;

    /**
     * @brief the time out event
     */
    CppEvent::Event<Timer*> timeout_;

    static uint64_t kSavedTime;

    static uint64_t kProgramTime;

  };

}
