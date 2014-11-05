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

#ifndef _BLENDINT_STOCK_CURSOR_HPP_
#define _BLENDINT_STOCK_CURSOR_HPP_

#include <stack>

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Gui/CursorType.hpp>

namespace BlendInt {

	class Context;

	class Cursor
	{
		DISALLOW_COPY_AND_ASSIGN(Cursor);

	public:

		static Cursor* instance;

		void RegisterCursorType (CursorType* cursor_type);

		void SetCursor (int cursor_type);

		void PushCursor ();

		void PushCursor (int cursor_type);

		void PopCursor ();

	private:

		friend class Context;

		static bool Initialize ();

		static void Release ();

		Cursor ();

		~Cursor ();

		CursorType * cursor_type_;

		std::stack<int> cursor_stack_;

	};

}

#endif	// _BLENDINT_STOCK_CURSOR_HPP_
