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

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Core/Point.hpp>

#include <BlendInt/HID/HIDEvent.hpp>

namespace BlendInt {

	class HIDEvent;

	class Frame;

	/**
	 * @brief Mouse event class
	 *
	 * @ingroup device_events
	 */
	class MouseEvent: public HIDEvent
	{
	public:

		/**
		 * @brief Default constructor
		 */
		MouseEvent ()
		: HIDEvent(),
		  action_(MouseNone),
		  button_(MouseButtonNone)
		{}

		/**
		 * @brief Constructor
		 * @param[in] type one of Event::Type
		 */
		MouseEvent (MouseAction action, MouseButton button)
		: HIDEvent(), action_(action), button_(button)
		{}
		
		virtual ~MouseEvent ()
		{}

		MouseAction action () const
		{
			return action_;
		}

		void set_action (MouseAction action) {action_ = action;}

		MouseButton button () const
		{
			return button_;
		}

		void set_button (MouseButton button) {button_ = button;}

	private:

		MouseAction action_;

		MouseButton button_;
	};

}

#endif	/* _BIL_MOUSEEVENT_H_ */
