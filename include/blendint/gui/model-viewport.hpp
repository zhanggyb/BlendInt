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

#pragma once

#include <blendint/gui/grid-floor.hpp>
#include <blendint/gui/perspective-camera.hpp>

#include <blendint/gui/abstract-viewport.hpp>

namespace BlendInt {

  /**
   * @brief A simple 3D viewport for demo and test only
   *
   * @ingroup blendint_gui_frames
   */
  class ModelViewport: public AbstractViewport
  {
  DISALLOW_COPY_AND_ASSIGN(ModelViewport);

  public:

    ModelViewport ();

    virtual ~ModelViewport ();

    virtual Size GetPreferredSize () const;

    bool LoadPrimitive (const RefPtr<AbstractPrimitive>& primitive);

  protected:

    virtual void PostPositionUpdate ();

    virtual void PostSizeUpdate ();

    virtual void RenderScene ();

  private:

    RefPtr<GridFloor> gridfloor_;

    RefPtr<PerspectiveCamera> default_camera_;

    RefPtr<AbstractPrimitive> primitive_;

    glm::mat4 projection_matrix_;

    glm::mat3 model_matrix_;
  };

}
