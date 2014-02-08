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
	  m_fovy(45.0f),
	  m_aspect(1.0),
	  m_near(0.1f),
	  m_far(100.f),
	  m_yaw(0.0),
	  m_pitch(0.0),
	  m_roll(0.0)
	{
		LookAt (glm::vec3(5.f, 5.f, 5.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f));

		m_projection = glm::perspective(m_fovy, m_aspect, m_near, m_far);
	}

	AbstractCamera::AbstractCamera (const glm::vec3& pos, const glm::vec3& center, const glm::vec3& up)
	: Object(),
	  m_fovy(45.0f),
	  m_aspect(1.0),
	  m_near(0.1f),
	  m_far(100.f),
	  m_yaw(0.0),
	  m_pitch(0.0),
	  m_roll(0.0)
	{
		LookAt (pos, center, up);

		m_projection = glm::perspective(m_fovy, m_aspect, m_near, m_far);
	}

	AbstractCamera::~AbstractCamera ()
	{
	}

	void AbstractCamera::LookAt(const glm::vec3& pos, const glm::vec3& center, const glm::vec3& up)
	{
		m_position = pos;
		m_center = center;
		m_up = up;

		m_n = m_center - m_position;
		m_u = glm::cross(m_n, m_up);
		m_v = glm::cross(m_u, m_n);

		m_n = glm::normalize(m_n);
		m_u = glm::normalize(m_u);
		m_v = glm::normalize(m_v);

		m_view = glm::lookAt(m_position, m_center, m_up);
	}

	void AbstractCamera::SetPerspective (const float fovy, const float aspect,
	        const float near, const float far)
	{
		m_near = near;
		m_far = far;
		m_fovy = fovy;
		m_aspect = aspect;

		m_projection = glm::perspective(m_fovy, m_aspect, m_near, m_far);
	}

	void AbstractCamera::Orbit (float x, float y)
	{
	}

	void AbstractCamera::Pan (float x, float y)
	{
	}

	void AbstractCamera::Zoom (float fac)
	{
		m_position = m_position + m_n * (fac / 500.f);

		Update ();
	}

	void AbstractCamera::Rotate(float yaw, float pitch, float roll)
	{
		m_yaw = glm::radians(yaw);
		m_pitch = glm::radians(pitch);
		m_roll = glm::radians(roll);

		Update ();
	}

	glm::mat4 AbstractCamera::GetMatrixUsingYawPitchRoll(float yaw, float pitch, float roll)
	{
		glm::mat4 R = glm::mat4(1);

		R = glm::rotate(R, roll, glm::vec3(0, 0, 1));
		R = glm::rotate(R, yaw, glm::vec3(0, 1, 0));
		R = glm::rotate(R, pitch, glm::vec3(1, 0, 0));

		return R;
	}

}

