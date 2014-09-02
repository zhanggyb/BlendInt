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

#include <math.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/NavigationCamera.hpp>

#include <BlendInt/Core/Types.hpp>

namespace BlendInt {

	float NavigationCamera::orbit_speed = 25.f;

	float NavigationCamera::pan_speed = 50.f;

	float NavigationCamera::zoom_speed = 100.f;

	NavigationCamera::NavigationCamera ()
	{
		set_position(5.0, 5.0, 5.0);
		set_center(0.0, 0.0, 0.0);
		set_up(0.0, 0.0, 1.0);
	}

	NavigationCamera::~NavigationCamera ()
	{
	}

	void NavigationCamera::Orbit (float dx, float dy)
	{
		float radius = glm::distance(last_position_, last_center_);

		glm::mat4 I = glm::mat4(1);
		glm::mat4 Rh = glm::rotate(I, dx / radius / orbit_speed, up());
		glm::mat4 Rv = glm::rotate(I, -dy / radius / orbit_speed, local_x());

		glm::vec4 pos = glm::vec4(last_position_, 1.f);

		pos = Rv * Rh * pos;

		LookAt(glm::vec3(pos), last_center_, up());
	}

	void NavigationCamera::Pan (float dx, float dy)
	{
		glm::vec3 translate = local_x() * (dx / pan_speed) + local_y() * (dy / pan_speed);

		set_position(last_position_ + translate);
		set_center(last_center_ + translate);

		set_view(glm::lookAt(position(), center(), up()));
	}

	void NavigationCamera::Zoom (float dy)
	{
		glm::vec3 direct_orig = glm::normalize(last_position_ - center());
		glm::vec3 pos = last_position_ + local_z() * (dy / zoom_speed);
		glm::vec3 direct_new = glm::normalize(pos - center());

		if(direct_new != direct_orig)
			return;

		set_position(pos);
		set_view(glm::lookAt(position(), center(), up()));
	}

    void NavigationCamera::SaveCurrentPosition()
    {
        last_position_ = position();
    }

    void NavigationCamera::SaveCurrentCenter()
    {
        last_center_ = center();
    }

	void NavigationCamera::Update ()
	{
		set_view(glm::lookAt(position(), center(), up()));
	}

}

