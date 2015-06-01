/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <blendint/opengl/gl-framebuffer.hpp>

#include <blendint/gui/list-view.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

ListView::ListView ()
    : AbstractItemView(), highlight_index_(-1)
{
  set_size(400, 300);

  InitializeListView();
}

ListView::~ListView ()
{
  glDeleteVertexArrays(2, vao_);
}

bool ListView::IsExpandX () const
{
  return true;
}

bool ListView::IsExpandY () const
{
  return true;
}

const RefPtr<AbstractItemModel> ListView::GetModel () const
{
  return model_;
}

void ListView::SetModel (const RefPtr<AbstractItemModel>& model)
{
  if (model_) {
    model_ = model;
    RequestRedraw();
  } else if (model) {
    model_ = model;
    RequestRedraw();
  }

  if (model_) {
    int h = font_.height();
    h = model_->GetRowCount() * h;	// total height
  }
}

Size ListView::GetPreferredSize () const
{
  return Size(400, 300);
}

Response ListView::Draw (AbstractWindow* context)
{
  int y = size().height();
  const int h = font_.height();

  shaders()->widget_inner_program()->use();

  glUniform1i(shaders()->location(Shaders::WIDGET_INNER_GAMMA), 0);
  glUniform1i(shaders()->location(Shaders::WIDGET_INNER_SHADED), 0);
  glUniform4fv(shaders()->location(Shaders::WIDGET_INNER_COLOR),
               1, theme()->regular().inner.data());

  glBindVertexArray(vao_[0]);
  glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(round_type()) + 2);

  context->BeginPushStencil();	// inner stencil
  glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(round_type()) + 2);
  context->EndPushStencil();

  shaders()->widget_triangle_program()->use();

  glUniform1i(
      shaders()->location(Shaders::WIDGET_TRIANGLE_GAMMA), 0);
  glUniform1i(
      shaders()->location(Shaders::WIDGET_TRIANGLE_ANTI_ALIAS),
      0);
  glVertexAttrib4f(AttributeColor, 0.475f, 0.475f, 0.475f, 0.75f);

  glBindVertexArray(vao_[1]);

  int i = 0;
  while (y > 0) {
    y -= h;

    glUniform2f(shaders()->location(Shaders::WIDGET_TRIANGLE_POSITION),
                0, pixel_size(y));

    if (i == highlight_index_) {// TODO: use different functions for performance
      glUniform1i(shaders()->location(Shaders::WIDGET_TRIANGLE_GAMMA), -35);
    } else {
      if (i % 2 == 0) {
        glUniform1i(shaders()->location(Shaders::WIDGET_TRIANGLE_GAMMA), 0);
      } else {
        glUniform1i(shaders()->location(Shaders::WIDGET_TRIANGLE_GAMMA), 15);
      }
    }

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    i++;
  }

  RefPtr<AbstractItemModel> model = GetModel();
  if (model) {

    ModelIndex index = model->GetRootIndex();
    index = index.GetChildIndex(0, 0);

    Rect rect(0, size().height() - h, size().width(), h);

    while (index.valid()) {
      index.GetData()->DrawInRect(rect,
                                  AlignLeft | AlignVerticalCenter |
                                  AlignJustify | AlignBaseline,
                                  theme()->regular().text.data());
      index = index.GetDownIndex();
      rect.set_y(rect.y() - h);
    }

  }

  shaders()->widget_inner_program()->use();

  context->BeginPopStencil();	// pop inner stencil
  glBindVertexArray(vao_[0]);
  glDrawArrays(GL_TRIANGLE_FAN, 0, outline_vertex_count(round_type()) + 2);
  glBindVertexArray(0);
  context->EndPopStencil();

  return Finish;
}

Response ListView::PerformMousePress (AbstractWindow* context)
{
  if (model_) {

    ModelIndex index;

    int rows = model_->GetRowCount();

    if (rows > 0) {
      int h = font_.height();	// the row height
      int total = rows * h;

      int i = 0;
      if (total > size().height()) {
        i = position().y() - context->GetGlobalCursorPosition().y();
      } else {	// no vbar
        i = position().y() + size().height()
            - context->GetGlobalCursorPosition().y();
      }

      i = i / h;
      highlight_index_ = i;

      index = model_->GetRootIndex().GetChildIndex();
      while ((i > 0) && index.valid()) {
        index = index.GetDownIndex();
        i--;
      }

      if (!index.valid()) {
        highlight_index_ = -1;
      }
    }

  } else {
    highlight_index_ = -1;
  }

  return Finish;
}

ModelIndex ListView::GetIndexAt (const Point& point) const
{
  return ModelIndex();
}

void ListView::PerformSizeUpdate (const AbstractView* source,
                                  const AbstractView* target,
                                  int width,
                                  int height)
{
  if (target == this) {

    set_size(width, height);

    GLfloat h = font_.height();
    GLfloat verts[] = { 0.f, 0.f,
                        pixel_size(width), 0.f,
                        0.f, pixel_size(h),
                        pixel_size(width), pixel_size(h) };

    vbo_.bind(1);
    vbo_.set_data(sizeof(verts), verts);

    std::vector<GLfloat> inner_verts;
    GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);

    vbo_.bind(0);
    vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
    vbo_.reset();
  }

  if (source == this) {
    report_size_update(source, target, width, height);
  }
}

void ListView::InitializeListView ()
{
  GLfloat h = (GLfloat) font_.height();
  GLfloat verts[] = { 0.f, 0.f,
                      pixel_size(size().width()), 0.f,
                      0.f, pixel_size(h),
                      pixel_size(size().width()), pixel_size(h) };

  std::vector<GLfloat> inner_verts;

  GenerateVertices(size(), 0.f, RoundNone, 0.f, &inner_verts, 0);
  vbo_.generate();

  glGenVertexArrays(2, vao_);

  glBindVertexArray(vao_[0]);

  vbo_.bind(0);
  vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

  glEnableVertexAttribArray(AttributeCoord);
  glVertexAttribPointer(AttributeCoord, 3, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(vao_[1]);

  vbo_.bind(1);
  vbo_.set_data(sizeof(verts), verts);

  glEnableVertexAttribArray(AttributeCoord);
  glVertexAttribPointer(AttributeCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindVertexArray(0);
  vbo_.reset();
}

}
