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

#include <glm/gtc/matrix_transform.hpp>
#include <BlendInt/Gui/AbstractCamera.hpp>

#include <BlendInt/Core/Types.hpp>

namespace BlendInt {

	AbstractCamera::AbstractCamera ()
	: Object(),
	  fovy_(45.0f),
	  aspect_(1.0),
	  near_(0.1f),
	  far_(100.f)
	{
		LookAt (glm::vec3(5.f, 5.f, 5.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f));

		projection_ = glm::perspective(fovy_, aspect_, near_, far_);
	}

	AbstractCamera::AbstractCamera (const glm::vec3& pos, const glm::vec3& center, const glm::vec3& up)
	: Object(),
	  fovy_(45.0f),
	  aspect_(1.0),
	  near_(0.1f),
	  far_(100.f)
	{
		LookAt (pos, center, up);

		projection_ = glm::perspective(fovy_, aspect_, near_, far_);
	}

	AbstractCamera::~AbstractCamera ()
	{
	}

	void AbstractCamera::LookAt(const glm::vec3& pos, const glm::vec3& center, const glm::vec3& up)
	{
		if(pos == center) {
			DBG_PRINT_MSG("ERROR: %s", "position and center are the same");
			return;
		}

		position_ = pos;
		center_ = center;
		up_ = up;

		local_z_ = position_ - center_;
		local_x_ = glm::cross(up_, local_z_);
		local_y_ = glm::cross(local_z_, local_x_);

		local_x_ = glm::normalize(local_x_);
		local_y_ = glm::normalize(local_y_);
		local_z_ = glm::normalize(local_z_);

		view_ = glm::lookAt(position_, center_, up_);
	}

	void AbstractCamera::SetPerspective (const float fovy, const float aspect,
	        const float near, const float far)
	{
		near_ = near;
		far_ = far;
		fovy_ = fovy;
		aspect_ = aspect;

		projection_ = glm::perspective(fovy_, aspect_, near_, far_);
	}

}

