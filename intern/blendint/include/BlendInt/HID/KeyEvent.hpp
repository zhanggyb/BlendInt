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

#include <BlendInt/Core/String.hpp>
#include <BlendInt/Core/Types.hpp>

#include <BlendInt/HID/HIDEvent.hpp>

namespace BlendInt {

	/**
	 * @brief Keyboard events
	 *
	 * @ingroup device_events
	 */
	class KeyEvent: public HIDEvent
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
		KeyEvent ()
		: HIDEvent(),
		  action_(KeyNone),
		  key_(Key_Unknown),
		  scancode_(0)
		{
		}

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
		KeyEvent (KeyAction action, int key, int scancode, int mods, const String& text = String())
		: HIDEvent(mods),
		  action_(action),
		  key_(key),
		  scancode_(scancode),
		  text_ (text)
		{
		}

		virtual ~KeyEvent()
		{
		}

		int key () const {return key_;}

		void set_key (int key) {key_ = key;}

		int scancode () const {return scancode_;}

		void set_scancode (int scancode) {scancode_ = scancode;}

		KeyAction action () const {return action_;}

		void set_action (KeyAction action) {action_ = action;}

		const String& text () const {return text_;}

		void set_text (const String& text) {text_ = text;}

		void set_text (unsigned int character) {
			text_.clear();
			text_.push_back(character);
		}

		void clear_text () {text_.clear();}

	private:

		KeyAction action_;

		int key_;

		int scancode_;

		String text_;
	};

}

#endif	/* _BIL_KEYEVENT_H_ */
