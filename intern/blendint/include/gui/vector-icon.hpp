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

#include <core/color.hpp>
#include <opengl/gl-buffer.hpp>

#include <gui/abstract-icon.hpp>

namespace BlendInt {

  /**
   * Icon displayed with vertexes
   */
  class VectorIcon: public AbstractIcon
  {
  public:

    static const float num_tria_vert[3][2];

    static const unsigned int num_tria_face[1][3];

    static const float scroll_circle_vert[16][2];

    static const unsigned int scroll_circle_face[14][3];

    static const float menu_tria_vert[6][2];

    static const unsigned int menu_tria_face[2][3];

    static const float check_tria_vert[6][2];

    static const unsigned int check_tria_face[4][3];

    static const float cross_tria_vert[12][2];

    static const unsigned int cross_tria_face[10][3];

    VectorIcon (int width, int height);

    virtual ~VectorIcon ();

    void Load (const float (*vertex_array)[2],
               size_t array_size,
               const unsigned int (*vertex_indices)[3],
               size_t indeces_size);

    virtual void Draw (int x,
                       int y,
                       const float* color_ptr = Color(0x1A1A1A20).data(),
                       short gamma = 0,
                       float rotate = 0.f,
                       float scale_x = 1.f,
                       float scale_y = 1.f) const;

    virtual void DrawInRect (const Rect& rect,
                             int align,
                             const float* color_ptr = Color(0x1A1A1A20).data(),
                             short gamma = 0,
                             float rotate = 0.f,
                             bool scale = false) const;

  protected:

    virtual void PerformSizeUpdate (const Size& size);

  private:

    GLBuffer<ARRAY_BUFFER, 1> vertex_buffer_;
    GLBuffer<ELEMENT_ARRAY_BUFFER, 1> element_buffer_;

    GLuint vao_;

    int elements_;
  };
}
