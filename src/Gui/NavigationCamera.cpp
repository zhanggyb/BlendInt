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

namespace BlendInt {

	NavigationCamera::NavigationCamera ()
	: m_speed(200.f)
	{
		set_position(5.0, 5.0, 5.0);
		set_center(0.0, 0.0, 0.0);
		set_up(0.0, 0.0, 1.0);

		m_last_position = glm::vec3(0);
		m_last_center = glm::vec3(0);
	}

	NavigationCamera::~NavigationCamera ()
	{
	}

	void NavigationCamera::Orbit (float dx, float dy)
	{
		float radius = glm::distance(m_last_position, m_last_center);

		glm::mat4 I = glm::mat4(1);
		glm::mat4 T1 = glm::translate(I, m_last_center * (-1.f));
		glm::mat4 T2 = glm::translate(I, m_last_center);
		glm::mat4 Rh = glm::rotate(I, dx / radius / (m_speed / 2), up());
		glm::mat4 Rv = glm::rotate(I, -dy / radius / (m_speed / 2), local_x());

		glm::vec4 pos = glm::vec4(m_last_position, 1.0);
		pos = T2 * Rv * Rh * T1 * pos;

		LookAt(glm::vec3(pos), m_last_center, up());
	}

	void NavigationCamera::Pan (float dx, float dy)
	{
		glm::vec3 translate = local_x() * (dx / m_speed) + local_y() * (dy / m_speed);

		set_position(m_last_position + translate);
		set_center(m_last_center + translate);

		set_view(glm::lookAt(position(), center(), up()));
	}

	void NavigationCamera::Zoom (float dy)
	{
		glm::vec3 direct_orig = glm::normalize(m_last_position - center());
		glm::vec3 pos = m_last_position + local_z() * (dy / m_speed / 2);
		glm::vec3 direct_new = glm::normalize(pos - center());

		if(direct_new != direct_orig)
			return;

		set_position(pos);
		set_view(glm::lookAt(position(), center(), up()));
	}

    void NavigationCamera::SaveCurrentPosition()
    {
        m_last_position = position();
    }

    void NavigationCamera::SaveCurrentCenter()
    {
        m_last_center = center();
    }

	void NavigationCamera::Update ()
	{
		set_view(glm::lookAt(position(), center(), up()));
	}

}

