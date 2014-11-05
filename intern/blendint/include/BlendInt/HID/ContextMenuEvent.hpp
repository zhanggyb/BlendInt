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

#ifndef _BLENDINT_CONTEXTMENUEVENT_HPP_
#define _BLENDINT_CONTEXTMENUEVENT_HPP_

#include <BlendInt/HID/HIDEvent.hpp>

namespace BlendInt {

	/**
	 * @brief Context Menu event (mostly the right-click)
	 *
	 * @ingroup device_events
	 */
	class ContextMenuEvent: public HIDEvent
	{
	public:

		enum Source {
			Unknown,
			Keyboard,
			Mouse
		};

		ContextMenuEvent (Source source,
						  int mods)
			: HIDEvent(mods), source_(source)
		{}

		ContextMenuEvent (Source source,
						  int mods,
						  const Point& local_pos,
						  const Point& window_pos)
			: HIDEvent(mods), source_(source),
			pos_(local_pos), window_pos_(window_pos)
		{}

		virtual ~ContextMenuEvent()
		{}

		Source source (void)
		{
			return source_;
		}

		const Point& pos (void) const
		{
			return pos_;
		}

		void set_pos (int x, int y)
		{
			pos_.set_x (x);
			pos_.set_y (y);
		}

		const Point& window_pos (void) const
		{
			return window_pos_;
		}

		void set_window_pos (int x, int y)
		{
			window_pos_.set_x(x);
			window_pos_.set_y(y);
		}

	private:

		Source source_;

		Point pos_;

		Point window_pos_;
	};

}

#endif	/* _BIL_CONTEXTMENUEVENT_H_ */
