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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <blendint/gui/ortho-camera.hpp>

namespace BlendInt {

OrthoCamera::OrthoCamera()
    : AbstractCamera(),
      left_(0.f),
      right_(100.f),
      bottom_(0.f),
      top_(100.f),
      near_(100.f),
      far_(-100.f)
{
}

OrthoCamera::~OrthoCamera()
{
}

void OrthoCamera::SetOrtho (float left, float right, float bottom,
                            float top, float near, float far)
{
  left_ = left;
  right_ = right;
  bottom_ = bottom;
  top_ = top;
  near_ = near;
  far_ = far;

  set_projection(glm::ortho(left_, right_, bottom_, top_, near_, far_));
}

}
