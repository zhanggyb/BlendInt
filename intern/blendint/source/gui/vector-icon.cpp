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
 * License along with BlendInt.	 If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <opengl/opengl.hpp>
#include <gui/vector-icon.hpp>

#include <gui/abstract-window.hpp>

#ifndef WIDGET_AA_JITTER
#define WIDGET_AA_JITTER 8
#endif

namespace BlendInt {

  const float VectorIcon::num_tria_vert[3][2] = {
      { -0.352077, 0.532607 },
      { -0.352077, -0.549313 },
      { 0.330000, -0.008353 }
  };

  const unsigned int VectorIcon::num_tria_face[1][3] = {
      { 0, 1, 2 }
  };

  const float VectorIcon::scroll_circle_vert[16][2] = {
      { 0.382684, 0.923879 },
      { 0.000001, 1.000000 },
      { -0.382683, 0.923880 },
      { -0.707107, 0.707107 },
      { -0.923879, 0.382684 },
      { -1.000000, 0.000000 },
      { -0.923880, -0.382684 },
      { -0.707107, -0.707107 },
      { -0.382683, -0.923880 },
      { 0.000000, -1.000000 },
      { 0.382684, -0.923880 },
      { 0.707107, -0.707107 },
      { 0.923880, -0.382684 },
      { 1.000000, -0.000000 },
      { 0.923880, 0.382683 },
      { 0.707107, 0.707107 }
  };

  const unsigned int VectorIcon::scroll_circle_face[14][3] = {
      { 0, 1, 2 },
      { 2, 0, 3 },
      { 3, 0, 15 },
      { 3, 15, 4 },
      { 4, 15, 14 },
      { 4, 14, 5 },
      { 5, 14, 13 },
      { 5, 13, 6 },
      { 6, 13, 12 },
      { 6, 12, 7 },
      { 7, 12, 11 },
      { 7, 11, 8 },
      { 8, 11, 10 },
      { 8, 10, 9 }
  };

  const float VectorIcon::menu_tria_vert[6][2] = {
      { -0.33, 0.16 },
      { 0.33, 0.16 },
      { 0, 0.82 },
      { 0, -0.82 },
      { -0.33, -0.16 },
      { 0.33, -0.16 }
  };

  const unsigned int VectorIcon::menu_tria_face[2][3] = {
      { 2, 0, 1 },
      { 3, 5, 4 }
  };

  const float VectorIcon::check_tria_vert[6][2] = {
      { -0.578579, 0.253369 },
      { -0.392773, 0.412794 },
      { -0.004241, -0.328551 },
      { -0.003001, 0.034320 },
      { 1.055313, 0.864744 },
      { 0.866408, 1.026895 }
  };

  const unsigned int VectorIcon::check_tria_face[4][3] = {
      { 3, 2, 4 },
      { 3, 4, 5 },
      { 1, 0, 3 },
      { 0, 2, 3 }
  };

  const float VectorIcon::cross_tria_vert[12][2] = {
      { 0.551543, 0.721249 },
      { 0.000000, 0.169706 },
      { -0.551543, 0.721249 },
      { -0.721249, 0.551543 },
      { -0.169706, 0.000000 },
      { -0.721249, -0.551543 },
      { -0.551543, -0.721249 },
      { 0.000000, -0.169706 },
      { 0.551543, -0.721249 },
      { 0.721249, -0.551543 },
      { 0.169706, 0.000000 },
      { 0.721249, 0.551543 }
  };

  const unsigned int VectorIcon::cross_tria_face[10][3] = {
      { 10, 11, 0 },
      { 0, 1, 10 },
      { 1, 2, 3 },
      { 3, 4, 1 },
      { 4, 5, 6 },
      { 6, 7, 4 },
      { 7, 8, 9 },
      { 9, 10, 7 },
      { 10, 1, 4 },
      { 4, 7, 10 }
  };

  VectorIcon::VectorIcon (int width, int height)
  : AbstractIcon(width, height), vao_(0), elements_(0)
  {
  }

  VectorIcon::~VectorIcon ()
  {
    glDeleteVertexArrays(1, &vao_);
  }

  void VectorIcon::Load (const float (*vertex_array)[2],
                         size_t array_size,
                         const unsigned int (*vertex_indices)[3],
                         size_t indeces_size)
  {
    if (!glIsVertexArray(vao_)) {
      glGenVertexArrays(1, &vao_);
    }

    glBindVertexArray(vao_);

    vertex_buffer_.generate();
    vertex_buffer_.bind();
    vertex_buffer_.set_data(array_size * sizeof(vertex_array[0]),
                            vertex_array[0]);

    glEnableVertexAttribArray(AttributeCoord);
    glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

    element_buffer_.generate();
    element_buffer_.bind();
    element_buffer_.set_data(indeces_size * sizeof(vertex_indices[0]),
                             vertex_indices[0]);

    glBindVertexArray(0);

    vertex_buffer_.reset();
    element_buffer_.reset();

    elements_ = indeces_size * 3;
  }

  void VectorIcon::Draw (int x,
                         int y,
                         const float* color_ptr,
                         short gamma,
                         float rotate,
                         float scale_x,
                         float scale_y) const
  {
    AbstractWindow::shaders()->widget_triangle_program()->use();

    glVertexAttrib4fv(AttributeColor, color_ptr);
    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_POSITION),
        x, y);
    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_GAMMA),
        gamma);
    glUniform1i(
        AbstractWindow::shaders()->location(
            Shaders::WIDGET_TRIANGLE_ANTI_ALIAS),
        1);

    glUniform1f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_ROTATION),
        rotate);
    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_SCALE),
        scale_x, scale_y);

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, elements_,
    GL_UNSIGNED_INT,
                   BUFFER_OFFSET(0));

    glUniform1f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_ROTATION),
        0.f);
    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_SCALE),
        1.f, 1.f);
  }

  void VectorIcon::DrawInRect (const Rect& rect,
                               int align,
                               const float* color_ptr,
                               short gamma,
                               float rotate,
                               bool scale) const
  {
    int x = rect.left();
    int y = rect.bottom();

    if (align & AlignLeft) {
      x = rect.left() + size().width() / 2;
    } else if (align & AlignRight) {
      x = rect.right() - size().width() / 2;
    } else if (align & AlignHorizontalCenter) {
      x = rect.hcenter();
    }

    if (align & AlignTop) {
      y = rect.top() - size().height() / 2;
    } else if (align & AlignBottom) {
      y = rect.bottom() + size().height() / 2;
    } else if (align & AlignVerticalCenter) {
      y = rect.vcenter();
    }

    AbstractWindow::shaders()->widget_triangle_program()->use();

    glVertexAttrib4fv(AttributeColor, color_ptr);
    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_POSITION),
        x, y);
    glUniform1i(
        AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_GAMMA),
        gamma);
    glUniform1i(
        AbstractWindow::shaders()->location(
            Shaders::WIDGET_TRIANGLE_ANTI_ALIAS),
        1);
    glUniform1f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_ROTATION),
        rotate);

    if (scale) {
      float scale_x = rect.width() * 1.f / size().width();
      float scale_y = rect.height() * 1.f / size().height();
      glUniform2f(
          AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_SCALE),
          scale_x, scale_y);
    } else {
      glUniform2f(
          AbstractWindow::shaders()->location(Shaders::WIDGET_TRIANGLE_SCALE),
          1.f, 1.f);
    }

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, elements_,
    GL_UNSIGNED_INT,
                   BUFFER_OFFSET(0));
  }

  void VectorIcon::PerformSizeUpdate (const Size& size)
  {
    set_size(size);
  }

}
