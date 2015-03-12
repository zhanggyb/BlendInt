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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include <opengl/opengl.hpp>

#include <gui/model-viewport.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

  ModelViewport::ModelViewport ()
      : AbstractViewport(640, 480)
  {
    projection_matrix_ = glm::ortho(0.f, (float) size().width(), 0.f,
                                    (float) size().height(), 100.f, -100.f);
    model_matrix_ = glm::mat3(1.f);

    default_camera_.reset(new PerspectiveCamera);

    // setup camera
    glm::vec3 pos = glm::vec3(8.f, -10.f, 6.f);
    glm::vec3 center = glm::vec3(0);
    glm::vec3 up = glm::vec3(0.0, 0.0, 1.0);
    default_camera_->LookAt(pos, center, up);

    default_camera_->SetPerspective(default_camera_->fovy(),
                                    1.f * size().width() / size().height());

    gridfloor_.reset(new GridFloor);
  }

  ModelViewport::~ModelViewport ()
  {
  }

  bool ModelViewport::LoadPrimitive (const RefPtr<AbstractPrimitive>& primitive)
  {
    if (!primitive) return false;

    primitive_ = primitive;
    RequestRedraw();
    return true;
  }

  Size ModelViewport::GetPreferredSize () const
  {
    return Size(640, 480);
  }

  void ModelViewport::PerformPositionUpdate (const PositionUpdateRequest& request)
  {
    if (request.target() == this) {

      Point offset = GetOffset();
      float x = static_cast<float>(request.position()->x() + offset.x());
      float y = static_cast<float>(request.position()->y() + offset.y());

      projection_matrix_ = glm::ortho(x, x + (float) size().width(), y,
                                      y + (float) size().height(), 100.f,
                                      -100.f);

      model_matrix_ = glm::translate(glm::mat3(1.f), glm::vec2(x, y));

      set_position(*request.position());

    }

    if (request.source() == this) {
      ReportPositionUpdate(request);
    }
  }

  void ModelViewport::PerformSizeUpdate (const SizeUpdateRequest& request)
  {
    if (request.target() == this) {

      Point offset = GetOffset();
      float x = static_cast<float>(position().x() + offset.x());
      float y = static_cast<float>(position().y() + offset.y());

      projection_matrix_ = glm::ortho(x, x + (float) request.size()->width(), y,
                                      y + (float) request.size()->height(),
                                      100.f, -100.f);

      set_size(*request.size());

      default_camera_->SetPerspective(
          default_camera_->fovy(),
          1.f * request.size()->width() / request.size()->height());
    }

    if (request.source() == this) {
      ReportSizeUpdate(request);
    }
  }

  void ModelViewport::RenderScene ()
  {
    gridfloor_->Render(default_camera_->projection(), default_camera_->view());

    if (primitive_) {
      primitive_->Render(default_camera_->projection(),
                         default_camera_->view());
    }
  }

}

