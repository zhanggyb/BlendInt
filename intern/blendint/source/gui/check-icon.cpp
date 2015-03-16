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

#include <gui/check-icon.hpp>
#include <gui/abstract-window.hpp>

namespace BlendInt {

  CheckIcon::CheckIcon ()
  {
    set_size(18, 18);
    set_round_type(RoundAll);
    set_radius(5.0);

    glGenVertexArrays(2, vao_);

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    GenerateRoundedVertices(Vertical,
                            15,
                            -10,
                            &inner_verts,
                            &outer_verts);

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

  CheckIcon::~CheckIcon ()
  {
    glDeleteVertexArrays(2, vao_);
  }

  void CheckIcon::Draw (int x,
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
        1, AbstractWindow::theme()->menu().inner.data());
    glUniform1i(
        AbstractWindow::shaders()->location(
            Shaders::WIDGET_SIMPLE_TRIANGLE_GAMMA),
        gamma);

    glBindVertexArray(vao_[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, GetOutlineVertices(round_type()) + 2);

    AbstractWindow::shaders()->widget_outer_program()->use();

    glUniform2f(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_POSITION), x,
        y);
    glUniform4fv(
        AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR), 1,
        AbstractWindow::theme()->menu().outline.data());

    glBindVertexArray(vao_[1]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0,
                 GetOutlineVertices(round_type()) * 2 + 2);

  }

  void CheckIcon::PerformSizeUpdate (const Size& size)
  {
    // does not allow size update
  }

  void CheckIcon::PerformRoundTypeUpdate (int type)
  {
    // does not allow round type update
  }

  void CheckIcon::PerformRoundRadiusUpdate (float radius)
  {
    // does not allow round radius update
  }

}

