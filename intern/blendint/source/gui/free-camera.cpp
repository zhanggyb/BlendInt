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

#include <gui/free-camera.hpp>

namespace BlendInt {

	FreeCamera::FreeCamera ()
	{
		set_position(5.0, 5.0, 5.0);
		set_center(0.0, 0.0, 0.0);
		set_up(0.0, 0.0, 1.0);
	}

	FreeCamera::~FreeCamera ()
	{
	}

	void FreeCamera::Update ()
	{
		//set_view(glm::lookAt(position(), center(), up()));

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

