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

#include <errno.h>
#include <string.h>

#include <iostream>

#include <blendint/core/timer.hpp>

namespace BlendInt {

	uint64_t Timer::kSavedTime = 0;

	uint64_t Timer::kProgramTime = 0;

	Timer::Timer()
	: Object(),
#if BLENDINT_USE_POSIX_TIMER
			m_id (0),
#endif
			interval_(40),
			enabled_(false)
	{
		Create();
	}

	Timer::~Timer()
	{
#if BLENDINT_USE_POSIX_TIMER
		timer_delete(m_id);
#else
		Stop();
#endif
	}

	void Timer::Start ()
	{
		int ret = -1;

#if BLENDINT_USE_POSIX_TIMER
		struct itimerspec ts;

		unsigned int sec = interval_ / 1000;
		long nsec = (interval_ % 1000) * 1000 * 1000;

		ts.it_interval.tv_sec = sec;
		ts.it_interval.tv_nsec = nsec;
		ts.it_value.tv_sec = sec;
		ts.it_value.tv_nsec = nsec;

		ret = timer_settime(m_id, 0, &ts, 0);

		if(ret < 0) {
			std::cerr << "Fail to start timer in " << __func__ << std::endl;
			enabled_ = false;
		} else {
			enabled_ = true;
		}
#else
		if(enabled_) Stop();
		
		// Create a thread		
		ret = pthread_create (&m_id, NULL, Timer::ThreadCallback, this);
		if(ret == 0) {
			enabled_ = true;
		} else {
			std::cerr << "Fail to start timer in " << __func__ << std::endl;
			enabled_ = false;
		}
#endif

	}

	void Timer::Stop ()
	{
#if BLENDINT_USE_POSIX_TIMER
		if(! enabled_) return;

		int ret = -1;
		struct itimerspec ts;
		memset (&ts, 0, sizeof(ts));

		ret = timer_settime(m_id, 0, &ts, 0);

		if(ret < 0) {
			std::cerr << "Fail to stop timer in " << __func__ << std::endl;
		}

		enabled_ = false;
#else
		if(enabled_) {
			int ret = pthread_cancel(m_id);
			if(ret != 0) {
				std::cerr << "Error: Fail to cancel the timer thread!" << std::endl;
			}
			
			enabled_ = false;
		}
#endif
	}

	void Timer::SetInterval(unsigned int interval)
	{
		if(interval_ == interval) return;

		interval_ = interval;

#if BLENDINT_USE_POSIX_TIMER
		if(enabled_) Start ();
#else
		if(enabled_) {
			Stop();
			Start();
		}
#endif
	}

	double Timer::GetIntervalOfSeconds()
	{
		uint64_t current = GetMicroSeconds();

		return (current - kSavedTime) / (1000000.0);
	}

	double Timer::GetIntervalOfMilliseconds()
	{
		uint64_t current = GetMicroSeconds();

		return (current - kSavedTime) / (1000.0);
	}

	double Timer::GetIntervalOfMicroseconds()
	{
		uint64_t current = GetMicroSeconds();

		return (double)(current - kSavedTime);
	}

	double Timer::GetProgramSeconds()
	{
		uint64_t current = GetMicroSeconds();

		return (current - kProgramTime) / (1000000.0);
	}

	double Timer::GetProgramMilliseconds()
	{
		uint64_t current = GetMicroSeconds();

		return (current - kProgramTime) / (1000.0);
	}

	double Timer::GetProgramMicroseconds()
	{
		uint64_t current = GetMicroSeconds();

		return (double)(current - kProgramTime);
	}

	uint64_t Timer::GetMicroSeconds()
	{
		uint64_t retval = 0;
		struct timeval tv = {0, 0};

		gettimeofday(&tv, NULL);

		retval = tv.tv_sec * 1000 * 1000 + tv.tv_usec;

		return retval;
	}

	void Timer::SaveCurrentTime()
	{
		kSavedTime = GetMicroSeconds();
	}

	void Timer::SaveProgramTime ()
	{
		kProgramTime = GetMicroSeconds();
	}

	void Timer::Create()
	{
#if BLENDINT_USE_POSIX_TIMER
		int ret = -1;
		struct sigevent sev;
		sev.sigev_notify = SIGEV_THREAD;
		sev.sigev_value.sival_ptr = this;
		sev.sigev_notify_function = ThreadCallback;
		sev.sigev_notify_attributes = 0;

		ret = timer_create(CLOCK_REALTIME, &sev, &m_id);
		if(ret < 0) {
			// do error
			std::cerr << "Fail to create timer in " << __func__ << std::endl;
			if(ret == EAGAIN) {
				std::cerr << "The calling process has already created all of the timers it is allowed by this implementation" << std::endl;
			}
		}
#endif
	}

#if BLENDINT_USE_POSIX_TIMER

	void Timer::ThreadCallback(union sigval sigev_value)
	{
		Timer* timer = static_cast<Timer*>(sigev_value.sival_ptr);
		timer->timeout_.Invoke(timer);
	}

#else

	void *Timer::ThreadCallback (void* data)
	{
		Timer* timer = static_cast<Timer*>(data);
		struct timespec ts;
		// int ret = 0;

		pthread_detach(pthread_self());

		unsigned int sec = timer->interval_ / 1000;
		long nsec = (timer->interval_ % 1000) * 1000 * 1000;

		ts.tv_sec = sec;
		ts.tv_nsec = nsec;

		while(true) {
			// TODO: call nanosleep()
			//ret = nanosleep (&ts, NULL);
			nanosleep (&ts, NULL);

			if(timer->enabled()) {
				timer->timeout_.Invoke(timer);
			} else {
				break;
			}
		}

		pthread_exit(NULL);
	}

#endif	// __APPLE__

}
