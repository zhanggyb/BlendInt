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

		AbstractCamera (const glm::vec3& pos, const glm::vec3& center, const glm::vec3& up);

		virtual ~AbstractCamera ();

		void LookAt (const glm::vec3& pos, const glm::vec3& center, const glm::vec3& up);

		/**
		 * @brief Set the projection data of this camera
		 * @param fovy
		 * @param aspect
		 * @param near
		 * @param far
		 */
		void SetPerspective (const float fovy, const float aspect,
		        const float near = 0.1f, const float far = 100.f);

		void Orbit (float x, float y);

		void Pan (float x, float y);

		void Zoom (float fac);

		glm::mat4 GetMatrixUsingYawPitchRoll (float yaw, float pitch, float roll);

		const glm::vec3& position () const {return m_position;}

		const glm::vec3& center () const {return m_center;}

		const glm::vec3& up () const {	return m_up;}

		const glm::vec3& n () const {return m_n;}

		const glm::vec3& u () const {return m_u;}

		const glm::vec3& v () const {return m_v;}

		const glm::mat4& projection () const	{return m_projection;}

		const glm::mat4& view () const {return m_view;}

		float fovy () const {return m_fovy;}

		float aspect () const	{return m_aspect;}

		float near () const	{return m_near;}

		float far () const {return m_far;}

		float yaw () const {return m_yaw;}

		float pitch () const {return m_pitch;}

		float roll () const {return m_roll;}

		/**
		 * @brief Update the model view projection matrix
		 */
		virtual void Update () = 0;

		virtual void Rotate (float yaw, float pitch, float roll);

	protected:

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

		void set_center (const glm::vec3& center)
		{
			m_center = center;
		}

		void set_center (float x, float y, float z)
		{
			m_center = glm::vec3(x, y, z);
		}

		void set_up (const glm::vec3& up)
		{
			m_up = up;
		}

		void set_up (float x, float y, float z)
		{
			m_up = glm::vec3(x, y, z);
		}

		void set_n (const glm::vec3& n)
		{
			m_n = n;
		}

		void set_n (float x, float y, float z)
		{
			m_n.x = x;
			m_n.y = y;
			m_n.z = z;
		}

		void set_u (const glm::vec3& u)
		{
			m_u = u;
		}

		void set_u (float x, float y, float z)
		{
			m_u.x = x;
			m_u.y = y;
			m_u.z = z;
		}

		void set_v (const glm::vec3& v)
		{
			m_v = v;
		}

		void set_v (float x, float y, float z)
		{
			m_v.x = x;
			m_v.y = y;
			m_v.z = z;
		}

		void set_projection (const glm::mat4& projection)
		{
			m_projection = projection;
		}

		void set_view (const glm::mat4& view)
		{
			m_view = view;
		}

		void set_fovy (float fovy)
		{
			m_fovy = fovy;
		}

		void set_aspect (float aspect)
		{
			m_aspect = aspect;
		}

		void set_near (float near)
		{
			m_near = near;
		}

		void set_far (float far)
		{
			m_far = far;
		}

		void set_yaw (float yaw)
		{
			m_yaw = yaw;
		}

		void set_pitch (float pitch)
		{
			m_pitch = pitch;
		}

		void set_roll (float roll)
		{
			m_roll = roll;
		}

	private:

		/** The eye position */
		glm::vec3 m_position;

		/** Position of the reference point */
		glm::vec3 m_center;

		/** Direction of the up vector */
		glm::vec3 m_up;

		/** Look direction of the camera */
		glm::vec3 m_n;

		/** Up direction of the camera */
		glm::vec3 m_v;

		/** Right direction of the camera */
		glm::vec3 m_u;

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
