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

#include <BlendInt/AbstractCamera.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace BlendInt {

	AbstractCamera::AbstractCamera ()
	: Object(),
	  m_fovy(0.0),
	  m_aspect(1.0),
	  m_near(0.1f),
	  m_far(100.f),
	  m_yaw(0.0),
	  m_pitch(0.0),
	  m_roll(0.0)
	{
	}

	AbstractCamera::~AbstractCamera ()
	{
	}

	void AbstractCamera::SetProjection (const float fovy, const float aspect,
	        const float near, const float far)
	{
		m_near = near;
		m_far = far;
		m_fovy = fovy;
		m_aspect = aspect;
		m_projection = glm::perspective(m_near, m_aspect, m_near, m_far);
	}

	void AbstractCamera::Rotate(float yaw, float pitch, float roll)
	{
		m_yaw = glm::radians(yaw);
		m_pitch = glm::radians(pitch);
		m_roll = glm::radians(roll);

		Update ();
	}

}

