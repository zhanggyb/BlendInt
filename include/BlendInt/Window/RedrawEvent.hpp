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
		
		void set_pv_matrix (const glm::mat4& pv)
		{
			m_pv_matrix = pv;
		}

		const glm::mat4& pv_matrix () const {return m_pv_matrix;}

	private:

		glm::mat4 m_pv_matrix;

	};

}

#endif // _BLENDINT_WINDOW_REDRAWEVENT_HPP_
