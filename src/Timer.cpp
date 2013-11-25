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

#include <iostream>

#include <BlendInt/Timer.hpp>

namespace BlendInt {

	Timer::Timer()
	: m_id (0), m_interval(50)
	{
		Create();
	}

	Timer::~Timer()
	{
		timer_delete(m_id);
	}

	void Timer::Start ()
	{
		int ret = -1;
		struct itimerspec ts;

		// debug time
		ts.it_interval.tv_sec = 1;
		ts.it_interval.tv_nsec = 0;
		ts.it_value.tv_sec = 1;
		ts.it_value.tv_nsec = 0;

		ret = timer_settime(m_id, 0, &ts, 0);

		if(ret < 0) {
			std::cout << "Fail to start timer" << std::endl;
		}
	}

	void Timer::Stop ()
	{
		int ret = -1;
		struct itimerspec ts;

		// debug time
		ts.it_interval.tv_sec = 1;
		ts.it_interval.tv_nsec = 0;
		ts.it_value.tv_sec = 0;
		ts.it_value.tv_nsec = 0;

		ret = timer_settime(m_id, 0, &ts, 0);

		if(ret < 0) {
			std::cout << "Fail to start timer" << std::endl;
		}
	}

	void Timer::Create()
	{
		int ret = -1;
		struct sigevent sev;
		sev.sigev_notify = SIGEV_THREAD;
		sev.sigev_value.sival_ptr = this;
		sev.sigev_notify_function = ThreadCallback;
		sev.sigev_notify_attributes = 0;

		ret = timer_create(CLOCK_REALTIME, &sev, &m_id);
		if(ret < 0) {
			// do error
			std::cout << "Fail to create timer" << std::endl;
		}
	}

	void Timer::ThreadCallback(union sigval sigev_value)
	{
#ifdef DEBUG
		std::cout << "Timer callback in thread" << std::endl;
#endif
		Timer* timer = static_cast<Timer*>(sigev_value.sival_ptr);
		timer->m_timeout.fire();
	}

}
