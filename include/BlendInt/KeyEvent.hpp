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

#ifndef _BLENDINT_KEYEVENT_HPP_
#define _BLENDINT_KEYEVENT_HPP_

#include <BlendInt/Types.hpp>
#include <BlendInt/InputEvent.hpp>

namespace BlendInt {

	/**
	 * @brief Keyboard events
	 *
	 * @ingroup device_events
	 */
	class KeyEvent: public InputEvent
	{
	public:

		/**
		 * @brief Default Constructor of KeyEvent
		 *
		 * @param[in] key The keyboard key that was pressed or released
		 * @param[in] scancode The system-specific scancode of the key
		 * @param[in] action KeyButtonAction
		 * @param[in] mods Bit field describing which modifier keys were held down
		 *
		 * This signature a key event
		 */
		KeyEvent(int key, int scancode,
				 int action, int mods)
			: InputEvent (mods), _key(key),
			_scancode(scancode), _action(action)
		{ }

		virtual ~KeyEvent()
		{

		}

		int key () const {return _key;}

		int scancode () const {return _scancode;}

		int action () const {return _action;}

	private:

		int _key;
		int _scancode;
		int _action;
	};

}

#endif	/* _BIL_KEYEVENT_H_ */
