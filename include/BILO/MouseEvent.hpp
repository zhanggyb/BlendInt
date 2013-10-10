/*
 * This file is part of BILO (Blender-like Interface Library in
 * OpenGL).
 *
 * BILO (Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BILO (Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BILO.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BILO_MOUSEEVENT_HPP_
#define _BILO_MOUSEEVENT_HPP_

#include <BILO/Types.hpp>
#include <BILO/Event.hpp>
#include <BILO/InputEvent.hpp>

#include <BILO/Coord.hpp>

namespace BILO {

	class InputEvent;

	class MouseEvent: public InputEvent
	{
	public:

		/**
		 * @brief Constructor
		 * @param[in] type one of Event::Type
		 */
		MouseEvent (MouseAction action, MouseButton button)
			: m_action(action), m_button(button),
			m_position(Coord2d(0.0, 0.0))
			{}
		
		MouseEvent (MouseAction action, MouseButton button,
					const Coord2d& position)
			: m_action(action), m_button(button),
			m_position(position)
		{}

		virtual ~MouseEvent ()
		{}

		MouseAction action () const
		{
			return m_action;
		}

		MouseButton button () const
		{
			return m_button;
		}

		const Coord2d& position () const
		{
			return m_position;
		}

		void set_position (float x, float y)
		{
			m_position.set_x (x);
			m_position.set_y (y);
		}

	private:

		MouseAction m_action;
		MouseButton m_button;
		Coord2d m_position;
	};

}

#endif	/* _BIL_MOUSEEVENT_H_ */
