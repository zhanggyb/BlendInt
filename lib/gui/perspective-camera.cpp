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

#include <blendint/gui/perspective-camera.hpp>

#include <blendint/core/types.hpp>

namespace BlendInt {

float PerspectiveCamera::orbit_speed = 25.f;

float PerspectiveCamera::pan_speed = 50.f;

float PerspectiveCamera::zoom_speed = 100.f;

PerspectiveCamera::PerspectiveCamera ()
    : AbstractCamera(),
      fovy_(45.0f),
      aspect_(1.0),
      near_(0.1f),
      far_(100.f)
{
  set_position(5.f, 5.f, 5.f);
  set_center(0.f, 0.f, 0.f);
  set_up(0.f, 0.f, 1.f);

  glm::vec3 z = position() - center();
  glm::vec3 x = glm::cross(up(), z);
  glm::vec3 y = glm::cross(z, x);

  set_local_x(glm::normalize(x));
  set_local_y(glm::normalize(y));
  set_local_z(glm::normalize(z));

  set_view(glm::lookAt(position(), center(), local_y()));
  set_projection(glm::perspective(fovy_, aspect_, near_, far_));
}

PerspectiveCamera::~PerspectiveCamera ()
{
}

void PerspectiveCamera::SetPerspective (const float fovy, const float aspect,
                                        const float near, const float far)
{
  near_ = near;
  far_ = far;
  fovy_ = fovy;
  aspect_ = aspect;

  set_projection(glm::perspective(fovy_, aspect_, near_, far_));
}

void PerspectiveCamera::Orbit (float dx, float dy)
{
  float radius = glm::distance(last_position_, last_center_);

  glm::mat4 I = glm::mat4(1);
  glm::mat4 T1 = glm::translate(I, last_center_ * (-1.f));
  glm::mat4 T2 = glm::translate(I, last_center_);
  glm::mat4 Rh = glm::rotate(I, dx / radius / orbit_speed, up());
  glm::mat4 Rv = glm::rotate(I, -dy / radius / orbit_speed, local_x());

  glm::vec4 pos = glm::vec4(last_position_, 1.f);

  pos = T2 * Rv * Rh * T1 * pos;

  set_position(glm::vec3(pos));
  set_center(last_center_);

  glm::vec3 z = position() - center();
  glm::vec3 x = glm::cross(up(), z);
  glm::vec3 y = glm::cross(z, x);

  set_local_z(z);
  set_local_x(glm::normalize(x));
  set_local_y(glm::normalize(y));

  set_view(glm::lookAt(position(), center(), local_y()));
}

void PerspectiveCamera::Pan (float dx, float dy)
{
  glm::vec3 translate = local_x() * (dx / pan_speed) + local_y() * (dy / pan_speed);

  set_position(last_position_ + translate);
  set_center(last_center_ + translate);

  set_view(glm::lookAt(position(), center(), local_y()));
}

void PerspectiveCamera::Zoom (float dy)
{
  glm::vec3 direct_orig = glm::normalize(last_position_ - center());
  glm::vec3 pos = last_position_ + local_z() * (dy / zoom_speed);
  glm::vec3 direct_new = glm::normalize(pos - center());

  if(direct_new != direct_orig)
    return;

  set_position(pos);
  set_view(glm::lookAt(position(), center(), up()));
}

void PerspectiveCamera::SaveCurrentPosition()
{
  last_position_ = position();
}

void PerspectiveCamera::SaveCurrentCenter()
{
  last_center_ = center();
}

}

