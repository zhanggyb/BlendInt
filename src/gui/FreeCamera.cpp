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

#include <BlendInt/FreeCamera.hpp>

namespace BlendInt {

	FreeCamera::FreeCamera ()
	{
		set_position(5.0, 5.0, 5.0);
		set_center(0.0, 0.0, 0.0);
		set_up(0.0, 0.0, 1.0);

        m_last_position = glm::vec3(0);
		m_last_center = glm::vec3(0);
	}

	FreeCamera::~FreeCamera ()
	{
	}

	void FreeCamera::Orbit (float dx, float dy)
	{
		float radius = glm::distance(m_last_position, m_last_center);

		glm::mat4 m = glm::mat4(1);

		glm::mat4 T = glm::translate(m, m_last_center);
		glm::mat4 R = glm::rotate(T, dx / radius, up());

		glm::vec4 pos = R * glm::vec4(m_last_position, 1.0);

		R = glm::rotate(R, -dy / radius, local_x());

		pos = R * pos;

		LookAt(glm::vec3(pos), m_last_center, up());

		/*
		glm::vec3 pos = m_last_position;

		float alpha = atan2(m_last_position.y - center().y, m_last_position.x - center().x);
		float beta = asin((m_last_position.z - center().z)/radius);

		float theta = alpha + dx/radius;
		float gamma = beta + dy/radius;

		pos.x = radius * cos(theta) + center().x;
		pos.y = radius * sin(theta) + center().y;
		pos.z = radius * sin(gamma) + center().z;

		LookAt(pos, center(), up());
		*/
	}

	void FreeCamera::Pan (float x, float y)
	{
//		glm::vec3 pos = position() + local_x() * (x / 200.f);
//		pos = pos + local_y() * (y / 200.f);
//
//		glm::vec3 cent = center() + local_x() * (x / 200.f);
//		cent = cent + local_y() * (y / 200.f);
//
//		set_view(glm::lookAt(pos, cent, up()));

		glm::vec3 translate = local_x() * (x / 200.f) + local_y() * (y / 200.f);

		set_position(m_last_position + translate);

		set_center(m_last_center + translate);

		//m_position = pos;
		//m_center = cent;

		set_view(glm::lookAt(position(), center(), up()));

		//LookAt(pos, cent, up());
	}

	void FreeCamera::Zoom (float fac)
	{
		//m_position = m_position + m_n * (fac / 500.f);

		set_position(m_last_position + local_z() * (fac / 200.f));

		set_view(glm::lookAt(position(), center(), up()));

		//Update ();
	}

    void FreeCamera::SaveCurrentPosition()
    {
        m_last_position = position();
    }
    
    void FreeCamera::SaveCurrentCenter()
    {
        m_last_center = center();
    }
    
	void FreeCamera::Update ()
	{
		set_view(glm::lookAt(position(), center(), up()));

		/*
		glm::mat4 R = glm::yawPitchRoll(yaw(), pitch(), roll());

		// set_position(position() + m_translation);
		// m_translation = glm::vec3(0);

		set_n(glm::vec3(R * glm::vec4(0, 0, 1, 0)));
		//glm::vec3 center = position() + look();
		//set_center(position() + look());
		set_v(glm::vec3(R * glm::vec4(0, 1, 0, 0)));
		set_u(glm::cross(n(), v()));

		set_view(glm::lookAt(position(), center(), up()));

		SetPerspective (45.0f, 1.0f*5/4,
		        0.1f, 10.0f);
		*/

		/*
		glm::mat4 view = glm::lookAt(glm::vec3(5.0, 5.0, 5.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 projection = glm::perspective(45.0f, 1.0f*5/4, 0.1f, 10.0f);
		set_view(view);
		set_projection(projection);
		*/
	}

}

