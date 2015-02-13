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

#ifndef _BLENDINT_GUI_PERSPECTIVECAMERA_HPP_
#define _BLENDINT_GUI_PERSPECTIVECAMERA_HPP_

#include <glm/vec3.hpp>
#include <gui/abstract-camera.hpp>

namespace BlendInt {

	class PerspectiveCamera: public AbstractCamera
	{
	public:

		PerspectiveCamera ();

		virtual ~PerspectiveCamera ();

		/**
		 * @brief Set the projection data of this camera
		 * @param fovy
		 * @param aspect
		 * @param near
		 * @param far
		 */
		void SetPerspective (const float fovy, const float aspect,
		        const float near = 0.1f, const float far = 100.f);

		virtual void Orbit (float dx, float dy);

		virtual void Pan (float dx, float dy);

		virtual void Zoom (float fac);

		void SaveCurrentPosition ();

		void SaveCurrentCenter ();

		float fovy () const {return fovy_;}

		float aspect () const {return aspect_;}

		float near () const	{return near_;}

		float far () const {return far_;}

	protected:

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

		glm::vec3 last_position_;

		glm::vec3 last_center_;

		/** The field of view angle, in degrees, in the y direction */
		float fovy_;

		/** Aspect ratio that determines the field of view in the x direction, the aspect ratio is the ratio of x (width) to y (height) */
		float aspect_;

		/**	The distance from the viewer to the near clipping plane (always positive) */
		float near_;

		/** The distance from the viewer to the far clipping plane (always positive) */
		float far_;

		static float orbit_speed;

		static float pan_speed;

		static float zoom_speed;

	};

}

#endif /* _BLENDINT_GUI_PERSPECTIVECAMERA_HPP_ */
