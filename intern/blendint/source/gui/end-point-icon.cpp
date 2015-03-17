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

#include <gui/font.hpp>
#include <gui/abstract-window.hpp>
#include <gui/end-point-icon.hpp>

namespace BlendInt {

  EndPointIcon::EndPointIcon ()
  : AbstractIcon()
  {
    int radius = pixel_size(7);

    set_size(2 * radius, 2 * radius);

    glGenVertexArrays(2, vao_);

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    GenerateVertices(-radius, -radius, radius, radius, pixel_size(1), RoundAll,
                     radius, &inner_verts, &outer_verts);

    vbo_.generate();

    glBindVertexArray(vao_[0]);

    vbo_.bind(0);
    vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

    glEnableVertexAttribArray(
        AbstractWindow::shaders()->location(
            Shaders::WIDGET_SIMPLE_TRIANGLE_COORD));
    glVertexAttribPointer(
        AbstractWindow::shaders()->location(
            Shaders::WIDGET_SIMPLE_TRIANGLE_COORD),
        3,
        GL_FLOAT,
        GL_FALSE, 0, 0);

    glBindVertexArray(vao_[1]);
    vbo_.bind(1);
    vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);

    glEnableVertexAttribArray(AttributeCoord);
    glVertexAttribPointer(AttributeCoord, 2,
    GL_FLOAT,
                          GL_FALSE, 0, 0);

    glBindVertexArray(0);
    vbo_.reset();
  }

  EndPointIcon::~EndPointIcon ()
  {
    glDeleteVertexArrays(2, vao_);
  }

  void EndPointIcon::Draw (int x,
                           int y,
                           const float* color_ptr,
                           short gamma,
                           float rotate,
                           float scale_x,
                           float scale_y) const
  {
    AbstractWindow::shaders()->widget_simple_triangle_program()->use();

    glUniform2f(
        AbstractWindow::shaders()->location(
            Shaders::WIDGET_SIMPLE_TRIANGLE_POSITION),
        x, y);
    glUniform4fv(
        AbstractWindow::shaders()->location(
            Shaders::WIDGET_SIMPLE_TRIANGLE_COLOR),
        1, color_ptr);
    glUniform1i(
        AbstractWindow::shaders()->location(
            Shaders::WIDGET_SIMPLE_TRIANGLE_GAMMA),
        gamma);

    glBindVertexArray(vao_[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(RoundAll) + 2);

    AbstractWindow::shaders()->widget_outer_program()->use();

    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_POSITION), x,
        y);
    glUniform4fv(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR), 1,
        AbstractWindow::theme()->regular().outline.data());

    glBindVertexArray(vao_[1]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, GetOutlineVertices(RoundAll) * 2 + 2);
  }

  void EndPointIcon::DrawInRect (const Rect& rect,
                                 int align,
                                 const float* color_ptr,
                                 short gamma,
                                 float rotate,
                                 bool scale) const
  {
  }

  void EndPointIcon::PerformSizeUpdate (const Size& size)
  {
    // does not allow size update
    return;
  }

}

