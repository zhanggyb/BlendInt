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

#ifndef _BLENDINT_GUI_CURSORTYPE_HPP_
#define _BLENDINT_GUI_CURSORTYPE_HPP_

#include <stack>

namespace BlendInt {

 	class CursorType
 	{
 	public:

 		CursorType ();

 		virtual ~CursorType ();

		/**
		 * @brief Set the cursor
		 *
		 * Override this to set custom cursor
		 */
		virtual void SetCursor (int cursor_type);

		inline int current_cursor () const
		{
			return current_cursor_;
		}

 	private:

		int current_cursor_;

 	};

}

#endif	// _BLENDINT_GUI_CURSORTYPE_HPP_
