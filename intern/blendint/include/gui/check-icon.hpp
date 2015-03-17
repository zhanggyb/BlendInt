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

#include <opengl/gl-buffer.hpp>
#include <gui/abstract-icon.hpp>

namespace BlendInt {

  /**
   * @brief Icon used in CheckButton
   *
   * @ingroup blendint_gui_forms
   */
  class CheckIcon: public AbstractIcon
  {
  public:

    CheckIcon ();

    virtual ~CheckIcon ();

    virtual void Draw (int x,
                       int y,
                       const float* color_ptr = Color(Color::Black).data(),
                       short gamma = 0,
                       float rotate = 0.f,
                       float scale_x = 1.f,
                       float scale_y = 1.f) const;

    virtual void DrawInRect (const Rect& rect,
                             int align,
                             const float* color_ptr =
                                 Color(Color::Black).data(),
                             short gamma = 0,
                             float rotate = 0.f,
                             bool scale = false) const final;

  protected:

    virtual void PerformSizeUpdate (const Size& size) final;

    virtual void PerformRoundTypeUpdate (int type) final;

    virtual void PerformRoundRadiusUpdate (float radius) final;

  private:

    GLuint vao_[2];
    GLBuffer<ARRAY_BUFFER, 2> vbo_;

  };

}
