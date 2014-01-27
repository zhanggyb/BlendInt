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

#ifndef _BLENDINT_ABSTRACTCAMERA_HPP_
#define _BLENDINT_ABSTRACTCAMERA_HPP_

#include <glm/glm.hpp>

#include <BlendInt/Object.hpp>

namespace BlendInt {

	class AbstractCamera: public Object
	{
	public:

		AbstractCamera ();

		virtual ~AbstractCamera ();

		void set_position (const glm::vec3& pos)
		{
			m_position = pos;
		}

		void set_position (float x, float y, float z)
		{
			m_position.x = x;
			m_position.y = y;
			m_position.z = z;
		}

		const glm::vec3& position () const {return m_position;}

		const glm::vec3& look () const
		{
			return m_look;
		}

		void set_look (const glm::vec3& look)
		{
			m_look = look;
		}

		const glm::vec3& up () const
		{
			return m_up;
		}

		void set_up (const glm::vec3& up)
		{
			m_up = up;
		}

		const glm::vec3& center () const
		{
			return m_center;
		}

		void set_center (const glm::vec3& center)
		{
			m_center = center;
		}

		const glm::mat4& projection () const
		{
			return m_projection;
		}

		const glm::mat4& view () const
		{
			return m_view;
		}

		float fovy () const
		{
			return m_fovy;
		}

		void set_fovy (float fovy)
		{
			m_fovy = fovy;
		}

		float aspect () const
		{
			return m_aspect;
		}

		void set_aspect (float aspect)
		{
			m_aspect = aspect;
		}

		float near () const
		{
			return m_near;
		}

		void set_near (float near)
		{
			m_near = near;
		}

		float far () const
		{
			return m_far;
		}

		void set_far (float far)
		{
			m_far = far;
		}

		float yaw () const
		{
			return m_yaw;
		}

		void set_yaw (float yaw)
		{
			m_yaw = yaw;
		}

		float pitch () const
		{
			return m_pitch;
		}

		void set_pitch (float pitch)
		{
			m_pitch = pitch;
		}

		float roll () const
		{
			return m_roll;
		}

		void set_roll (float roll)
		{
			m_roll = roll;
		}

		/**
		 * @brief Update the model view projection matrix
		 */
		virtual void Update () = 0;

	protected:

		void set_projection (const glm::mat4& projection)
		{
			m_projection = projection;
		}

		void set_view (const glm::mat4& view)
		{
			m_view = view;
		}

	private:

		/** The eye position */
		glm::vec3 m_position;

		glm::vec3 m_look;

		/** Direction of the up vector */
		glm::vec3 m_up;

		/** Position of the reference point */
		glm::vec3 m_center;

		/** The view matrix */
		glm::mat4 m_view;

		/** The projection matrix */
		glm::mat4 m_projection;

		/** The field of view angle, in degrees, in the y direction */
		float m_fovy;

		/** Aspect ratio that determines the field of view in the x direction, the aspect ratio is the ratio of x (width) to y (height) */
		float m_aspect;

		/**	The distance from the viewer to the near clipping plane (always positive) */
		float m_near;

		/** The distance from the viewer to the far clipping plane (always positive) */
		float m_far;

		/** Used in glm::yawPitchRoll */
		float m_yaw;

		float m_pitch;

		float m_roll;
	};

}

#endif /* _BLENDINT_ABSTRACTCAMERA_HPP_ */
