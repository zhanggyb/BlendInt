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

#include <BlendInt/Core/Object.hpp>

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

		const glm::vec3& position () const {return position_;}

		const glm::vec3& center () const {return center_;}

		const glm::vec3& up () const {	return up_;}

		const glm::vec3& local_x () const {return local_x_;}

		const glm::vec3& local_y () const {return local_y_;}

        const glm::vec3& local_z () const {return local_z_;}
        
		const glm::mat4& projection () const	{return projection_;}

		const glm::mat4& view () const {return view_;}

		float fovy () const {return fovy_;}

		float aspect () const	{return aspect_;}

		float near () const	{return near_;}

		float far () const {return far_;}

		/**
		 * @brief Update the model view projection matrix
		 */
		virtual void Update () = 0;

	protected:

		void set_position (const glm::vec3& pos)
		{
			position_ = pos;
		}

		void set_position (float x, float y, float z)
		{
			position_.x = x;
			position_.y = y;
			position_.z = z;
		}

		void set_center (const glm::vec3& center)
		{
			center_ = center;
		}

		void set_center (float x, float y, float z)
		{
			center_ = glm::vec3(x, y, z);
		}

		void set_up (const glm::vec3& up)
		{
			up_ = up;
		}

		void set_up (float x, float y, float z)
		{
			up_ = glm::vec3(x, y, z);
		}

		void set_local_x (const glm::vec3& x)
		{
			local_x_ = x;
		}

		void set_local_x (float x, float y, float z)
		{
			local_x_.x = x;
			local_x_.y = y;
			local_x_.z = z;
		}

		void set_local_y (const glm::vec3& y)
		{
			local_y_ = y;
		}

		void set_local_y (float x, float y, float z)
		{
			local_y_.x = x;
			local_y_.y = y;
			local_y_.z = z;
		}

        void set_local_z (const glm::vec3& z)
		{
			local_z_ = z;
		}
        
		void set_local_z (float x, float y, float z)
		{
			local_z_.x = x;
			local_z_.y = y;
			local_z_.z = z;
		}

		void set_projection (const glm::mat4& projection)
		{
			projection_ = projection;
		}

		void set_view (const glm::mat4& view)
		{
			view_ = view;
		}

		void set_fovy (float fovy)
		{
			fovy_ = fovy;
		}

		void set_aspect (float aspect)
		{
			aspect_ = aspect;
		}

		void set_near (float near)
		{
			near_ = near;
		}

		void set_far (float far)
		{
			far_ = far;
		}

	private:

		/** The eye position */
		glm::vec3 position_;

		/** Position of the reference point */
		glm::vec3 center_;

		/** Direction of the up vector */
		glm::vec3 up_;

		/** Right direction of the camera */
		glm::vec3 local_x_;

		/** Up direction of the camera */
		glm::vec3 local_y_;
        
        /** Look direction of the camera */
		glm::vec3 local_z_;
        
		/** The view matrix */
		glm::mat4 view_;

		/** The projection matrix */
		glm::mat4 projection_;

		/** The field of view angle, in degrees, in the y direction */
		float fovy_;

		/** Aspect ratio that determines the field of view in the x direction, the aspect ratio is the ratio of x (width) to y (height) */
		float aspect_;

		/**	The distance from the viewer to the near clipping plane (always positive) */
		float near_;

		/** The distance from the viewer to the far clipping plane (always positive) */
		float far_;

	};

}

#endif /* _BLENDINT_ABSTRACTCAMERA_HPP_ */
