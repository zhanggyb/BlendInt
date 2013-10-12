/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_INPUTEVENT_HPP_
#define _BLENDINT_INPUTEVENT_HPP_

#include <BlendInt/Types.hpp>
#include <BlendInt/Event.hpp>

namespace BlendInt {

	class InputEvent: public Event
	{
	public:
		
		/**
		 * @brief Default Constructor
		 * @param modifiers Bitwise conbination of KeyModifier
		 */
		InputEvent(int modifiers = ModifierNone)
			: modifiers_(modifiers)
		{
		}

		virtual ~InputEvent()
		{
		}

	protected:

		int modifiers_;

	};

} /* namespace BlendInt */

#endif	/* _BIL_INPUTEVENT_H_ */
