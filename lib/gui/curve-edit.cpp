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

#include <blendint/gui/curve-edit.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

CurveEdit::CurveEdit ()
    : AbstractWidget()
{
  set_size(400, 300);

  std::vector<GLfloat> inner_verts;
  std::vector<GLfloat> outer_verts;

  GenerateVertices(size(), 1.f, RoundNone, 0.f, &inner_verts, &outer_verts);

  glGenVertexArrays(2, vao_);

  vbo_.generate();

  glBindVertexArray(vao_[0]);
  vbo_.bind(0);
  vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
  glEnableVertexAttribArray(AttributeCoord);
  glVertexAttribPointer(AttributeCoord, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(vao_[1]);
  vbo_.bind(1);
  vbo_.set_data(sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
  glEnableVertexAttribArray(AttributeCoord);
  glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);
  vbo_.reset();
}

CurveEdit::~CurveEdit ()
{
  glDeleteVertexArrays(2, vao_);
}

Size CurveEdit::GetPreferredSize () const
{
  return Size(400, 300);
}

bool CurveEdit::IsExpandX () const
{
  return true;
}

bool CurveEdit::IsExpandY () const
{
  return true;
}

void CurveEdit::PerformSizeUpdate(const AbstractView* source,
                                  const AbstractView* target,
                                  int width,
                                  int height)
{
  if (target == this) {

    set_size(width, height);

    std::vector<GLfloat> inner_verts;
    std::vector<GLfloat> outer_verts;

    GenerateVertices(size(), 1.f, RoundNone, 0.f, &inner_verts, &outer_verts);

    vbo_.bind(0);
    vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    vbo_.bind(1);
    vbo_.set_sub_data(0, sizeof(GLfloat) * outer_verts.size(), &outer_verts[0]);
    vbo_.reset();

    RequestRedraw();
  }

  if (source == this) {
    report_size_update(source, target, width, height);
  }
}

Response CurveEdit::Draw (AbstractWindow* context)
{
  AbstractWindow::shaders()->widget_debug_program()->use();

  glBindVertexArray(vao_[0]);
  glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(RoundNone) + 2);

  AbstractWindow::shaders()->widget_outer_program()->use();

  glUniform2f(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_OFFSET),
              0.f, 0.f);
  glUniform4fv(AbstractWindow::shaders()->location(Shaders::WIDGET_OUTER_COLOR),
               1, AbstractWindow::theme()->push_button().outline.data());

  glBindVertexArray(vao_[1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0,
               outline_vertex_count(RoundNone) * 2 + 2);

    return Finish;
}

}  // namespace BlendInt
