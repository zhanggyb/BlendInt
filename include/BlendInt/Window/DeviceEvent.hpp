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

#ifndef _BLENDINT_DEVICEEVENT_HPP_
#define _BLENDINT_DEVICEEVENT_HPP_

/**
 * @defgroup device_events Device events from window system
 *
 * Device events come from the window system, usually are triggered by
 * a input device like keyboard or mouse.
 *
 * BlendInt does not provide a mechanism to generate device events
 * inside, instead it only receive them from the native window
 * system.
 *
 * @note A device event is different from the event fired in certain
 * widget object, see the details of event/delegate in @ref cpp_events
 *
 * @ingroup window
 */

namespace BlendInt {

	class AbstractWidget;

	/**
	 * @brief the basic event class
	 *
	 * @ingroup device_events
	 */
	class DeviceEvent
	{
	public:

		DeviceEvent ()
		{
		}

		~DeviceEvent ()
		{}

	};

} /* namespace BlendInt */

#endif	/* _BLENDINT_DEVICEEVENT_HPP_ */
