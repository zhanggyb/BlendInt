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

#ifndef _BLENDINT_OPENGL_SCISSORSTATUS_HPP_
#define _BLENDINT_OPENGL_SCISSORSTATUS_HPP_

#include <BlendInt/Core/Rect.hpp>

#include <stack>

namespace BlendInt {

	class ScissorStatus
	{
	public:

		ScissorStatus();

		~ScissorStatus ();

		void Push (const Rect& area);

		void Pop ();

		void Push (int x, int y, int w, int h);

		void Enable ();

		void Disable ();

		bool valid ()
		{
			return m_area_stack.size() > 0;
		}

	private:

		std::stack<Rect> m_area_stack;

	};

}

#endif /* _BLENDINT_OPENGL_SCISSORSTATUS_HPP_ */
