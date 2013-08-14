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

#ifndef _BIL_KEYEVENT_H_
#define _BIL_KEYEVENT_H_

#include <BIL/Types.h>
#include <BIL/InputEvent.h>

namespace BIL {

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

	private:

		int _key;
		int _scancode;
		int _action;
	};

}

#endif	/* _BIL_KEYEVENT_H_ */
