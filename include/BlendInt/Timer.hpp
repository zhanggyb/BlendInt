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

#include <Cpp/Events.hpp>

namespace BlendInt {

	/**
	 * @brief the timer in BlendInt
	 */
	class Timer
	{
	public:

		Timer ();

		~Timer ();

		void Start ();

		void Stop ();

		/**
		 * @brief Set Interval
		 * @param interval the interval in millisecond
		 */
		void set_interval (unsigned int interval)
		{
			m_interval = interval;
		}

		unsigned int interval () const {return m_interval;}

		Cpp::EventRef<> timeout() {return m_timeout;}

	protected:

		static void ThreadCallback (union sigval sigev_value);

	private:

		void Create ();

		timer_t m_id;

		unsigned int m_interval;

		/**
		 * @brief the time out event
		 */
		Cpp::Event<> m_timeout;

	};

}

#endif /* _BLENDINT_TIMER_HPP_ */
