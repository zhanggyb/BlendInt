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

#ifndef _BLENDINT_WINDOW_REDRAWEVENT_HPP_
#define _BLENDINT_WINDOW_REDRAWEVENT_HPP_

#include <glm/glm.hpp>

#include <BlendInt/Window/DeviceEvent.hpp>

namespace BlendInt {

	class RedrawEvent: public DeviceEvent
	{
	public:

		RedrawEvent ()
			: DeviceEvent()
		{}

		~RedrawEvent ()
		{}

		void set_cursor_position (const Point& cursor_position)
		{
			m_cursor_position = cursor_position;
		}
		
		void set_projection_matrix (const glm::mat4& matrix)
		{
			m_projection_matrix = matrix;
		}

		void set_view_matrix (const glm::mat4& matrix)
		{
			m_view_matrix = matrix;
		}

		const Point& cursor_position() const {return m_cursor_position;}

		const glm::mat4& view_matrix () const {return m_view_matrix;}

		const glm::mat4& projection_matrix () const {return m_projection_matrix;}

	private:

		Point m_cursor_position;

		glm::mat4 m_projection_matrix;
		glm::mat4 m_view_matrix;

	};

}

#endif // _BLENDINT_WINDOW_REDRAWEVENT_HPP_
