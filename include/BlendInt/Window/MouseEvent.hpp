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

#ifndef _BLENDINT_MOUSEEVENT_HPP_
#define _BLENDINT_MOUSEEVENT_HPP_

#include <BlendInt/Types.hpp>

#include <BlendInt/Window/InputEvent.hpp>

#include <BlendInt/Core/Point.hpp>

namespace BlendInt {

	class InputEvent;

	/**
	 * @brief Mouse event class
	 *
	 * @ingroup device_events
	 */
	class MouseEvent: public InputEvent
	{
	public:

		/**
		 * @brief Default constructor
		 */
		MouseEvent ()
			: InputEvent(),
				m_action(MouseNone),
			  m_button(MouseButtonNone)
			{}

		/**
		 * @brief Constructor
		 * @param[in] type one of Event::Type
		 */
		MouseEvent (MouseAction action, MouseButton button)
			: InputEvent(), m_action(action), m_button(button)
			{}
		
		MouseEvent (MouseAction action, MouseButton button,
					const Point& position)
			: InputEvent(), m_action(action), m_button(button),
			m_position(position)
		{}

		virtual ~MouseEvent ()
		{}

		MouseAction action () const
		{
			return m_action;
		}

		void set_action (MouseAction action) {m_action = action;}

		MouseButton button () const
		{
			return m_button;
		}

		void set_button (MouseButton button) {m_button = button;}

		const Point& position () const
		{
			return m_position;
		}

		void set_position (const Point& pos)
		{
			m_position = pos;
		}

		void set_position (int x, int y)
		{
			m_position.set_x (x);
			m_position.set_y (y);
		}

	private:

		MouseAction m_action;
		MouseButton m_button;
		Point m_position;
	};

}

#endif	/* _BIL_MOUSEEVENT_H_ */
