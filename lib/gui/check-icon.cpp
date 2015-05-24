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

#include <blendint/gui/check-icon.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

CheckIcon::CheckIcon ()
    : AbstractIcon()
{
  int rad = pixel_size(7);
  set_size(rad * 2, rad * 2);

  glGenVertexArrays(2, vao_);

  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;

  GenerateRawVertices(-rad, -rad, rad, rad, 1.f, RoundAll, 5.f, Vertical,
                   15, -10, &inner_verts, &outer_verts);

  vbo_.generate();

  glBindVertexArray(vao_[0]);

  vbo_.bind(0);
  vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

  glEnableVertexAttribArray(
      shaders()->location(
          Shaders::WIDGET_SIMPLE_TRIANGLE_COORD));
  glVertexAttribPointer(
      shaders()->location(
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
  shaders()->widget_simple_triangle_program()->use();

  glUniform2f(
      shaders()->location(
          Shaders::WIDGET_SIMPLE_TRIANGLE_POSITION),
      x, y);
  glUniform4fv(
      shaders()->location(
          Shaders::WIDGET_SIMPLE_TRIANGLE_COLOR),
      1, theme()->menu().inner.data());
  glUniform1i(
      shaders()->location(
          Shaders::WIDGET_SIMPLE_TRIANGLE_GAMMA),
      gamma);

  glBindVertexArray(vao_[0]);
  glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(RoundAll) + 2);

  shaders()->widget_outer_program()->use();

  glUniform2f(shaders()->location(Shaders::WIDGET_OUTER_OFFSET),
              x, y);
  glUniform4fv(shaders()->location(Shaders::WIDGET_OUTER_COLOR),
               1, theme()->menu().outline.data());

  glBindVertexArray(vao_[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, outline_vertex_count(RoundAll) * 2 + 2);
}

void CheckIcon::DrawInRect (const Rect& rect,
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

  shaders()->widget_simple_triangle_program()->use();

  glUniform2f(
      shaders()->location(
          Shaders::WIDGET_SIMPLE_TRIANGLE_POSITION),
      x, y);
  glUniform4fv(
      shaders()->location(
          Shaders::WIDGET_SIMPLE_TRIANGLE_COLOR),
      1, theme()->menu().inner.data());
  glUniform1i(
      shaders()->location(
          Shaders::WIDGET_SIMPLE_TRIANGLE_GAMMA),
      gamma);

  glBindVertexArray(vao_[0]);
  glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(RoundAll) + 2);

  shaders()->widget_outer_program()->use();

  glUniform2f(shaders()->location(Shaders::WIDGET_OUTER_OFFSET),
              x, y);
  glUniform4fv(shaders()->location(Shaders::WIDGET_OUTER_COLOR),
               1, theme()->menu().outline.data());

  glBindVertexArray(vao_[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, outline_vertex_count(RoundAll) * 2 + 2);
}

void CheckIcon::PerformSizeUpdate (int width, int height)
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
