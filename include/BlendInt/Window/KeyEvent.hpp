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

#include <BlendInt/Types.hpp>
#include <BlendInt/Window/InputEvent.hpp>


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
		KeyEvent ()
		: InputEvent(),
		  m_action(KeyNone),
		  m_key(Key_Unknown),
		  m_scancode(0)
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
		: InputEvent(mods),
		  m_action(action),
		  m_key(key),
		  m_scancode(scancode),
		  m_text (text)
		{
		}

		virtual ~KeyEvent()
		{
		}

		int key () const {return m_key;}

		void set_key (int key) {m_key = key;}

		int scancode () const {return m_scancode;}

		void set_scancode (int scancode) {m_scancode = scancode;}

		KeyAction action () const {return m_action;}

		void set_action (KeyAction action) {m_action = action;}

		const String& text () const {return m_text;}

		void set_text (const String& text) {m_text = text;}

		void set_text (unsigned int character) {
			m_text.clear();
			m_text.push_back(character);
		}

		void clear_text () {m_text.clear();}

	private:

		KeyAction m_action;
		int m_key;
		int m_scancode;

		String m_text;
	};

}

#endif	/* _BIL_KEYEVENT_H_ */
