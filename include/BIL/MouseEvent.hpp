/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_MOUSEEVENT_H_
#define _BIL_MOUSEEVENT_H_

#include <BIL/Types.hpp>
#include <BIL/Tuple.hpp>
#include <BIL/Event.hpp>
#include <BIL/InputEvent.hpp>

#include <BIL/Coord.hpp>

namespace BIL {

	class InputEvent;

	class MouseEvent: public InputEvent
	{
	public:

		/**
		 * @brief Constructor
		 * @param[in] type one of Event::Type
		 */
		MouseEvent (MouseAction action, MouseButton button)
			: action_(action), button_(button),
			pos_(Coord2f(0.0, 0.0)),
			window_pos_(Coord2d(0.0, 0.0))
			{}
		
		MouseEvent (MouseAction action, MouseButton button,
					const Coord2f& local_pos,
					const Coord2d& window_pos)
			: action_(action), button_(button),
			pos_(local_pos), window_pos_(window_pos)
		{}

		virtual ~MouseEvent ()
		{}

		MouseAction action () const
		{
			return action_;
		}

		MouseButton button () const
		{
			return button_;
		}

		const Coord2f& pos () const
		{
			return pos_;
		}

		void set_pos (float x, float y)
		{
			pos_.set_x (x);
			pos_.set_y (y);
		}

		const Coord2d& window_pos () const
		{
			return window_pos_;
		}

		void set_window_pos (double x, double y)
		{
			window_pos_.coord.x = x;
			window_pos_.coord.y = y;
		}

	private:

		MouseAction action_;
		MouseButton button_;
		Coord2f pos_;
		Coord2d window_pos_;
	};

}

#endif	/* _BIL_MOUSEEVENT_H_ */
